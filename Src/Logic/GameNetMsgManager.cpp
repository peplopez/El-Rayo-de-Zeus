//---------------------------------------------------------------------------
// GameNetMsgManager.cpp
//---------------------------------------------------------------------------

/**
@file GameNetMsgManager.cpp

Contiene la implementación del gestor de los mensajes de red durante la partida.

@see Logic::Manager::IObserver
@see Logic::CGameNetMsgManager

@author David Llansó
@date Febrero, 2011
*/

#include "GameNetMsgManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"

#include "Net/paquete.h"
#include "Net/buffer.h"

#include "Application/BaseApplication.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::NET_MANAGER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic {

	CGameNetMsgManager* CGameNetMsgManager::_instance = 0;

	//--------------------------------------------------------

	CGameNetMsgManager::CGameNetMsgManager()
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CGameNetMsgManager::~CGameNetMsgManager()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CGameNetMsgManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CGameNetMsgManager no permitida!");

		new CGameNetMsgManager();

		return true;

	} // Init

	//--------------------------------------------------------

	void CGameNetMsgManager::Release()
	{
		assert(_instance && "Logic::CGameNetMsgManager no está inicializado!");

		if(_instance)
			delete _instance;

	} // Release

	//--------------------------------------------------------

	void CGameNetMsgManager::activate() 
	{
		// TODO Escuchamos los mensajes de red. Engancharnos a Net::CManager
		Net::CManager::getSingletonPtr()->addObserver(this);
	} // activate

	//--------------------------------------------------------

	void CGameNetMsgManager::deactivate() 
	{	
		// TODO Dejamos de escuchar los mensajes de red. 
		// Desengancharnos de Net::CManager
		Net::CManager::getSingletonPtr()->removeObserver(this);
	} // deactivate

	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendEntityMessage(const TMessage &txMsg, TEntityID destID)
	{
		// [f®§] Serializar el mensaje lógico. Para la serialización
		// apoyarse en Net::CBuffer. Se debe guardar primero el tipo
		// de mensaje de red (Net::ENTITY_MSG) y luego el id de la
		// entidad y el mensaje serializado. La función de serialización
		// de un mensaje lógico está en Message. Al final se debe hacer
		// el envío usando el gestor de red.
		// Es un mensaje para enviar por el tubo.
		// Lo serializamos y enviamos por la red...

		Net::NetMessageType msgType = Net::NetMessageType::ENTITY_MSG;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;		
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID)); // Escribimos el id de la entidad destino
			Message::Serialize(txMsg, serialMsg);   // Serializamos el mensaje a continuación en el buffer
		Net::CManager::getSingletonPtr()->send(serialMsg.getbuffer(), serialMsg.getSize());

		LOG("TX ENTITY_MSG " << txMsg._type << " to EntityID " << destID);

	} // sendEntityMessage

	//---------------------------------------------------------
		
	void CGameNetMsgManager::processEntityMessage(Net::CPaquete* packet)
	{
		// TODO Método que debe de ser invocado desde el método que
		// recibe todos los paquetes de red cuando el tipo de mensaje
		// de red es Net::ENTITY_MSG. Se debe sacar el ID de la entidad,
		// recuperarla, deserializar el mensaje y enviárselo

		// Creamos un buffer con los datos para leer de manera más cómoda
		Net::CBuffer serialMsg;
			serialMsg.write(packet->getData(),packet->getDataLength());
			serialMsg.reset(); // Reiniciamos el puntero de lectura a la posición 0

		// Extraemos, pero ignoramos el tipo de mensaje de red. Ya lo hemos procesado		
		Net::NetMessageType msgType;
			serialMsg.read(&msgType,sizeof(msgType));
		TEntityID destID; // Escribimos el id de la entidad
			serialMsg.read(&destID, sizeof(destID));
		TMessage rxMsg;
			Message::Deserialize(serialMsg, rxMsg);

		// PROPAGACIÓN INMEDIATA: Msgs que deben propagarse directamente desde 1 cliente al resto (p.e. CONTROL)
		if(rxMsg._type == TMessageType::CONTROL) // HACK: Si hay más mensajes de propagación inmediata -> config en map.txt
			Net::CManager::getSingletonPtr()->send(serialMsg.getbuffer(), serialMsg.getSize(), packet->getConexion() );

		// RTX DESERIALIZADO
		CEntity* destEntity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(destID);
			if(destEntity != 0)
				destEntity->emitMessage(rxMsg);
		
		if(packet->getConexion())
			LOG("RX ENTITY_MSG " << rxMsg._type << " from NetID " << packet->getConexion()->getId() << " for EntityID " << destID);
	} // processEntityMessage


	/*******************
		NET: IObserver
	*********************/
	// Aquí es donde debemos recibir los mensajes de red
	void CGameNetMsgManager::dataPacketReceived(Net::CPaquete* packet)
	{
		Net::CBuffer rxSerialMsg; // Packet: "NetMessageType | extraData"
			rxSerialMsg.write(packet->getData(),packet->getDataLength());
			rxSerialMsg.reset();

		Net::NetMessageType rxMsgType;
			rxSerialMsg.read( &rxMsgType, sizeof(rxMsgType) );			
		switch (rxMsgType)
		{

		case Net::NetMessageType::ENTITY_MSG:	
			processEntityMessage(packet);
			break;	

		case Net::NetMessageType::END_GAME:	

			TEntityID entityID; 
				rxSerialMsg.read(&entityID, sizeof(entityID) );  //	Packet: "NetMessageType | extraData(NetID)"	
			
			CEntity* player = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(entityID);
				if( player->isPlayer() )						// GameOver si el END_GAME es para nuestro player
					Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
				else											// Si no, eliminamos ese player del mapa
					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(player);

			break;
		}
	} // dataPacketReceived

	//--------------------------------------------------------

} // namespace Logic
