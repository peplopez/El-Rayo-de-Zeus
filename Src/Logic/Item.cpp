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
#include "Logic/Maps/EntityFactory.h"
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
	
		_type = _entity->getName().substr(0, _entity->getName().length() - 1); 
			
		return true;		
	} // spawn

	//---------------------------------------------------------

	bool CItem::activate()
	{		
		if( _type == "puPoison") {		
			CMessage *txMsg = new CMessage();	
				txMsg->setType(Message::FX_START);
				txMsg->setAction(Message::FX_SPARKS);
				_entity->emitMessage(txMsg,this);
		}
		return true;
	}

	//---------------------------------------------------------
	bool CItem::accept(const CMessage *message)
	{
		return	message->getType() == Logic::Message::TRIGGER &&
				message->getAction() == Logic::Message::TRIGGER_ENTER;
	} // accept

	//---------------------------------------------------------

	void CItem::process(CMessage *message)
	{
		CMessageUInt* rxMsg = static_cast<CMessageUInt*>(message);
		CEntity* otherEntity = _entity->getMap()
			->getEntityByID( rxMsg->getUInt() );

		// FRS Sólo cogen items los players
		if(otherEntity->getType() == "Player" || otherEntity->getType() == "OtherPlayer") {	
			
			// ATTACH TO HAND
			if( _modelOnHand.length() > 0 ) {
				CMessageString *txMsg = new CMessageString();
					txMsg->setType(TMessageType::ATTACH); 
					txMsg->setAction(TActionType::ATTACH_TO_HAND);
					txMsg->setString(_modelOnHand);
					otherEntity->emitMessage(txMsg);
			}

///////////// HACK TEST FRS Para probar FX -> Esto lo suyo sería hacerlo en cada uno de los hijos de Item			
			if( _type == "puPandora" ) {
				CMessage *txMsg = new CMessage();	
					txMsg->setType(Message::FX_START);
					txMsg->setAction(Message::FX_BLAST_SMALL);
					otherEntity->emitMessage(txMsg,this);
			
			} else if ( _type == "puApple" ) {
				CMessage *txMsg = new CMessage();	
					txMsg->setType(Message::FX_START);
					txMsg->setAction(Message::FX_TRAILS);
					otherEntity->emitMessage(txMsg,this);

			} else if ( _type == "puThrone" ) {
				CMessage *txMsg = new CMessage();	
					txMsg->setType(Message::FX_START);
					txMsg->setAction(Message::FX_FLAME_THROWER);
					otherEntity->emitMessage(txMsg,this);
			}
////////////////////



			// ITEM DEATH
			//FRS Sin CDeath habrá que hacer el deferred delete directamente
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);


			// TODO FRS También habría que notificar, en cada impl. hija de este CItem padre,
			// que se ha cogido el item X o que dicho item causa X efecto sobre el player.
		}

	} // process




} // namespace Logic

