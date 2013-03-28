/**
@file Item.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CItem
@see Logic::IComponent

@author FRS
@date 17-02-2013
*/

#include "Item.h"


#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CItem);
	
	//---------------------------------------------------------

	bool CItem::accept(const CMessage *message) {
		return	message->getType() == Logic::Message::TRIGGER &&
				message->getAction() == Logic::Message::TRIGGER_ENTER;
	} // accept

	//---------------------------------------------------------

	void CItem::process(CMessage *message){

		CMessageUInt* rxMsg = static_cast<CMessageUInt*>(message);
		CEntity* entity = Logic::CServer::getSingletonPtr()->getMap()
			->getEntityByID( rxMsg->getUInt() );

		// FRS Sólo cogen items los players
		if(entity->getType() == "Player" || entity->getType() == "OtherPlayer") {	

			CMessage *txMsg = new CMessage();
				txMsg->setType(Logic::Message::DEAD); // Si alguien nos coge, morimos
				_entity->emitMessage(txMsg, this);

			// TODO FRS También habría que notificar, en cada impl. hija de este CItem padre,
			// que se ha cogido el item X o que dicho item causa X efecto sobre el player.
		}

	} // process


} // namespace Logic

