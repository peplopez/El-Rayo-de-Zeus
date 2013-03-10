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

#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CLifeModifier);
	
	//---------------------------------------------------------

	bool CLifeModifier::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("hpModifier"))
			_HP_MODIFIER = entityInfo->getIntAttribute("hpModifier");
		
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLifeModifier::activate() {
		// TODO registrarse como observer fisico
		return IComponent::activate();
	}

	//---------------------------------------------------------

	void CLifeModifier::deactivate() {
		// release observer fisico
		_active = false;
	}

	//---------------------------------------------------------

	void CLifeModifier::onTrigger() {
		CMessageInt *txMsg = new CMessageInt();
			txMsg->setInt(_HP_MODIFIER);
			txMsg->setType(TMessageType::LIFE_MODIFIER);
			_entity->emitMessage(txMsg, this);
	}
} // namespace Logic

