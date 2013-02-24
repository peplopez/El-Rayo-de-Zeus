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

		bool CActor::intersects(CActor *otherActor, float &x, float &y)
		{
			
			float x = 0;
			float y = 0;
			float xCenterDistance = _logicPosition._degrees - otherActor->getLogicPos()._degrees;
			float yCenterDistance = 0;
			if (xCenterDistance > 180) //
				xCenterDistance -= 360;	
			else if (xCenterDistance < 180)
				xCenterDistance += 360;
			
			if (abs(xCenterDistance) > (_boxWidth + otherActor->getBoxWidth()))
				return false;
			else if (abs( yCenterDistance = (_logicPosition._height - otherActor->getLogicPos()._height) ) > (_boxHeight + otherActor->getBoxHeight()))
				return false;
			//si hay intersección devuelvo la información de la misma
			else
			{
				if (abs(xCenterDistance) < (_boxWidth + otherActor->getBoxWidth()))
					if (xCenterDistance < 0)
						x = -(_boxWidth + otherActor->getBoxWidth() - abs(xCenterDistance));
					else
						x = _boxWidth + otherActor->getBoxWidth() - abs(xCenterDistance);

				if (abs(yCenterDistance) < (_boxHeight + otherActor->getBoxHeight()))
					if (yCenterDistance < 0)
						y = -(_boxHeight + otherActor->getBoxHeight() - abs(yCenterDistance));
					else
						y = _boxHeight + otherActor->getBoxHeight() - abs(yCenterDistance);

				return true;
			}

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


