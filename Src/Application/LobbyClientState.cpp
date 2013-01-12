//---------------------------------------------------------------------------
// LobbyClientState.cpp
//---------------------------------------------------------------------------

/**
@file LobbyClientState.cpp

Contiene la implementación del estado de lobby del cliente.

@see Application::CApplicationState
@see Application::CLobbyClientState

@author David Llansó
@date Agosto, 2010
*/

#include "LobbyClientState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include "net/Manager.h"
#include "net/Cliente.h"
#include "net/factoriared.h"
#include "net/paquete.h"
#include "net/conexion.h"


#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CLobbyClientState::~CLobbyClientState() 
	{
	} // ~CLobbyClientState

	//--------------------------------------------------------

	bool CLobbyClientState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("NetLobbyClient.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Start")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyClientState::startReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyClientState::backReleased, this));
	
		return true;

	} // init

	//--------------------------------------------------------

	void CLobbyClientState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CLobbyClientState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

		// NET
		Net::CManager::getSingletonPtr()->addObserver(this); // Se registra como IObserver
		Net::CManager::getSingletonPtr()->activateAsClient(); //  Activar la red como tipo Client

	} // activate

	//--------------------------------------------------------

	void CLobbyClientState::deactivate() 
	{	
		Net::CManager::getSingletonPtr()->removeObserver(this);

		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);

		CApplicationState::deactivate();


	} // deactivate

	//--------------------------------------------------------

	void CLobbyClientState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	
	bool CLobbyClientState::keyPressed(GUI::TKey key)
	{
	   return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CLobbyClientState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->setState("netmenu");
			break;
		case GUI::Key::RETURN:
			doStart();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CLobbyClientState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLobbyClientState::mousePressed(const GUI::CMouseState &mouseState)
	{
		//CEGUI::System::getSingleton().injectMouseButtonDown(mouseState.);
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CLobbyClientState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CLobbyClientState::startReleased(const CEGUI::EventArgs& e)
	{
		doStart();
		return true;
	} // startReleased
			
	//--------------------------------------------------------

	bool CLobbyClientState::backReleased(const CEGUI::EventArgs& e)
	{
		Net::CManager::getSingletonPtr()->deactivateNetwork(); // NET: deactivate
		_app->setState("netmenu");
		return true;

	} // backReleased
			
	//--------------------------------------------------------

	void CLobbyClientState::doStart()
	{
		// Deshabilitamos el botón de Start
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Start")->setEnabled(false);
		
		// Actualizamos el status
		CEGUI::Window * status = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Status");
		status->setText("Status: Connecting...");

		// Obtenemos la ip desde el Editbox
		CEGUI::String ip = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Editbox")->getText();

		// NET: Conectamos
		Net::CManager::getSingletonPtr()->connectTo((char*)ip.c_str(),1234,1);

		// Actualizamos el status
		status->setText("Status: Connected to server. Waiting to start game...");

		//tickNet is in charge of receiving the message and change the state

	} // doStart


	/**********************
		NET: IOBSERVER
	********************/
	void CLobbyClientState::dataPacketReceived(Net::CPaquete* packet)
	{
		Net::NetMessageType rxMsg;
			memcpy(&rxMsg, packet->getData(),sizeof(rxMsg)); // Packet: "NetMessageType | extraData"
		
		switch (rxMsg)
		{

		// TODO Gestionar la carga del blueprints cliente y del mapa cuando  
		// se reciba un mensaje de red de tipo LOAD_MAP. Cuando se finalice
		// la carga avisar al servidor enviando un mensaje tipo MAP_LOADED
		case Net::LOAD_MAP:

#if _DEBUG	fprintf (stdout, "NET::CLIENT::RX>> LOAD_MAP.\n");
#endif			
			// Cargamos el archivo con las definiciones de las entidades del nivel.
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_client.txt"))			{
				CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar el nivel");
				Net::CManager::getSingletonPtr()->deactivateNetwork();
				_app->exitRequest();

			// Cargamos el nivel a partir del nombre del mapa. 
			} else if (!Logic::CServer::getSingletonPtr()->loadLevel("map_client.txt")){
				CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar el nivel");
				Net::CManager::getSingletonPtr()->deactivateNetwork();
				_app->exitRequest();

			} else { //Avisamos de que hemos terminado la carga.

#if _DEBUG		fprintf (stdout, "NET::CLIENT>> MAPA Cargado.\n"); 
#endif			
				Net::NetMessageType txMsg = Net::NetMessageType::MAP_LOADED; // Informamos de carga finalizada
					Net::CManager::getSingletonPtr()->send(&txMsg, sizeof(txMsg));

#if _DEBUG		fprintf (stdout, "NET::CLIENT::TX>> MAP_LOADED.\n");
#endif
			}

			break;

		// CARGAR PLAYER
		case Net::LOAD_PLAYER: { 

#if _DEBUG	fprintf (stdout, "NET::CLIENT::RX>> LOAD_PLAYER.\n");
#endif

			Net::NetID id;
				memcpy(&id, packet->getData() + sizeof(rxMsg), sizeof(id) ); //	Packet: "NetMessageType | extraData(NetID)"		
			std::stringstream number;
				number << id;			// TODO Por qué es necesario pasar por stringstream para el append?
			std::string name("Player");
				name.append(number.str());

			// TODO Llamar al método de creación del jugador. Deberemos decidir
			// si el jugador es el jugador local (si el ID del paquete coincide 
			// con nuestro ID de red).
			bool localPlayer = id == Net::CManager::getSingletonPtr()->getID(); // id rx == id local?
			Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, localPlayer);
			// HACK Deberíamos poder propocionar caracteríasticas
			// diferentes según el cliente (nombre, modelo, etc.). Esto es una
			// aproximación, solo cambiamos el nombre y decimos si es el jugador
			// local. Los datos deberían llegar en el paquete de red.

			//Enviamos el mensaje de que se ha creado el jugador
			Net::NetMessageType msg = Net::PLAYER_LOADED;
			Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg));

#if _DEBUG	fprintf (stdout, "NET::CLIENT::TX>> PLAYER_LOADED.\n");
#endif
		}	break;	

		case Net::START_GAME:
#if _DEBUG	fprintf (stdout, "NET::CLIENT::RX>> START_GAME.\n");
#endif		_app->setState("game"); // Rx mensaje START -> game state
			break;
		}

	} // dataPacketReceived

	//--------------------------------------------------------


} // namespace Application
