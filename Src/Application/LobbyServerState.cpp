//---------------------------------------------------------------------------
// LobbyServerState.cpp
//---------------------------------------------------------------------------

/**
@file LobbyServerState.cpp

Contiene la implementación del estado de lobby del servidor.

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
#include "net/Manager.h"
#include "net/Servidor.h"
#include "net/factoriared.h"
#include "net/paquete.h"
#include "net/buffer.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "APP::SERVER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Application {

	CLobbyServerState::CLobbyServerState(CBaseApplication *app) : CApplicationState(app),_waiting(true) 
	{
	} // CLobbyServerState

	CLobbyServerState::~CLobbyServerState() 
	{
	} // ~CLobbyServerState

	//--------------------------------------------------------

	bool CLobbyServerState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("NetLobbyServer.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyServerState::startReleased, this));

		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(false);
		
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyServerState::backReleased, this));

		_nClients = 0; // Nº clientes conectados
		_clients = 0;
		_mapLoadedByClients = 0;		
		memset(_playerNicks,			0,	sizeof(char*) * Net::CLIENTS_MAX);
		memset(_playerModels,			0,	sizeof(char*) * Net::CLIENTS_MAX);
		memset(_playersLoadedByClients, 0,	sizeof(unsigned int) * Net::CLIENTS_MAX);

		return true;

	} // init

	//--------------------------------------------------------

	void CLobbyServerState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CLobbyServerState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

		// NET: Registro como observer server
		Net::CManager::getSingletonPtr()->addObserver(this);
		Net::CManager::getSingletonPtr()->activateAsServer(1234,16); // 16 conexiones

		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")
			->setText("Status: Server up. Waiting for clients ...");

	} // activate

	//--------------------------------------------------------

	void CLobbyServerState::deactivate() 
	{		
		Net::CManager::getSingletonPtr()->removeObserver(this);
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CLobbyServerState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick



	//--------------------------------------------------------

	bool CLobbyServerState::keyPressed(GUI::TKey key)
	{
	   return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CLobbyServerState::keyReleased(GUI::TKey key)
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
	
	bool CLobbyServerState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLobbyServerState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CLobbyServerState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CLobbyServerState::startReleased(const CEGUI::EventArgs& e)
	{
		doStart();
		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CLobbyServerState::backReleased(const CEGUI::EventArgs& e)
	{
		Net::CManager::getSingletonPtr()->deactivateNetwork();
		_app->setState("netmenu");
		return true;

	} // backReleased

	//--------------------------------------------------------

	void CLobbyServerState::doStart()
	{
		_waiting = false;
		// TODO Aquí debemos enviar a los clientes un mensaje de tipo
		// Net::LOAD_MAP para que comiencen la carga del mapa. Tras esto 
		// se debe realizar la carga del blueprints específico del 
		// servidor y el mapa. La carga por defecto para monojugador
		// se ha retrasado y se encuentra en MenuState.cpp			
		Net::NetMessageType txMsg = Net::LOAD_MAP; //Enviamos el mensaje de que carguen el mapa a todos los clientes
			Net::CManager::getSingletonPtr()->send( &txMsg, sizeof(txMsg));

		LOG("TX LOAD_MAP");

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_server.txt")){
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar el nivel");
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();
		}							
		// Cargamos el nivel a partir del nombre del mapa. 
		else if (!Logic::CServer::getSingletonPtr()->loadLevel("map_server.txt")){
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar el nivel");
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();

		} else {

			LOG("MAPA Cargado");
		}
	} // doStart

	//--------------------------------------------------------

	/*******************
		NET: IObserver
	*********************/
	void CLobbyServerState::dataPacketReceived(Net::CPaquete* packet)
	{
		// TODO Aquí es donde debemos recibir los mensajes de red. Hay
		// que atender al mensaje Net::MAP_LOADED para que cuando se
		// reciba, enviar un mensaje tipo Net::START_GAME a los clientes
		// y cambiar al estado "game" de la aplicación
		Net::NetID clientID = packet->getConexion()->getId();

		Net::CBuffer rxSerialMsg; // Packet: "NetMessageType | extraData"
			rxSerialMsg.write(packet->getData(),packet->getDataLength());
			rxSerialMsg.reset();

		Net::NetMessageType rxMsgType;
			rxSerialMsg.read( &rxMsgType, sizeof(rxMsgType) );
		
		switch (rxMsgType)
		{
			case Net::MAP_LOADED: {
			
			LOG("RX MAP_LOADED from " << clientID);
					
			//Almacenamos el ID del usuario que se ha cargado el mapa.
			_mapLoadedByClients |= 1 << clientID;

			// PLAYER INFO: Extraemos info del player que ha cargado el mapa				
			unsigned int nickSize;
				rxSerialMsg.read(&nickSize, sizeof(nickSize)); // Leemos longitud		
			_playerNicks[clientID] = new char[nickSize];		// Reservamos bloque car[] de tamaño size
				rxSerialMsg.read( _playerNicks[clientID], nickSize);		
			unsigned int modelSize;
				rxSerialMsg.read(&modelSize, sizeof(modelSize)); // Leemos longitud		
			_playerModels[clientID] = new char[modelSize];		// Reservamos bloque car[] de tamaño size
				rxSerialMsg.read( _playerModels[clientID], modelSize);	

			//Si todos los clientes han cargado los mapas pasamos a crear jugadores.
			if(_clients == _mapLoadedByClients ) // Lista de clientes (IDs) VS lista de clientes que han cargado el mapa
			{		
				for(int i=0; i < Net::CLIENTS_MAX; ++i)  // Se debe crear un jugador por cada cliente registrado.
				{
					if( ! (_clients & 1 << i ) ) // Si no tenemos cliente número i saltamos. TODO: si NetID se asigna ordenado, quizá habría que hacer break -> quedan libres NetIDs tras disconnect?
						continue;				// TODO todo esto de las máscaras quedará más legible con funciones

					//Inicializamos a 0 la lista de control de carga de jugadores.					
					_playersLoadedByClients[i] = 0;	//Cada cliente ha cargado 0 jugadores

					// [FRS] Hay que enviar un paquete tipo LOAD_PLAYER con 
					// el NetID del cliente del que estamos creando el jugador (*it)
					// Server orquesta carga de cada jugador: "voy a cargar tal, vosotros también"						
					Net::CBuffer txSerialMsg;

						Net::NetMessageType msgType = Net::LOAD_PLAYER;  // Informamos de carga finalizada
							txSerialMsg.write(&msgType, sizeof(msgType));						
						
						Net::NetID playerNetID = i;
							txSerialMsg.write(&playerNetID, sizeof(playerNetID));
						unsigned int nickSize =  nickSize;  // TODO unas funciones de serialización de tipo serán de mucha ayuda
							txSerialMsg.write(&nickSize,sizeof(nickSize));			
							txSerialMsg.write(_playerNicks[i], nickSize);

						unsigned int modelSize = modelSize; 
							txSerialMsg.write(&modelSize,sizeof(modelSize));			
							txSerialMsg.write(_playerModels[i], modelSize);
				
					Net::CManager::getSingletonPtr()->send(txSerialMsg.getbuffer(),	txSerialMsg.getSize() );

					LOG("TX LOAD_PLAYER " << _playerNicks[i] );

					// TODO Llamar al método de creación del jugador. Deberemos decidir
					// si el jugador es el jugador local. Al ser el servidor ninguno lo es
					Logic::CServer::getSingletonPtr()->getMap()->createPlayer(_playerNicks[i], _playerModels[i] , false);
					// HACK Deberíamos poder propocionar caracteríasticas
					// diferentes según el cliente (nombre, modelo, etc.). Esto es una
					// aproximación, solo cambiamos el nombre y decimos si es el jugador local

				}

			} break;
		}


		case Net::PLAYER_LOADED:
		{
			LOG("RX PLAYER_LOADED from " << packet->getConexion()->getId() );
			
			//Aumentamos el número de jugadores cargados por el cliente
			++_playersLoadedByClients[packet->getConexion()->getId()];

			//[FRS] Comprobar si todos los clientes han terminado de cargar todos los jugadores
			bool loadFinished = true;
				for(int i = 0; i<Net::CLIENTS_MAX; ++i)				
					if(_clients & 1 << i)
						if(_playersLoadedByClients[i] < _nClients){ // Nº de jugadores cargados < Nº clientes?
							loadFinished = false;
							break;
						}
		
			if(loadFinished) //Si todos los clientes han cargado todos los players (inc. el suyo)
			{
				//Enviamos el mensaje de que empieza el juego a todos los clientes
				Net::NetMessageType txMsg = Net::START_GAME;
					Net::CManager::getSingletonPtr()->send( &txMsg, sizeof(txMsg));
				_app->setState("game");

				LOG("TX START_GAME");
			}
			break;
		}
		}
	} // dataPacketReceived

	//--------------------------------------------------------

	void CLobbyServerState::connexionPacketReceived(Net::CPaquete* packet)
	{
		LOG("RX CONNECT from " << packet->getConexion()->getId() );

		if(_waiting){
			//Mostramos un poco de información en el status		
			unsigned int ip = packet->getConexion()->getAddress();
				byte* p = (byte*)&ip;

			char id[100];
				sprintf_s(id,"Client conected: %d.%d.%d.%d:%d\nWaiting for more players...",p[0],p[1],p[2],p[3], packet->getConexion()->getPort()); 				
				CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText(id);
		
			//Habilitamos el botón de start.
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(true);

			//Almacenamos el ID del usuario que se ha conectado.
			//UNDONE _clients.push_back(packet->getConexion()->getId());
			_clients |= 1 << packet->getConexion()->getId();
			++_nClients;
		}

	} // connexionPacketReceived

	//--------------------------------------------------------

	void CLobbyServerState::disconnexionPacketReceived(Net::CPaquete* packet)
	{
		Net::NetID clientID = packet->getConexion()->getId();

		// TODO gestionar desconexiones.
		LOG("RX DISCONNECT from " << clientID ); // TODO quedan libres NetIDs tras disconnect? > 8 en algun momento?
	
		//Eliminamos el ID del usuario que se ha desconectado.
		--_nClients;
		_clients &= 0 << clientID;
		_mapLoadedByClients &= 0 << clientID;

		// UNDONE
		/*TNetIDCounterMap::const_iterator pairIt = _playersLoadedByClients.find(packet->getConexion()->getId());
		if(pairIt != _playersLoadedByClients.end())
			_playersLoadedByClients.erase(pairIt);*/
		_playersLoadedByClients[clientID] = 0;
		
		if(_playerNicks[clientID] ) {
			delete[] _playerNicks[clientID];
			 _playerNicks[clientID] = 0;
		}
		if(_playerModels[clientID]){
			delete[] _playerModels[clientID];
			 _playerModels[clientID] = 0;
		}
		
		if(_clients) {
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")
				->setText("Client disconnected. Waiting for new clients...");					
		} else{
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")
				->setText("All clients disconnected. Waiting for some client...");
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(false); //Dehabilitamos el botón de start.
		}
			

	} // disconnexionPacketReceived

} // namespace Application
