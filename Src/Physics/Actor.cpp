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
			
			float centerDistance = abs(_logicPosition._degrees - otherActor->getLogicPos()._degrees);
			if (centerDistance>180) //
				centerDistance=360-centerDistance;	
			return  centerDistance < (_boxWidth + otherActor->getBoxWidth()) && 
						abs(_logicPosition._height - otherActor->getLogicPos()._height) < (+ _boxHeight + otherActor->getBoxHeight());            
		}

		//--------------------------------------------------------
		
		Logic::TLogicalPosition& CActor::getGlobalPose()
		{
			return _logicPosition;
		}
		
		//--------------------------------------------------------
		void CActor::release()
		{
			delete this;
		}
		


} // namespace Physics


