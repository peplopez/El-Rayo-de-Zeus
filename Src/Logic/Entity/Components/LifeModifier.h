/**
@file LifeModifier.h

Contiene la declaración del componente que genera modificadores de vida hacia otras entidades.

@see Logic::CLifeModifier
@see Logic::IComponent

@author FRS
@date 17-02-2013
*/
#ifndef __Logic_LifeModifier_H
#define __Logic_LifeModifier_H

#include "Logic/Entity/Component.h"


namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{
/**
	Genera modificadores de vida hacia otras entidades.
	
    @ingroup logicGroup

	@author FRS
	@date 17-02-2013
*/
	class CLifeModifier : public IComponent //, IPhysics
	{
		DEC_FACTORY(CLifeModifier);
	public:

		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		void deactivate();

		/**********************
			PHYSICS::IOBSERVER
		**********************/
		void onTrigger();


	protected:
		
		/**
		Modificador de vida
		*/
		int _HP_MODIFIER;

	}; // class CLifeModifier

	REG_FACTORY(CLifeModifier);

} // namespace Logic

#endif // __Logic_LifeModifier_H
