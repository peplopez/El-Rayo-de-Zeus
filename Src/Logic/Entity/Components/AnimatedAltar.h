/**
@file AnimatedAltar.h

Contiene la declaración del componente que controla la
activacion/desactivación de un Altar.

@see Logic::CAnimatedAltar


@author Emilio Santalla
@date Marzo, 2013
*/

#ifndef __Logic_AnimatedAltar_H
#define __Logic_AnimatedAltar_H

#include "Logic/Entity/Component.h"
#include "Application/BaseApplication.h"

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Logic
{
	class CMessage;
	class CGameStatus;
	class CAltarInfo;
}

//declaración de la clase
namespace Logic 
{

	class CAnimatedAltar : public IComponent
	{
		DEC_FACTORY(CAnimatedAltar);
	public:


		CAnimatedAltar() : IComponent(GetAltTypeIdOf(CAnimatedAltar)), _switchingState(false), 
			_revertingState(), _on(false), _switchingTime(2500), _acumTime(0), _player(NULL) {}

		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		
		void deactivate();

		void tick(unsigned int msecs);

		bool accept(const CMessage *message);

		virtual void process(CMessage *message);

		
		void startSwitchingState();
		void stopSwitchingState();

		bool isActivated(){return _on;}

	protected:

		/**
		*/
		bool _switchingState;

		/**
		*/
		bool _revertingState;

		/**
		*/
		bool _on;

		/**
		*/

		int _switchingTime;

		/**
		*/
		int _acumTime;
	
		/**
		*/
		CEntity* _player;

		Logic::CGameStatus* _gameStatus;

		Logic::CAltarInfo* _altarInfo;
	}; // class CAltar

	REG_FACTORY(CAnimatedAltar);

} // namespace Logic

#endif // __Logic_Altar_H */