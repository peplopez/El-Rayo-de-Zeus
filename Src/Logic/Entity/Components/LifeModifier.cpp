/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLifeModifier
@see Logic::IComponent

@author FRS
@date 17-02-2013
*/

#include "LifeModifier.h"


#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/MessageInt.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CLifeModifier);
	
	//---------------------------------------------------------

	bool CLifeModifier::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("lifeModifier"))
			_LIFE_MODIFIER = entityInfo->getIntAttribute("lifeModifier");
		
		return true;

	} // spawn

	//---------------------------------------------------------
	
	bool CLifeModifier::accept(const CMessage *message) {
		return	message->getType() == Logic::Message::TRIGGER &&
				message->getAction() == Logic::Message::TRIGGER_ENTER;
	} // accept

	//---------------------------------------------------------

	void CLifeModifier::process(CMessage *message){

		CMessageUInt* rxMsg = static_cast<CMessageUInt*>(message);
		CEntity* entity = Logic::CServer::getSingletonPtr()->getMap()
			->getEntityByID( rxMsg->getUInt() );

		CMessageInt *txMsg = new CMessageInt();
			txMsg->setInt(_LIFE_MODIFIER);
			if (_LIFE_MODIFIER<0)
				txMsg->setAction(Message::DAMAGE);
			else
				txMsg->setAction(Message::HEAL);			
			txMsg->setType(Logic::Message::LIFE_MODIFIER);
				entity->emitMessage(txMsg, this);
	} // process

} // namespace Logic

