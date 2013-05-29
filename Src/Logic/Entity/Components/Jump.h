/**
@file Jump.h

Contiene la declaración del componente que controla el movimiento 
de la entidad.

@see Logic::CJump
@see Logic::IComponent

@author
@date 
*/

#ifndef __Logic_Jump_H
#define __Logic_Jump_H

#include "Logic/Entity/Component.h"


namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{


	class CJump : public IComponent
	{
		DEC_FACTORY(CJump);
	public:

		CJump() : IComponent(GetAltTypeIdOf(CJump)), _jumping(false), _jumpSpeed(0), _jumpDecay(0), _initialJumpSpeed(0), _lastTickHeight(0), _maxHeightReached(false){}
		
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		
		void deactivate();

		void tick(unsigned int msecs);

		bool accept(const CMessage *message);

		void process(CMessage *message);

		void jump();

		bool isJumping() { return _jumping;}

	protected:

		/**
		*/
		bool _jumping;

		/**
		*/
		float _jumpSpeed;


		/**
		*/
		float _jumpDecay;
		
		/**
		*/
		float _lastTickHeight;

		/**
		*/
		float _initialJumpSpeed;

		/**
		*/
		bool _maxHeightReached;


	}; // class CJump

	REG_FACTORY(CJump);

} // namespace Logic

#endif // __Logic_Jump_H */
