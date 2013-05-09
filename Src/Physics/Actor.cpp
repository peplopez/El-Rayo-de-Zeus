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
#include "../Logic/Entity/LogicalPosition.h"
namespace Physics
{
		CActor::CActor(Logic::CLogicalPosition *position, const float angularWidth, const float height, float widthOffset, float heightOffset, IObserver *component) : 
		 _boxWidth(angularWidth), _boxHeight(height), _component(component), _widthOffset(widthOffset), _heightOffset(heightOffset)
		{
			_logicPosition =new Logic::CLogicalPosition();
			_logicPosition->setBase(position->getBase());
			_logicPosition->setRing(position->getRing());
			_logicPosition->setHeight(position->getHeight() + height + heightOffset);
			_logicPosition->setDegree(position->getDegree() + widthOffset);
			_logicPosition->setSense(position->getSense());
		}

		//--------------------------------------------------------
		
		void CActor::move(const float degrees, const float height, const char ring, const char base)
		{		
			
			_logicPosition->setDegree(_logicPosition->getDegree() + degrees );
	
			_logicPosition->setRing(static_cast<Logic::LogicalPosition::Ring>(_logicPosition->getRing() + ring));		
			_logicPosition->setBase(_logicPosition->getBase() + base); 

			((_logicPosition->getHeight() + height - _boxHeight - _heightOffset) > 0) ? 
				_logicPosition->setHeight(_logicPosition->getHeight() + height) : _logicPosition->setHeight(_boxHeight + _heightOffset);

		} // move


		//--------------------------------------------------------

		bool CActor::intersects(CActor *otherActor, float &overlapX, float &overlapY)
		{
			
			if (_logicPosition->getBase() != otherActor->getLogicPosition()->getBase())
				return false;
			if (_logicPosition->getRing() != otherActor->getLogicPosition()->getRing())
				return false;
			overlapX = 0;
			overlapY = 0;
			float xCenterDistance = _logicPosition->getDegree() - otherActor->getLogicPosition()->getDegree();
			float yCenterDistance = 0;
			if (xCenterDistance > 180) //
				xCenterDistance -= 360;	
			else if (xCenterDistance < -180)
				xCenterDistance += 360;
			
			if (abs(xCenterDistance) >= (_boxWidth + otherActor->getBoxWidth()))
				return false;
			else if (abs( yCenterDistance = (_logicPosition->getHeight() - otherActor->getLogicPosition()->getHeight()) ) >= (_boxHeight + otherActor->getBoxHeight()))
				return false;

			// INTERSECCIÓN: devuelvo la información de la misma overlapX y overlapY
			else
			{
				if (abs(xCenterDistance) < (_boxWidth + otherActor->getBoxWidth()))
					if (xCenterDistance < 0)
						overlapX = -(_boxWidth + otherActor->getBoxWidth() - abs(xCenterDistance));
					else if (xCenterDistance > 0)
						overlapX = _boxWidth + otherActor->getBoxWidth() - abs(xCenterDistance);

			
				if (abs(yCenterDistance) < (_boxHeight + otherActor->getBoxHeight()))
					if (yCenterDistance < 0)
						overlapY = -(_boxHeight + otherActor->getBoxHeight() - abs(yCenterDistance));
					else if (yCenterDistance > 0)
						overlapY = _boxHeight + otherActor->getBoxHeight() - abs(yCenterDistance);
		
				return true;
			}

		}
		

		// HACK revisar
		bool CActor::intersects(CActor *otherActor)
		{
			if (_logicPosition->getBase() != otherActor->getLogicPosition()->getBase())
				return false;
			if (_logicPosition->getRing() != otherActor->getLogicPosition()->getRing())
				return false;

			float xCenterDistance = _logicPosition->getDegree() - otherActor->getLogicPosition()->getDegree();
			float yCenterDistance = 0;
			if (xCenterDistance > 180) //
				xCenterDistance -= 360;	
			else if (xCenterDistance < -180)
				xCenterDistance += 360;		
			if (abs(xCenterDistance) > (_boxWidth + otherActor->getBoxWidth()))
				return false;
			else if (abs( yCenterDistance = (_logicPosition->getHeight() - otherActor->getLogicPosition()->getHeight()) ) > (_boxHeight + otherActor->getBoxHeight()))
				return false;

			// INTERSECCIÓN
			else
				return true;
		}
		
} // namespace Physics


