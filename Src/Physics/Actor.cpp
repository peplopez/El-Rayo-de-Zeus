//---------------------------------------------------------------------------
// Actor.cpp
//---------------------------------------------------------------------------

/**
@file Actor.cpp


@see Physics::CActor

@author Emilio Santalla
@date Febrero, 2013
*/

#include "Actor.h"
#include "BaseSubsystems/Math.h"
#include "Logic/Entity/LogicalPosition.h"




namespace Physics
{
		CActor::CActor(const Logic::TLogicalPosition &position, const float angularWidth, const float height, 
					bool trigger, IObserver *component) : _logicPosition(position), _width(angularWidth), _height(height), _trigger(trigger), _component(component)
		{
		}

		//--------------------------------------------------------
		void CActor::move(const Logic::TLogicalPosition &pos)
		{
		}
		
		//--------------------------------------------------------

		bool CActor::intersects(const CActor *actor)
		{
		}

		//--------------------------------------------------------
		Logic::TLogicalPosition& CActor::getGlobalPose()
		{
		}

		void CActor::release()
		{
			delete this;
		}
		


} // namespace Physics


