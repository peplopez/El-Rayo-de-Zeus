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
#include "RigidBody.h"

namespace Physics
{
	CActor::CActor(Vector2 position, float radius, float density, float restitution, IObserver *component) : 
		 _component(component)
	{
		Vector2 circlePos(position.x, position.y);
		
		if (circlePos.x > 180)
			circlePos.x -= 360;

		_body = new CRigidBody(circlePos, radius, density, restitution);
	}

	//--------------------------------------------------------

	CActor::CActor(Vector2 min, Vector2 max, float density, float restitution, IObserver *component) : 
		 _component(component)
	{
		_body = new CRigidBody(min, max, density, restitution);
	}

	//--------------------------------------------------------
		
	void CActor::move(const float degrees, const float height)
	{		
		
		_body->_shape->move(Vector2(degrees, height));

	} // move

	//--------------------------------------------------------

	void CActor::moveKinematic(const float degrees, const float height)
	{		
		
		move(degrees, height);
		_body->_diffPosition = Vector2(degrees, height);

	} // move

	//--------------------------------------------------------

	float CActor::getDegree()
	{
		return _body->_shape->getDegree();
		
	}
	//--------------------------------------------------------

	float CActor::getHeight()
	{
		return _body->_shape->getHeight();
	}
	//--------------------------------------------------------

		
} // namespace Physics


