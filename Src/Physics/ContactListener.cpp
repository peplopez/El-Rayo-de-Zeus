#include <Box2D\Box2D.h>
#include "ContactListener.h"
#include "IObserver.h"



namespace Physics {

	void CContactListener::BeginContact(b2Contact* contact)
	{
		_l1 = static_cast<IObserver*>(contact->GetFixtureA()->GetBody()->GetUserData());
		_l2 = static_cast<IObserver*>(contact->GetFixtureB()->GetBody()->GetUserData());

		if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureA()->IsSensor())
		{
			_l1->onTrigger(_l2, true);
			_l2->onTrigger(_l1, true);
		}
		else
		{
			_l1->onCollision(_l2);
			_l2->onCollision(_l1);
		}
			
	}

	void CContactListener::EndContact(b2Contact* contact)
	{
		_l1 = static_cast<IObserver*>(contact->GetFixtureA()->GetBody()->GetUserData());
		_l2 = static_cast<IObserver*>(contact->GetFixtureB()->GetBody()->GetUserData());

		if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureA()->IsSensor())
		{
			_l1->onTrigger(_l2, false);
			_l2->onTrigger(_l1, false);
		}

	}

}

