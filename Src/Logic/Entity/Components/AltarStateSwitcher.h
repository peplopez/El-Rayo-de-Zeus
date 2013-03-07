/**
@file AltarStateSwitcher.h

Contiene la declaración del componente que controla la
capacidad de un Character de activar/desactivar altares

@see Logic::CAltarStateSwitcher


@author Emilio Santalla
@date Marzo, 2013
*/

#ifndef __Logic_AltarStateSwitcher_H
#define __Logic_AltarStateSwitcher_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{

	class CAltarStateSwitcher : public IComponent
	{
		DEC_FACTORY(CAltarStateSwitcher);
	public:


		CAltarStateSwitcher() : IComponent(GetAltTypeIdOf(CAltarStateSwitcher)), _switchingState(false), _switchingAllowed(false), _target(NULL), _sense(Logic::Sense::UNDEFINED), _acumRotation(0) {}

		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		
		void deactivate();

		void tick(unsigned int msecs);

		bool accept(const CMessage *message);

		virtual void process(CMessage *message);

		
		void allowSwitching();
		void prohibitSwitching();
		
		void startSwitchingState();
		void stopSwitchingState();

	protected:

		/**
		*/
		bool _switchingState;

		/**
		*/
		bool _switchingAllowed;

		/**
		*/
		CEntity* _target;

		/*
		**/
		Logic::Sense _sense;

		/*
		**/

		float _acumRotation;
		
		

	}; // class CAltarStateSwitcher

	REG_FACTORY(CAltarStateSwitcher);

} // namespace Logic

#endif // __Logic_AltarStateSwitcher_H */

