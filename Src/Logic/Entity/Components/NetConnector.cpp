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

#include "Logic/Entity/Message.h"

#include "Net/Manager.h"
#include "Logic/GameNetMsgManager.h"
#include "Net/buffer.h"

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

		return true;
	}

	bool CNetConnector::accept(const TMessage &message)
	{
		// TODO Vemos si es uno de los mensajes que debemos trasmitir 
		// por red. Para eso usamos la lista de mensajes que se ha
		// leido del mapa.
		std::vector<TMessageType>::const_iterator it; 	
		for(it = _forwardedMsgTypes.begin(); it != _forwardedMsgTypes.end(); ++it)
			if( *it == message._type)
				return true;	

		return false;

	} // accept
		
	//---------------------------------------------------------------------------------

	void CNetConnector::process(const TMessage &message)
	{
		// TODO Es un mensaje para enviar por el tubo.
		// Lo enviamos por la red usando el front-end CGameNetMsgManager
		CGameNetMsgManager::getSingletonPtr()->sendEntityMessage(message, _entity->getEntityID());

	} // process
		
	//---------------------------------------------------------------------------------
		
	void CNetConnector::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

} // namespace Logic
