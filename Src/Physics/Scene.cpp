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

#include <assert.h>
#include <iostream>


#define DEBUG 0
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

	// TODO FRS hacerlas inline? 
	void CScene::addActor(CActorTrigger* actor)
	{
		_triggers.push_back(actor);

	} // addActor

	void CScene::addActor(CActor* actor)
	{
		_actors.push_back(actor);
	} // addActor

	

	//--------------------------------------------------------


	void CScene::removeActor(CActor* actor)
	{
		TActorVector::iterator position = std::find(_actors.begin(), _actors.end(), actor);
		if (position != _actors.end())
		{
			(*position)->release();
			_actors.erase(position);
		}
	} // removeActor

	void CScene::removeActor(CActorTrigger* actor)
	{
		TTriggerVector::iterator position = std::find(_triggers.begin(), _triggers.end(), actor);
		if (position != _triggers.end())
		{
			(*position)->release();
			_triggers.erase(position);
		}
	} // removeActor

	//--------------------------------------------------------
	void CScene::release()
	{
		TActorVector::iterator aIt = _actors.begin();
			for ( ; aIt != _actors.end(); ++aIt) {
				(*aIt)->release();
			}
		TTriggerVector::iterator tIt= _triggers.begin();
			for ( ; tIt != _triggers.end(); ++tIt){
				(*tIt)->release();
			}

		_actors.clear();
		_triggers.clear();
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
		for (size_t i = 0; i < _actors.size() - 1; ++i)	
			for (size_t j = i + 1; j < _actors.size(); ++j)
				if ( _actors[i]->intersects(_actors[j], x, y) )
				{		
					LOG("Collision")
					updateLogicPosition(_actors[i], _actors[j], x, y);
					_actors[i]->getIObserver()->onCollision(_actors[j]->getIObserver());
					_actors[j]->getIObserver()->onCollision(_actors[i]->getIObserver());	
				}

	} // checkCollisions

	//--------------------------------------------------------

	void CScene::checkTriggers() {

		for (size_t i = 0; i < _triggers.size(); ++i)	
			for (size_t j = 0; j < _actors.size(); ++j)
				
				if ( _actors[j]->intersects( _triggers[i] ) ) {
					if( _triggers[i]->enters( _actors[j] ) ) {						
						LOG("Trigger Enter")
						_triggers[i]->getIObserver()->onTrigger( _actors[j]->getIObserver(), true);
						_actors[j]->getIObserver()->onTrigger( _triggers[i]->getIObserver(), true);	
					}
					
				} else if( _triggers[i]->exits( _actors[j] ) ) {						
						LOG("Trigger Exit")
						_triggers[i]->getIObserver()->onTrigger( _actors[j]->getIObserver(), false);
						_actors[j]->getIObserver()->onTrigger( _triggers[i]->getIObserver(), false);
				}

	} // checkTriggers


	//--------------------------------------------------------

	void CScene::updateLogicPosition(Physics::CActor *actor1, Physics::CActor *actor2, float x, float y)
	{
		if (abs(x) < abs(y) || y == 0)
		{
			if (x < 0)
			{
				Logic::TLogicalPosition pos = actor2->getLogicPos();
				pos._degrees -= x;
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
				actor2->setLogicPos(pos);
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