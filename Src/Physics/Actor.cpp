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
					bool trigger, IObserver *component) : _logicPosition(position), _boxWidth(angularWidth), _boxHeight(height), _trigger(trigger), _component(component)
		{
		}

		//--------------------------------------------------------
		void CActor::move(const Logic::TLogicalPosition &pos)
		{
			_logicPosition  = pos;
		}
		
		//--------------------------------------------------------

		bool CActor::intersects(CActor *otherActor)
		{
			return  (_logicPosition._degrees < otherActor->getLogicPos()._degrees + otherActor->getBoxWidth()) && 
            (otherActor->getLogicPos()._degrees < _logicPosition._degrees + _boxWidth)            && 
            (_logicPosition._height < otherActor->getLogicPos()._height + otherActor->getBoxHeight()) && 
            (otherActor->getLogicPos()._height < _logicPosition._height + _boxHeight);              
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


