//---------------------------------------------------------------------------
// LobbyClientState.cpp
//---------------------------------------------------------------------------

/**
@file LobbyClientState.cpp

Contiene la implementaci�n del estado de lobby del cliente.

@see Application::CApplicationState
@see Application::CLobbyClientState

@author David Llans�
@date Agosto, 2010
*/

#include "LobbyClientState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include "NET/Manager.h"
#include "NET/Cliente.h"
#include "NET/factoriared.h"
#include "NET/paquete.h"
#include "NET/conexion.h"
#include "NET/buffer.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "APP::CLIENT>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Application {

	CLobbyClientState::~CLobbyClientState() 
	{
	} // ~CLobbyClientState

	//--------------------------------------------------------

	bool CLobbyClientState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el men�
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("NetLobbyClient.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient");
		
		// Asociamos los botones del men� con las funciones que se deben ejecutar.
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

		// Activamos la ventana que nos muestra el men� y activamos el rat�n.
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

		// Desactivamos la ventana GUI con el men� y el rat�n.
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
		// Deshabilitamos el bot�n de Start
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
		Net::CBuffer rxSerialMsg; // Packet: "NetMessageType | extraData"
			rxSerialMsg.write(packet->getData(),packet->getDataLength());
			rxSerialMsg.reset();
		
		Net::NetMessageType rxMsgType;
			rxSerialMsg.read( &rxMsgType, sizeof(rxMsgType) );
		switch (rxMsgType)
		{

		// TODO Gestionar la carga del blueprints cliente y del mapa cuando  
		// se reciba un mensaje de red de tipo LOAD_MAP. Cuando se finalice
		// la carga avisar al servidor enviando un mensaje tipo MAP_LOADED
		case Net::LOAD_MAP:

			LOG("RX LOAD_MAP");
		
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

			} else { //Avisamos de que hemos terminado la carga y tx info de player
				
				LOG("MAPA Cargado"); 	

				// OBTENER PLAYER INFO
				//// TODO obtener nickname
				std::ostringstream number; 
					number << rand() % 8;			
				std::string playerNick("Player");
					playerNick.append(number.str());		
				//CEGUI::String ip = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Editbox")->getText();
LOG(playerNick);
				//// TODO obtener modelo: sin mesh y luego a�adirselo
				//CEGUI::String ip = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Editbox")->getText();
				std::string models[] = {"loco.mesh", "marine.mesh"};//, "AttaObrera.mesh", "bioshock.mesh","AttaSoldada.mesh", "aranna.mesh"};
				int nModels = sizeof(models)/sizeof(std::string);
				std::string playerModel = models[ rand() % nModels];
LOG(playerModel);
				// HACK Lo suyo ser�a que cada uno ejecutara su propio createPlayer y que se propagara el LOAD_PLAYER como si fuera de server
				// TX MAP LOADED
				Net::CBuffer txSerialMsg;

					Net::NetMessageType msgType = Net::NetMessageType::MAP_LOADED; // Informamos de carga finalizada
						txSerialMsg.write(&msgType, sizeof(msgType));

					unsigned int nickSize = playerNick.size();  // TODO unas funciones de serializaci�n de tipo ser�n de mucha ayuda
						txSerialMsg.write(&nickSize,sizeof(nickSize));			
						txSerialMsg.write((void*)(playerNick.c_str()),nickSize);

					unsigned int modelSize = playerModel.size(); 
						txSerialMsg.write(&modelSize,sizeof(modelSize));			
						txSerialMsg.write((void*)(playerNick.c_str()),modelSize);
				
				Net::CManager::getSingletonPtr()->send(txSerialMsg.getbuffer(),	txSerialMsg.getSize() );
				
				LOG("TX MAP_LOADED");
			}

			break;

		// CARGAR PLAYER
		case Net::LOAD_PLAYER: { 	 //	Packet: "NetMessageType | netID | playerInfo"		

			Net::NetID id;
				rxSerialMsg.read(&id, sizeof(id) );

			// PLAYER INFO  	
			unsigned int nickSize;
				rxSerialMsg.read(&nickSize, sizeof(nickSize)); // Leemos longitud		
			char* playerNick =  new char[nickSize];		// Reservamos bloque car[] de tama�o size
				rxSerialMsg.read(playerNick, nickSize);		
			unsigned int modelSize;
				rxSerialMsg.read(&modelSize, sizeof(modelSize)); // Leemos longitud		
			char* playerModel = new char[modelSize];		// Reservamos bloque car[] de tama�o size
				rxSerialMsg.read(playerModel, modelSize);	
			
			LOG( "RX LOAD_PLAYER " << playerNick);

			// TODO Llamar al m�todo de creaci�n del jugador. Deberemos decidir
			// si el jugador es el jugador local (si el ID del paquete coincide 
			// con nuestro ID de red).
			bool isLocalPlayer = id == Net::CManager::getSingletonPtr()->getID(); // id rx == id local?

			Logic::CServer::getSingletonPtr()->getMap()->createPlayer(playerNick, playerModel,  isLocalPlayer);
			// HACK Deber�amos poder propocionar caracter�asticas
			// diferentes seg�n el cliente (nombre, modelo, etc.). Esto es una
			// aproximaci�n, solo cambiamos el nombre y decimos si es el jugador
			// local. Los datos deber�an llegar en el paquete de red.

			//Enviamos el mensaje de que se ha creado el jugador
			Net::NetMessageType msg = Net::NetMessageType::PLAYER_LOADED;
				Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg));

			LOG("TX PLAYER_LOADED " << playerNick);

			delete[] playerNick;
			delete[] playerModel;

		}	break;	

		case Net::START_GAME:

			LOG("RX START_GAME");
			_app->setState("game"); // Rx mensaje START -> game state
			break;
		}

	} // dataPacketReceived

	//--------------------------------------------------------


} // namespace Application