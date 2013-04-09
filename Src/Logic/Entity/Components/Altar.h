/**
@file Altar.h

Contiene la declaración del componente que controla la
activacion/desactivación de un Altar.

@see Logic::CAltar


@author Emilio Santalla
@date Marzo, 2013
*/

#ifndef __Logic_Altar_H
#define __Logic_Altar_H

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

	class CAltar : public IComponent
	{
		DEC_FACTORY(CAltar);
	public:


		CAltar() : IComponent(GetAltTypeIdOf(CAltar)), _switchingState(false), 
			_revertingState(), _on(false), _switchingTime(3000), _acumTime(0), _player(NULL) {}

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
		std::string _activatedMaterial;

		/**
		*/
		std::string _unactivatedMaterial;
		
		/**
		*/
		CEntity* _player;

		Logic::CGameStatus* _gameStatus;

		Logic::CAltarInfo* _altarInfo;
	}; // class CAltar

	REG_FACTORY(CAltar);

} // namespace Logic

#endif // __Logic_Altar_H */