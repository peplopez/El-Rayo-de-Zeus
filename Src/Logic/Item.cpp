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
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CItem);
	
	//---------------------------------------------------------

	bool CItem::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{		
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if( entityInfo->hasAttribute("modelOnHand") )
			_modelOnHand = entityInfo->getStringAttribute("modelOnHand");

		return true;		
	} // spawn


	//---------------------------------------------------------
	bool CItem::accept(const CMessage *message) {
		return	message->getType() == Logic::Message::TRIGGER &&
				message->getAction() == Logic::Message::TRIGGER_ENTER;
	} // accept

	//---------------------------------------------------------

	void CItem::process(CMessage *message){

		CMessageUInt* rxMsg = static_cast<CMessageUInt*>(message);
		CEntity* otherEntity = Logic::CServer::getSingletonPtr()->getMap()
			->getEntityByID( rxMsg->getUInt() );

		// FRS Sólo cogen items los players
		if(otherEntity->getType() == "Player" || otherEntity->getType() == "OtherPlayer") {	

			// ITEM DEATH
			CMessage *txMsg1 = new CMessage();
				txMsg1->setType(TMessageType::DEAD); // Si alguien nos coge, morimos
				_entity->emitMessage(txMsg1, this);
// TODO FRS Sin CDeath habrá que hacer el deferred delete directamente
			// GET OBJECT ANIM
			CMessageBoolString *txMsg2 = new CMessageBoolString();
				txMsg2->setType(TMessageType::SET_ANIMATION); 
				txMsg2->setBool(false);
				txMsg2->setString("GetObject");
				otherEntity->emitMessage(txMsg2); // TODO FRS falta desactivar INPUT => migrar esto a FSM de animaciones

			// ATTACH TO HAND
			if( _modelOnHand.length() > 0 ) {
				CMessageString *txMsg3 = new CMessageString();
					txMsg3->setType(TMessageType::ATTACH); 
					txMsg3->setAction(TActionType::ATTACH_TO_HAND);
					txMsg3->setString(_modelOnHand);
					otherEntity->emitMessage(txMsg3);
			}


			// TODO FRS También habría que notificar, en cada impl. hija de este CItem padre,
			// que se ha cogido el item X o que dicho item causa X efecto sobre el player.
		}

	} // process


} // namespace Logic

