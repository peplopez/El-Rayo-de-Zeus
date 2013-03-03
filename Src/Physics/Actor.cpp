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
					bool isTrigger, IObserver *component) : 
		_logicPosition(position), _boxWidth(angularWidth), 
			_boxHeight(height), _isTrigger(isTrigger), _component(component)
		{
		}

		//--------------------------------------------------------


		
		void CActor::move(const float degrees, const float height, const char ring, const char base)
		{		
			// HACK aqu� habr�a que recurrir a los setter securizados de logicPosition
			_logicPosition._degrees	+= degrees;
			_logicPosition._height	+= height;			
			_logicPosition._ring	= static_cast<Logic::LogicalPosition::Ring>(_logicPosition._ring + ring);		
			_logicPosition._base	+= base; 

			if (_logicPosition._height < 0)
				_logicPosition._height = 0;
			
			if (_logicPosition._degrees < 0)
				_logicPosition._degrees = 360 + _logicPosition._degrees;
			else if (_logicPosition._degrees >= 360)
				_logicPosition._degrees = _logicPosition._degrees - 360;
		
		} // move


		//--------------------------------------------------------

		bool CActor::intersects(CActor *otherActor, float &x, float &y)
		{
			
			if (_logicPosition._base != otherActor->getLogicPos()._base)
				return false;
			if (_logicPosition._ring != otherActor->getLogicPos()._ring)
				return false;
			x = 0;
			y = 0;
			float xCenterDistance = _logicPosition._degrees - otherActor->getLogicPos()._degrees;
			float yCenterDistance = 0;
			if (xCenterDistance > 180) //
				xCenterDistance -= 360;	
			else if (xCenterDistance < -180)
				xCenterDistance += 360;
			
			if (abs(xCenterDistance) >= (_boxWidth + otherActor->getBoxWidth()))
				return false;
			else if (abs( yCenterDistance = (_logicPosition._height - otherActor->getLogicPos()._height) ) >= (_boxHeight + otherActor->getBoxHeight()))
				return false;

			// INTERSECCI�N: devuelvo la informaci�n de la misma
			else
			{
				if (abs(xCenterDistance) < (_boxWidth + otherActor->getBoxWidth()))
					if (xCenterDistance < 0)
						x = -(_boxWidth + otherActor->getBoxWidth() - abs(xCenterDistance));
					else if (xCenterDistance > 0)
						x = _boxWidth + otherActor->getBoxWidth() - abs(xCenterDistance);

			
				if (abs(yCenterDistance) < (_boxHeight + otherActor->getBoxHeight()))
					if (yCenterDistance < 0)
						y = -(_boxHeight + otherActor->getBoxHeight() - abs(yCenterDistance));
					else if (yCenterDistance > 0)
						y = _boxHeight + otherActor->getBoxHeight() - abs(yCenterDistance);
		
				return true;
			}

		}
		

		// HACK revisar
		bool CActor::intersects(CActor *otherActor)
		{
			if (_logicPosition._base != otherActor->getLogicPos()._base)
				return false;
			if (_logicPosition._ring != otherActor->getLogicPos()._ring)
				return false;

			float xCenterDistance = _logicPosition._degrees - otherActor->getLogicPos()._degrees;
			float yCenterDistance = 0;
			if (xCenterDistance > 180) //
				xCenterDistance -= 360;	
			else if (xCenterDistance < -180)
				xCenterDistance += 360;		
			if (abs(xCenterDistance) > (_boxWidth + otherActor->getBoxWidth()))
				return false;
			else if (abs( yCenterDistance = (_logicPosition._height - otherActor->getLogicPos()._height) ) > (_boxHeight + otherActor->getBoxHeight()))
				return false;

			// INTERSECCI�N
			else
				return true;
		}

		//--------------------------------------------------------


		void CActor::release()
		{
			delete this;
		}
		


} // namespace Physics


