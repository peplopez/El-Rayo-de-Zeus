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

#include "NET/Buffer.h"
#include "NET/Manager.h"
#include "NET/Packet.h"
#include "NET/Serializable.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>
#include <time.h>

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"

//PT nuevos
#include <CEGUIDataContainer.h>
#include <CEGUIWindowRenderer.h>


#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "APP::LOBBY_CLIENT>> " << msg << std::endl;
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
	srand(time(0)); // HACK necesario subsistema random

		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		//CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("NetLobbyClient.layout");
		//_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient");

		// Cargamos la ventana que muestra el menú con LUA
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("NetLobbyClient");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initNetLobbyClient");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Connect")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyClientState::startReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyClientState::backReleased, this));

			//Modelo .mesh o personaje
		   _cbModel = static_cast<CEGUI::Combobox*>(CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/ModelBox"));

			// add items to the combobox list
			_cbModel->addItem(new CEGUI::ListboxTextItem("Espartano",0));
			_cbModel->addItem(new CEGUI::ListboxTextItem("Loco",1));
			_cbModel->addItem(new CEGUI::ListboxTextItem("Marine",2));
			_cbModel->addItem(new CEGUI::ListboxTextItem("Bioshock",3));
			_cbModel->addItem(new CEGUI::ListboxTextItem("Medusa",4));


			_cbModel->setReadOnly(true);

			//Color
			_cbColor = static_cast<CEGUI::Combobox*>(CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/ColorBox"));
			_cbColor->addItem(new CEGUI::ListboxTextItem("Rojo",10));
			_cbColor->addItem(new CEGUI::ListboxTextItem("Verde",11));
			_cbColor->addItem(new CEGUI::ListboxTextItem("Amarillo",12));
			_cbColor->addItem(new CEGUI::ListboxTextItem("Azul",13));

			_cbColor->setReadOnly(true);

	
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
		/*CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();
		*/

		// Activamos la ventana que nos muestra el menú y activamos el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showNetLobbyClient");

		// NET
		Net::CManager::getSingletonPtr()->addObserver(this); // Se registra como IObserver
		Net::CManager::getSingletonPtr()->activateAsClient(); //  Activar la red como tipo Client

	} // activate

	//--------------------------------------------------------

	void CLobbyClientState::deactivate() 
	{	
		Net::CManager::getSingletonPtr()->removeObserver(this);

		// Desactivamos la ventana GUI con el menú y el ratón.
		/*CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		*/

		// Desactivamos la ventana que nos muestra el menú y desactivamos el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideNetLobbyClient");

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
		//case GUI::Key::R:
		//	ScriptManager::CServer::getSingletonPtr()->reloadScript("NetLobbyClient");
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("reloadNetLobbyClient");
		//	break;
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
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Connect")->setEnabled(false);
		
		// Actualizamos el status
		CEGUI::Window * status = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Status");
		status->setText("Status: Connecting...");

		// Obtenemos la ip desde el Editbox
		CEGUI::String ip = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/IPBox")->getText();

		
		// NET: Conectamos

		Net::CManager::getSingletonPtr()->connectTo((char*)ip.c_str(),1234,1);

		// Actualizamos el status
		status->setText("Status: Connected to server. Waiting to start game...");

		//tickNet is in charge of receiving the message and change the state

	} // doStart


	/**********************
		NET: IOBSERVER
	********************/
	void CLobbyClientState::dataPacketReceived(Net::CPacket* packet)
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

			//PT Carga de mapRed en la lista de mapas
			_mapsToLoad.push_back("mapRed");

			//[ƒ®§] CARGA de Blueprints, Arquetypes y Map adelantada

			// Cargamos el archivo con las definiciones de las entidades del nivel.
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_client.txt")){
				CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar el nivel");
				Net::CManager::getSingletonPtr()->deactivateNetwork();
				_app->exitRequest();
		
			// Add - ESC
			// Cargamos el archivo con las definiciones de los archetypes
			} else if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes_client.txt")) {
				CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar archetypes");
				Net::CManager::getSingletonPtr()->deactivateNetwork();
				_app->exitRequest();
			
			// Add - JLS
			// Cargamos los anillos a partir del nombre del mapa. 
			} else if (!Logic::CServer::getSingletonPtr()->setRingPositions()) {//[ƒ®§] Esto no deberia ejecutarse como parte del loadLevel...?
				_app->exitRequest();
		
				// Cargamos el nivel a partir del nombre del mapa. 
			}
			//PT
			//else if (!Logic::CServer::getSingletonPtr()->loadMap("mapRed")){
			else if(!Logic::CServer::getSingletonPtr()->loadWorld(_mapsToLoad)){
				CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar el nivel");
				Net::CManager::getSingletonPtr()->deactivateNetwork();
				_app->exitRequest();

			} else { //Avisamos de que hemos terminado la carga y tx info de player
				
				LOG("MAPA Cargado"); 	

				// OBTENER PLAYER INFO
				std::string playerNick = std::string( CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/NickBox")->getText().c_str() );
				//// TODO obtener modelo mediante ddList -> tabla mesh			
				//"loco.mesh", "marine.mesh", "AttaObrera.mesh", "bioshock.mesh","AttaSoldada.mesh", "aranna.mesh"
				//std::string playerModel = std::string( CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/ModelBox")->getText().c_str() );
				std::string playerModel = "";
				//COLOR
				std::string playerColor = std::string(CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/ColorBox")->getText().c_str());

				//int playerModelID = (int)CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/ModelBox")->getID();
				int playerModelID = _cbModel->getSelectedItem()->getID();

				//PT. se rellena el string de playerModel con su correspondiente asociacion a su ID
				switch(playerModelID)
				{
					case 0: //espartano
						playerModel = "spartan2.4.mesh";
						break;
					case 1: //loco
						playerModel = "loco.mesh";
						break;
					case 2: //marine
						playerModel = "marine.mesh";
						break;
					case 3: //biosock
						playerModel = "bioshock.mesh";
						break;
					case 4: //medusa
						playerModel = "medusa.mesh";
					default: //espartano
						playerModel = "spartan2.4.mesh";
						break;
				}

				// HACK Lo suyo sería que cada uno ejecutara su propio createPlayer y que se propagara el LOAD_PLAYER como si fuera de server
				// TX MAP LOADED
				Net::CBuffer txSerialMsg;
				
				Net::NetMessageType msgType = Net::MAP_LOADED; // Informamos de carga finalizada
				txSerialMsg.write(&msgType, sizeof(msgType));
				Net::Serializable::serialize(txSerialMsg, playerNick);
				Net::Serializable::serialize(txSerialMsg, playerModel);
				Net::Serializable::serialize(txSerialMsg, playerColor);
				Net::CManager::getSingletonPtr()->send(txSerialMsg.getbuffer(),	txSerialMsg.getSize() );
				
				LOG("TX MAP_LOADED with Nick=" << playerNick << " and Model=" << playerModel << " and color = " << playerColor );
			}

			break;

		// CARGAR PLAYER
		case Net::LOAD_PLAYER: { 	 //	Packet: "NetMessageType | netID | playerInfo"		

			Net::NetID id;
				rxSerialMsg.read(&id, sizeof(id) );

			// PLAYER INFO
			std::string playerNick(		Net::Serializable::deserializeString(rxSerialMsg) );
			std::string playerModel(	Net::Serializable::deserializeString(rxSerialMsg) );
			std::string playerColor(	Net::Serializable::deserializeString(rxSerialMsg) );

			LOG( "RX LOAD_PLAYER " << id << " with Nick=" << playerNick << " and Model=" << playerModel << " and color = " << playerColor );

			// [FRS] Llamar al método de creación del jugador. Deberemos decidir
			// si el jugador es el jugador local (si el ID del packet coincide 
			// con nuestro ID de red).
			bool isLocalPlayer = id == Net::CManager::getSingletonPtr()->getID(); // id rx == id local?

			Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer(playerNick, isLocalPlayer, playerModel);
			// HACK Deberíamos poder propocionar caracteríasticas
			// diferentes según el cliente (nombre, modelo, etc.). Esto es una
			// aproximación, solo cambiamos el nombre y decimos si es el jugador
			// local. Los datos deberían llegar en el packet de red.

			//Enviamos el mensaje de que se ha creado el jugador
			Net::NetMessageType msg = Net::PLAYER_LOADED;
				Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg));

			LOG("TX PLAYER_LOADED " << id);			

		}	break;	

		case Net::START_GAME:

			LOG("RX START_GAME");
			_app->setState("game"); // Rx mensaje START -> game state
			break;
		}

	} // dataPacketReceived

	//--------------------------------------------------------


} // namespace Application
