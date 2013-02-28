//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp


@see Physics::CScene

@author Emilio Santalla
@date Febrero 2013
*/

#include "Logic/Entity/LogicalPosition.h"
#include "Physics/IObserver.h"
#include "Physics/Scene.h"

#include <algorithm>
#include <assert.h>
#include <iostream>


#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "PHYSICS::SCENE>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace Physics
{

	CScene::CScene() {}


	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();

	} // ~CScene

	//--------------------------------------------------------

	void CScene::addActor(CActor* actor)
	{
		_actors.push_back(actor);

	} // addActor

	//--------------------------------------------------------


	void CScene::removeActor(CActor* actor)
	{
		std::vector<CActor*>::iterator position = std::find(_actors.begin(), _actors.end(), actor);
		if (position != _actors.end())
		{
			(*position)->release();
			_actors.erase(position);
		}
	} // removeActor

	//--------------------------------------------------------
	void CScene::release()
	{
		TActorVector::iterator itr = _actors.begin();
		for ( ; itr != _actors.end(); ++itr) 
		{
			(*itr)->release();
		}

		_actors.clear();
	}

	//--------------------------------------------------------

	bool CScene::activate()
	{
		return true;
	} // activate

	//--------------------------------------------------------

	bool CScene::deactivate()
	{
		return true;
	} // deactivate
	
	//--------------------------------------------------------

	
	void CScene::simulate()
	{	
		checkCollisions();
		checkTriggers();

	} // simulate	

	//--------------------------------------------------------

	void CScene::checkCollisions() {

		//WTF!!
		float x = 0;
		float y = 0;
		for (int i = 0; i < _actors.size() - 1; ++i)	// TODO impl. diferentes listas de colision / triggers
			for (int j = i + 1; j < _actors.size(); ++j)
				if ( _actors[i]->intersects(_actors[j], x, y) )
				{					
					if ( (_actors[i]->isTrigger() || _actors[j]->isTrigger()))
					{
						LOG("Trigger")
						_actors[i]->getIObserver()->onTrigger(_actors[j]->getIObserver(), true);
						_actors[j]->getIObserver()->onTrigger(_actors[i]->getIObserver(), true);						
					}
					else
					{
						LOG("Colision")
						updateLogicPosition(_actors[i], _actors[j], x, y);
						_actors[i]->getIObserver()->onCollision(_actors[j]->getIObserver());
						_actors[j]->getIObserver()->onCollision(_actors[i]->getIObserver());						
					}
				}
	} // checkCollisions

	//--------------------------------------------------------

	void CScene::checkTriggers() {

	}

	void CScene::updateLogicPosition(Physics::CActor *actor1, Physics::CActor *actor2, float x, float y)
	{
		if (abs(x) < abs(y))
		{
			if (x < 0)
			{
				Logic::TLogicalPosition pos = actor2->getLogicPos();
				pos._degrees += -x;
				if (pos._degrees > 360)
					pos._degrees -= 360;
				actor2->setLogicPos(pos);
			}
			else if (x > 0)
			{
				Logic::TLogicalPosition pos = actor2->getLogicPos();
				pos._degrees -= x;
				if (pos._degrees < 0)
					pos._degrees += 360;
				actor1->setLogicPos(pos);
			}
		}
		else
		{
			if (y < 0)
			{
				Logic::TLogicalPosition pos = actor2->getLogicPos();
				pos._height += -y;
				actor2->setLogicPos(pos);
			}
			else if (y > 0)
			{
				Logic::TLogicalPosition pos = actor2->getLogicPos();
				pos._height += y;
				actor2->setLogicPos(pos);
			}
		}
	}


} // namespace Physics