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


#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "APP::LOBBY_CLIENT>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Application {
	
	void CLobbyClientState::_connect()
	{	
		_logStatus("Connecting...");

		// Obtenemos la ip desde el Editbox
		CEGUI::String ip = _windowManager->getWindow("NetLobbyClient/IPBox")->getText();
		
		// NET: Conectamos
		if( _netManager->clientConnectToServer( (char*)ip.c_str(), Net::SERVER_PORT, 1) ) {
			_logStatus("Connected to server. Waiting to start game...");// Actualizamos el status	
			_windowConnect->setEnabled(false); // Deshabilitamos el botón de Start
		} else {
			_logStatus("Server not responding or full. Re-check IP.");
		}

		//Net tick is in charge of receiving the message and change the state
	} // doStart



	/**********************
		NET: IOBSERVER
	********************/

	void CLobbyClientState::dataPacketReceived(Net::CPacket* packet)
	{
		// Packet: "NetMessageType | extraData"
		Net::NetMessageType rxMsgType;
		Net::CBuffer rxSerialMsg( packet->getData(), packet->getDataLength() );	
			rxSerialMsg.read( &rxMsgType, sizeof(rxMsgType) );

		switch (rxMsgType)
		{

		// TODO Gestionar la carga del blueprints cliente y del mapa cuando  
		// se reciba un mensaje de red de tipo LOAD_MAP. Cuando se finalice
		// la carga avisar al servidor enviando un mensaje tipo MAP_LOADED
		case Net::MAP_LOAD:

			LOG("RX LOAD_MAP");

			//PT Carga de mapRed en la lista de mapas
			_mapsToLoad.push_back("mapRed");

			//[ƒ®§] CARGA de Blueprints, Arquetypes y Map adelantada

			// Cargamos el archivo con las definiciones de las entidades del nivel.
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_client.txt")){
				_logStatus("Error al cargar el nivel");
				_netManager->deactivateNetwork();
				_app->exitRequest();
		
			// Add - ESC
			// Cargamos el archivo con las definiciones de los archetypes
			} else if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes_client.txt")) {
				_logStatus("Error al cargar archetypes");
				_netManager->deactivateNetwork();
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
				_logStatus("Error al cargar el nivel");
				_netManager->deactivateNetwork();
				_app->exitRequest();

			} else { //Avisamos de que hemos terminado la carga y tx info de player
				
				LOG("MAPA Cargado"); 	

				// OBTENER PLAYER INFO
				std::string playerNick = std::string( _windowManager->getWindow("NetLobbyClient/NickBox")->getText().c_str() );
				//// TODO obtener modelo mediante ddList -> tabla mesh			
				//"loco.mesh", "marine.mesh", "AttaObrera.mesh", "bioshock.mesh","AttaSoldada.mesh", "aranna.mesh"
				//std::string playerModel = std::string( _windowManager->getWindow("NetLobbyClient/ModelBox")->getText().c_str() );
				std::string playerModel = "";
				//COLOR
				std::string playerColor = std::string(_windowManager->getWindow("NetLobbyClient/ColorBox")->getText().c_str());

				//int playerModelID = (int)_windowManager->getWindow("NetLobbyClient/ModelBox")->getID();
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
				_netManager->send(txSerialMsg.getbuffer(),	txSerialMsg.getSize() );
				
				LOG("TX MAP_LOADED with Nick=" << playerNick << " and Model=" << playerModel << " and color = " << playerColor );
			}

			break;

		// CARGAR PLAYER
		case Net::PLAYER_LOAD: { 	 //	Packet: "NetMessageType | netID | playerInfo"		

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
			bool isLocalPlayer = id == _netManager->getID(); // id rx == id local?

			Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer(playerNick, isLocalPlayer, playerModel);
			// HACK Deberíamos poder propocionar caracteríasticas
			// diferentes según el cliente (nombre, modelo, etc.). Esto es una
			// aproximación, solo cambiamos el nombre y decimos si es el jugador
			// local. Los datos deberían llegar en el packet de red.

			//Enviamos el mensaje de que se ha creado el jugador
			Net::NetMessageType msg = Net::PLAYER_LOADED;
				_netManager->send(&msg, sizeof(msg));

			LOG("TX PLAYER_LOADED " << id);			

		}	break;	

		case Net::GAME_START:

			LOG("RX START_GAME");
			_app->setState("game"); // Rx mensaje START -> game state
			break;
		}

	} // dataPacketReceived

	



	/*************************
		CApplicationState
	**************************/

	bool CLobbyClientState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú con LUA
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("NetLobbyClient");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initNetLobbyClient");

		_windowManager = CEGUI::WindowManager::getSingletonPtr();
		
		_windowStatus = _windowManager->getWindow("NetLobbyClient/Status");

		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_windowConnect = _windowManager->getWindow("NetLobbyClient/Connect");
			_windowConnect->subscribeEvent(CEGUI::PushButton::EventClicked, 
							CEGUI::SubscriberSlot(&CLobbyClientState::_connectReleased, this));
		
		_windowBack = _windowManager->getWindow("NetLobbyClient/Back");
			_windowBack->subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot( &CLobbyClientState::_backReleased, this ) );

		//Modelo .mesh o personaje
		_cbModel = static_cast<CEGUI::Combobox*>(_windowManager->getWindow("NetLobbyClient/ModelBox"));			
			_cbModel->addItem(	new CEGUI::ListboxTextItem("Spartan") );// add items to the combobox list
			_cbModel->addItem(	new CEGUI::ListboxTextItem("Atalanta") );  // FRS AutoDelete = true por defecto (no hace falta delete)
			_cbModel->addItem(	new CEGUI::ListboxTextItem("Marine") );
			_cbModel->addItem(	new CEGUI::ListboxTextItem("Madman") );			
			_cbModel->setReadOnly(true);
			
		

		//Color
		_cbColor = static_cast<CEGUI::Combobox*>(_windowManager->getWindow("NetLobbyClient/ColorBox"));
			_cbColor->addItem(	new CEGUI::ListboxTextItem("Red")	);
			_cbColor->addItem(	new CEGUI::ListboxTextItem("Green")	);
			_cbColor->addItem(	new CEGUI::ListboxTextItem("Blue")	);
			_cbColor->addItem(	new CEGUI::ListboxTextItem("Yellow")	);
			_cbColor->setReadOnly(true);
			_cbColor->removeItem( _cbColor->getListboxItemFromIndex(2) ); // FRS el disable no surte efecto

		_netManager = Net::CManager::getSingletonPtr();
	
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

		// Activamos la ventana que nos muestra el menú y activamos el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showNetLobbyClient");

		// NET
		_netManager->addObserver(this); // Se registra como IObserver
		_netManager->activateAsClient(); //  Activar la red como tipo Client

	} // activate

	//--------------------------------------------------------

	void CLobbyClientState::deactivate() 
	{	
		_netManager->removeObserver(this);
		
		// Desactivamos la ventana que nos muestra el menú y desactivamos el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideNetLobbyClient");

		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CLobbyClientState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick



	
	/*********************************
		GUI::CKeyboardListener
	********************************/

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
			_netManager->deactivateNetwork();
			_app->setState("netmenu");
			break;
		case GUI::Key::RETURN:
			_connect();
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

	

	/*********************************
		GUI::CMouseListener
	********************************/

	bool CLobbyClientState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLobbyClientState::mousePressed(const GUI::CMouseState &mouseState)
	{	
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CLobbyClientState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	

	/**************************
		CEGUI::WindowManager
	****************************/
		
	bool CLobbyClientState::_connectReleased(const CEGUI::EventArgs& e)
	{
		_connect();
		return true;
	} // startReleased
			
	//--------------------------------------------------------

	bool CLobbyClientState::_backReleased(const CEGUI::EventArgs& e)
	{
		_netManager->deactivateNetwork(); // NET: deactivate
		_app->setState("netmenu");
		return true;

	} // backReleased
			
	//--------------------------------------------------------

	void CLobbyClientState::_logStatus(const std::string& statusMsg) 
	{ 
		assert(_windowStatus && "Status window is NULL!");
		_windowStatus->appendText(statusMsg);
		_windowStatus->appendText("\n");
	}


} // namespace Application
