/**
@file NetConnector.h

Contiene la implementación del componente que reenvia mensajes por la red.

@see Logic::CNet
@see Logic::IComponent

@author David Llansó
@date Diciembre, 2010
*/

#include "NetConnector.h"
#include <cassert>
#include <sstream>

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"



#include "Net/Manager.h"
#include "Logic/GameNetMsgManager.h"
#include "Net/buffer.h"

#include "Logic/Entity/Messages/Message.h"

#include <iostream>

#define BS_STACK_ALLOC_SIZE 128

namespace Logic {
		
	IMP_FACTORY(CNetConnector);

	bool CNetConnector::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)  {

		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		// NET: Procesamos la lista con los nombres de los mensajes...
		if (entityInfo->hasAttribute("msgList")) 
		{
			std::istringstream mgsTypeList(entityInfo->getStringAttribute("msgList"));

			// Para cada cadena entre comas...
			do {
				std::string MsgTypeName;
				std::getline(mgsTypeList, MsgTypeName, ','); // linea entre delimitadores
				
				std::istringstream str(MsgTypeName);     // wrappeamos cadena como Input Stream
				do {									// Le quitamos los espacios...
					std::getline(str, MsgTypeName, ' ');  // linea entre espacios
				} while (MsgTypeName.size() == 0 && !str.eof());

				
				// ... y registramos el tipo en la lista
				_forwardedMsgTypes.push_back((Logic::TMessageType)atoi(MsgTypeName.c_str())); // char[] -> int -> TMessageType

			} while (!mgsTypeList.eof());
		}

		// Obtenemos los milisegundos que se esperan entre envios de mensajes del mismo tipo
		if (entityInfo->hasAttribute("blockedTime")) 
			_timeOfBlocking = entityInfo->getIntAttribute("blockedTime");

		return true;
	}

	bool CNetConnector::accept(CMessage *message)
	{
		if(message->getType() == Message::CONTROL && !_entity->isPlayer() )  // CONTROL: sólo TX por red si lo ha generado el Player local (PlayerController)
			return false;

		// [ƒ®§] Vemos si es uno de los mensajes que debemos trasmitir 
		// por red. Para eso usamos la lista de mensajes que se ha leido del mapa.
		// Vemos si es uno de los mensajes que debemos trasmitir  por red.
		else if (std::find(_forwardedMsgTypes.begin(),  _forwardedMsgTypes.end(), message->getType()) != _forwardedMsgTypes.end())
		{			
			// Grano fino, en vez de aceptar el mensaje directamente
			// solo se retransmitirá por la red si no se ha transmitido 
			// hace poco (_timeOfBlocking milisegundos) un mensaje del mismo tipo
			if(_timeToUnblockMsgDelivery.count(message->getType()) == 0) // TODO probar sin ajuste fino qué tal va de fino :P
			{
				if(_timeOfBlocking)
					_timeToUnblockMsgDelivery.insert(TTimeToUnblockMsgDeliveryPair(message->getType(),_timeOfBlocking));
				return true;
			}
		}

		return false;

	} // accept
		
	//---------------------------------------------------------------------------------

	void CNetConnector::process(CMessage *message)
	{
		// TODO Es un mensaje para enviar por el tubo.
		// Lo enviamos por la red usando el front-end CGameNetMsgManager
		CGameNetMsgManager::getSingletonPtr()->
			sendEntityMessage(message,_entity->getEntityID());

	} // process
		
	//---------------------------------------------------------------------------------
		
	void CNetConnector::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		
		// Grano fino:
		// Solo permitimos envíos del mismo tipo de mensaje cada x tiempo.
		// Aquí actualizamos el tiempo que falta para poder enviar un nuevo
		// mensaje de un tipo espécifico.
		TTimeToUnblockMsgDelivery::iterator it =
			_timeToUnblockMsgDelivery.begin();
		while(it != _timeToUnblockMsgDelivery.end()) // Recorremos pares
		{
			(*it).second -= msecs;	//Descontamos tiempo a _timeToUnblock (second)
			if ((*it).second <= 0)
				it = _timeToUnblockMsgDelivery.erase(it); // Eliminamos par de bloqueo y saltamos al siguiente
			else
				it++;	// Si no se elimina par, incremetnamos para pasar a siguiente
		}
	}

} // namespace Logic
