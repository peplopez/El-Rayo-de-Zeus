//---------------------------------------------------------------------------
// LobbyServerState.cpp
//---------------------------------------------------------------------------

/**
@file LobbyServerState.cpp

Contiene la implementación del _state de lobby del servidor.

@see Application::CApplicationState
@see Application::CLobbyServerState

@author David Llansó
@date Agosto, 2010
*/

#include "LobbyServerState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include "NET/Buffer.h"
#include "NET/Packet.h"
#include "NET/Serializable.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "APP::LOBBY_SERVER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Application {


	// TODO FRS Los app->exitRequest ya deberían desactivar la red de por sí, hasta que punto es necesario meter redundancias de deactivate networks?

	// DLL Aquí debemos enviar a los clientes un mensaje de tipo
	// Net::LOAD_MAP para que comiencen la carga del mapa. Tras esto 
	// se debe realizar la carga del blueprints específico del 
	// servidor y el mapa. La carga por defecto para monojugador
	// se ha retrasado y se encuentra en MenuState.cpp	

	void CLobbyServerState::_start()
	{
		_waiting = false;

		Net::NetMessageType txMsg = Net::MAP_LOAD; //Enviamos el mensaje de que carguen el mapa a todos los clientes
			_netManager->send( &txMsg, sizeof(txMsg));
		// Tx carga de mapa antes de cargarlo porque idealmente suponemos que nunca fallará la carga
		// Mejor rendimiento cargarlo a la vez en lugar de cargar yo primero y luego el resto

		LOG("TX LOAD_MAP");

		//[ƒ®§] CARGA de Blueprints, Arquetypes y Map adelantada
			
		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_server.txt")){
			_logStatus("Error al cargar el nivel");	
			_app->exitRequest();
		
		// Add - ESC
		// Cargamos el archivo con las definiciones de los archetypes
		} else if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes_server.txt")) {
			_logStatus("Error al cargar archetypes");	
			_app->exitRequest();
			
		// Add - JLS
		// Cargamos los anillos a partir del nombre del mapa. 
		} else if (!Logic::CServer::getSingletonPtr()->setRingPositions()) {//[ƒ®§] Esto no deberia ejecutarse como parte del loadLevel...?
			_app->exitRequest();
		
		// TODO hay que cargar un mapa genérico y luego colorear por jugador
		// Cargamos el nivel a partir del nombre del mapa. 
		} else if (!Logic::CServer::getSingletonPtr()->loadMap("mapRed")){
			_logStatus("Error al cargar el nivel");		
			_app->exitRequest();

		} else {
			LOG("MAPA Cargado");
		}
	} // start



	/*******************
		NET: IObserver
	*********************/

	// DLL Aquí es donde debemos recibir los mensajes de red. Hay
	// que atender al mensaje Net::MAP_LOADED para que cuando se
	// reciba, enviar un mensaje tipo Net::START_GAME a los clientes
	// y cambiar al estado "game" de la aplicación
	void CLobbyServerState::dataPacketReceived(Net::CPacket* packet)
	{		
		Net::NetID clientID = packet->getConnection()->getId();

		Net::CBuffer rxSerialMsg( packet->getData(), packet->getDataLength() ); // Packet: "NetMessageType | extraData"
			rxSerialMsg.write( packet->getData(), packet->getDataLength() );
			rxSerialMsg.reset();

		Net::NetMessageType rxMsgType;
			rxSerialMsg.read( &rxMsgType, sizeof(rxMsgType) );
		
		switch (rxMsgType)
		{
			case Net::MAP_LOADED: {
					
			//Almacenamos el ID del usuario que se ha cargado el mapa.
			_mapLoadedByClients |= 1 << clientID;

			// PLAYER INFO: Extraemos info del player que ha cargado el mapa
			_playerNicks[clientID].assign(	Net::Serializable::deserializeString(rxSerialMsg) );
			_playerModels[clientID].assign( Net::Serializable::deserializeString(rxSerialMsg) );	

			LOG( "RX MAP_LOADED from " << clientID << " with Nick=" << _playerNicks[clientID] << " and Model=" << _playerModels[clientID] );

			//Si todos los clientes han cargado los mapas pasamos a crear jugadores.
			if(_maskClientIds == _mapLoadedByClients ) // Lista de clientes (IDs) VS lista de clientes que han cargado el mapa
			{		
				for(int id=0; id < Net::CLIENTS_MAX; ++id)  // Se debe crear un jugador por cada cliente registrado.
				{
					if( ! (_maskClientIds & 1 << id ) ) // Si no tenemos cliente número i saltamos. TODO: si NetID se asigna ordenado, quizá habría que hacer break -> quedan libres NetIDs tras disconnect?
						continue;				// TODO todo esto de las máscaras quedará más legible con funciones

					LOG("Load Players Loop: ClientID=" << id);

					//Inicializamos a 0 la lista de control de carga de jugadores.					
					_playersLoadedByClients[id] = 0;	//Cada cliente ha cargado 0 jugadores

					// [FRS] Hay que enviar un packet tipo LOAD_PLAYER con 
					// el NetID del cliente del que estamos creando el jugador (*it)
					// Server orquesta carga de cada jugador: "voy a cargar tal, vosotros también"						
					Net::CBuffer txSerialMsg;
						Net::NetMessageType msgType = Net::PLAYER_LOAD;  // Informamos de carga finalizada
							txSerialMsg.write(&msgType, sizeof(msgType));	
						Net::NetID playerNetID = id;
							txSerialMsg.write(&playerNetID, sizeof(playerNetID));						
						Net::Serializable::serialize(txSerialMsg, _playerNicks[id]  );
						Net::Serializable::serialize(txSerialMsg, _playerModels[id] );
					_netManager->send(txSerialMsg.getbuffer(),	txSerialMsg.getSize() );

					LOG("TX LOAD_PLAYER " << id << " with Nick=" << _playerNicks[id] << " and Model=" << _playerModels[id] );

					// [FRS] Llamar al método de creación del jugador. Deberemos decidir
					// si el jugador es el jugador local. Al ser el servidor ninguno lo es
			//		Logic::CServer::getSingletonPtr()->getMap("mapPlayer")->createPlayer(_playerNicks[id], false, _playerModels[id]);
					// HACK Deberíamos poder propocionar caracteríasticas
					// diferentes según el cliente (nombre, modelo, etc.). Esto es una
					// aproximación, solo cambiamos el nombre y decimos si es el jugador local

				}

			} break;
		}

// TODO Llevamos la cuenta de los que ha cargado cada uno
// Así, si tarda mucho en responder o manda un disconnect -> descartamos solo su recuento
		case Net::PLAYER_LOADED:
		{
			LOG("RX PLAYER_LOADED from " << packet->getConnection()->getId() );
			
			//Aumentamos el número de jugadores cargados por el cliente
			++_playersLoadedByClients[packet->getConnection()->getId()];

			//[FRS] Comprobar si todos los clientes han terminado de cargar todos los jugadores
			// TODO Sería mejor gestionar exactamente que player ha cargado cada uno, por si hay desconnectiones
			bool loadFinished = true;
				for(int i = 0; i<Net::CLIENTS_MAX; ++i)				
					if(_maskClientIds & 1 << i)
						if(_playersLoadedByClients[i] < _nClients){ // Nº de jugadores cargados < Nº clientes?
							loadFinished = false;
							break;
						}

		
			if(loadFinished) //Si todos los clientes han cargado todos los players (inc. el suyo)
			{
				//Enviamos el mensaje de que empieza el juego a todos los clientes
				Net::NetMessageType txMsg = Net::GAME_START;
					_netManager->send( &txMsg, sizeof(txMsg));
				_app->setState("game");

				LOG("TX START_GAME");
			}
			break;
		}
		}
	} // dataPacketReceived



	//--------------- CONNECT -----------------------------------

	void CLobbyServerState::connectPacketReceived(Net::CPacket* packet)
	{
		LOG("RX CONNECT from " << packet->getConnection()->getId() );

		if(!_waiting)
			return;

		//Mostramos un poco de información en el status		
		unsigned int ip = packet->getConnection()->getAddress();
			Net::byte* p = (Net::byte*) &ip;

		char welcomeText[100];
			sprintf_s(welcomeText,"Client conected: %d.%d.%d.%d:%d\nWaiting for more players...", 
				p[0],p[1],p[2],p[3], packet->getConnection()->getPort() ); 				
			_windowStatus->appendText(welcomeText);
		
		// > 1 Cliente? Habilitamos el botón de start.
		_windowStart->setEnabled(true);

		//Almacenamos el ID del usuario que se ha conectado.
		//UNDONE _maskClientIds.push_back(packet->getConnection()->getId());
		_maskClientIds |= 1 << packet->getConnection()->getId();

	} // connectPacketReceived


	//----------------- DISCONNECT ---------------------------------------

	// TODO FRS Gestionar desconnectiones y borrarles del recuento de mapas / jugadores cargados... 
	void CLobbyServerState::disconnectPacketReceived(Net::CPacket* packet)
	{
		Net::NetID clientID = packet->getConnection()->getId();

		// TODO gestionar desconnectiones.
		LOG("RX DISCONNECT from " << clientID ); // TODO quedan libres NetIDs tras disconnect? > 8 en algun momento?
	
		//Eliminamos el ID del usuario que se ha desconectado.
		--_nClients;
		_maskClientIds &= 0 << clientID;
		_mapLoadedByClients &= 0 << clientID;

		// UNDONE
		/*TNetIDCounterMap::const_iterator pairIt = _playersLoadedByClients.find(packet->getConnection()->getId());
		if(pairIt != _playersLoadedByClients.end())
			_playersLoadedByClients.erase(pairIt);*/
		_playersLoadedByClients[clientID] = 0;
		_playerNicks[clientID] = "";
		_playerModels[clientID]	= "";	
		
		if(_maskClientIds) {
			_logStatus("Client disconnected. Waiting for new clients...");					
		} else{
			_logStatus("All clients disconnected. Waiting for some client...");
			_windowStart->setEnabled(false); //Dehabilitamos el botón de start.
		}
			

	} // disconnectPacketReceived





	/**********************
		CApplicationState
	************************/

	// FRS En el init ya se han inicializado todos los motores de la app (CEGUI, NET, etc...)
	bool CLobbyServerState::init() 
	{
	// CEGUI

		// Cargamos la ventana que muestra el menú
		_windowManager = CEGUI::WindowManager::getSingletonPtr();
			_windowManager->loadWindowLayout("NetLobbyServer.layout");		
			_windowMenu =	_windowManager->getWindow("NetLobbyServer");
			_windowStatus = _windowManager->getWindow("NetLobbyServer/Status");			
			_windowBack =	_windowManager->getWindow("NetLobbyServer/Back");
			_windowStart = _windowManager->getWindow("NetLobbyServer/Start");
				_windowStart->setEnabled(false);

		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_windowStart->subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CLobbyServerState::_startReleased, this));		
		_windowBack->subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CLobbyServerState::_backReleased, this));
		
	// NET
		_netManager = Net::CManager::getSingletonPtr();

		return true;
	} // init


	//--------------------------------------------------------

	void CLobbyServerState::activate() 
	{		
		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->setGUISheet(_windowMenu);
			_windowMenu->setVisible(true);
			_windowMenu->activate();

		CEGUI::MouseCursor::getSingleton().show();

		// NET: Registro como ob-server (chistaco)
		_netManager->addObserver(this);
		_netManager->activateAsServer(); 

		_logStatus("Status: Server up. Waiting for clients ...");

	} // activate

	//--------------------------------------------------------

	void CLobbyServerState::deactivate() 
	{		
		_netManager->removeObserver(this);
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_windowMenu->deactivate();
		_windowMenu->setVisible(false);
	} // deactivate






	/*********************************
		GUI::CKeyboardListener
	********************************/

	bool CLobbyServerState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_netManager->deactivateNetwork();
			_app->setState("netmenu");
			break;
		case GUI::Key::RETURN:
			_start();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	


			
	

	/**************************
		CEGUI::WindowManager
	****************************/
		
	bool CLobbyServerState::_startReleased(const CEGUI::EventArgs& e)
	{
		_start();
		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CLobbyServerState::_backReleased(const CEGUI::EventArgs& e)
	{
		_netManager->deactivateNetwork();
		_app->setState("netmenu");
		return true;

	} // backReleased

	//--------------------------------------------------------

	void CLobbyServerState::_logStatus(const std::string& statusMsg) 
	{ 
		assert(_windowStatus && "Status window is NULL!");
		_windowStatus->appendText(statusMsg);
		_windowStatus->appendText("\n");
	}

} // namespace Application
