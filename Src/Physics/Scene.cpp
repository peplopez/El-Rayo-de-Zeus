//----------------------------------------------12


//-----------------------------
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

	CScene::~CScene() 
	{
		deactivate();	
		/* UNDONE FRS Sacado de Graphics::Scene
		_sceneMgr->destroyStaticGeometry(_staticGeometry);		
		_root->destroySceneManager(_sceneMgr);*/ 

	} // ~CScene
	
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
	
	void CScene::tick(unsigned int secs)
	{	
		this->simulate(); // Empezar la simulación física.
	} // tick

	


	/************
		ACTORS
	************/

	// TODO FRS hacerlas inline? 
	bool CScene::addActor(CActor* collider)
	{
		_colliders.push_back(collider);
		return true;
	} // addActor

	bool CScene::addActor(CActorTrigger* trigger)
	{
		_triggers.push_back(trigger);
		return true;
	} // addActor


		

	//--------------------------------------------------------


	void CScene::removeActor(CActor* collider)
	{
		TColliders::iterator colliderIndex = std::find(_colliders.begin(), _colliders.end(), collider);
		if (colliderIndex != _colliders.end())
			_colliders.erase(colliderIndex); // FRS El delete es responsabilidad del creador (Logic::CPhysics)		
	} // removeActor

	void CScene::removeActor(CActorTrigger* trigger)
	{
		TTriggers::iterator triggerIndex = std::find(_triggers.begin(), _triggers.end(), trigger);
		if (triggerIndex != _triggers.end())			
			_triggers.erase(triggerIndex);// FRS El delete es responsabilidad del creador (Logic::CPhysics)		
	} // removeActor

	

	/******************
		SIMULATION
	*****************/
	
	void CScene::simulate()
	{	
		checkCollisions();
		checkTriggers();

	} // simulate	

	//--------------------------------------------------------

	void CScene::checkCollisions() {

		//WTF!!
		float overlapX = 0;
		float overlapY = 0;
		for (size_t i = 0; i < _colliders.size() - 1; ++i)	
			for (size_t j = i + 1; j < _colliders.size(); ++j)
				if ( _colliders[i]->intersects(_colliders[j], overlapX, overlapY) )
				{		
					LOG("Collision ")
					resolveCollision(_colliders[i], _colliders[j], overlapX, overlapY);
					_colliders[i]->getIObserver()->onCollision(_colliders[j]->getIObserver());
					_colliders[j]->getIObserver()->onCollision(_colliders[i]->getIObserver());	
				}

	} // checkCollisions

	//--------------------------------------------------------

	void CScene::checkTriggers() {

		for (size_t i = 0; i < _triggers.size(); ++i)	
			for (size_t j = 0; j < _colliders.size(); ++j)
				
				if ( _colliders[j]->intersects( _triggers[i] ) ) {
					if( _triggers[i]->enters( _colliders[j] ) ) {						
						LOG("Trigger Enter")
						_triggers[i]->getIObserver()->onTrigger( _colliders[j]->getIObserver(), true);
						_colliders[j]->getIObserver()->onTrigger( _triggers[i]->getIObserver(), true);	
					}
					
				} else if( _triggers[i]->exits( _colliders[j] ) ) {						
						LOG("Trigger Exit")
						_triggers[i]->getIObserver()->onTrigger( _colliders[j]->getIObserver(), false);
						_colliders[j]->getIObserver()->onTrigger( _triggers[i]->getIObserver(), false);
				}

	} // checkTriggers


	//--------------------------------------------------------

	void CScene::resolveCollision(Physics::CActor *actor1, Physics::CActor *actor2, float overlapX, float overlapY)
	{

		if (abs(overlapX) < abs(overlapY) || overlapY == 0)
		{
			Logic::CLogicalPosition* pos = actor2->getLogicPosition();
			pos->setDegree(pos->getDegree() - overlapX);
			actor2->setLogicPosition(pos);
		}
		else
		{
			if (overlapY < 0)
			{
				if ((actor1->getLogicPosition()->getHeight() - actor1->getBoxHeight() + overlapY * 0.5f ) >  0)
					actor1->getLogicPosition()->setHeight(actor1->getLogicPosition()->getHeight() + overlapY * 0.5f);
				if ((actor2->getLogicPosition()->getHeight() - actor2->getBoxHeight() - overlapY * 0.5f ) >  0)
					actor2->getLogicPosition()->setHeight(actor2->getLogicPosition()->getHeight() - overlapY * 0.5f);
			}
			else if (overlapY > 0)
			{
				if ((actor1->getLogicPosition()->getHeight() - actor1->getBoxHeight() - overlapY * 0.5f ) >  0)
					actor1->getLogicPosition()->setHeight(actor1->getLogicPosition()->getHeight() - overlapY * 0.5f);
				if ((actor2->getLogicPosition()->getHeight() - actor2->getBoxHeight() + overlapY * 0.5f) >  0)
					actor2->getLogicPosition()->setHeight(actor2->getLogicPosition()->getHeight() + overlapY * 0.5f);
			}
		}
	}	


} // namespace Physics