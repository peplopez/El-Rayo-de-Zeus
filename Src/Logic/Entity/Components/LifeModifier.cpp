/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLifeModifier
@see Logic::IComponent

@author FRS
@date 17-02-2013
*/

#include "LifeModifier.h"

#include "BaseSubsystems/Math.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Map/Entity.h"


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
		CEntity* entity = _entity->getMap()->getEntityByID( rxMsg->getUInt() );

		CMessageUInt *txMsg = new CMessageUInt();

		//LifeModifier manda un mensaje al componente LIFE.cpp 
		// mensaje TIPO LIFE_MODIFIER. Action = DAMAGE, HEAL
		
		txMsg->setType(Message::LIFE_MODIFIER);
			
		//PT. se carga en el entero el modificador de vida (negativo o positivo)
		//txMsg->setUInt( abs(_LIFE_MODIFIER) );
		txMsg->setUInt(_LIFE_MODIFIER );

		if (_LIFE_MODIFIER < 0)
			txMsg->setAction(Message::DAMAGE);
		else if(_LIFE_MODIFIER > 0)
			txMsg->setAction(Message::HEAL);

		//PT. It just worth sending message when it hurts or heals, when is 0 doesnt worthy
		if(_LIFE_MODIFIER != 0)
			entity->emitMessage(txMsg, this);

	} // process

} // namespace Logic

