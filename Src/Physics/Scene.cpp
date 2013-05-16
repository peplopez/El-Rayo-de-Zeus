//-----------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp


@see Physics::CScene

@author Emilio Santalla
@date Febrero 2013
*/
#include <string>

#include "Physics/IObserver.h"
#include "Physics/Scene.h"
#include "Physics/Actor.h"
#include "Physics/ActorTrigger.h"


#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Common\b2Math.h>
#include <Box2D\Common\b2Settings.h>
#include <Physics\DebugDraw\OgreB2DebugDraw.h>

#include "Graphics\Server.h"
#include "Graphics\Scene.h"
#include "Graphics\Camera.h"


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
	CScene::CScene(const std::string& name) : _name(name), _world(0) 
	{	
		b2Vec2 gravity(0, -10);
		_world = new b2World(gravity);

		if (_name != "dummy_scene")
		{
			
			_debugDraw = new OgreB2DebugDraw(Graphics::CServer::getSingletonPtr()->getScene(name)->getSceneMgr(), "debugDraw") ;
			_debugDraw->setAutoTracking(Graphics::CServer::getSingletonPtr()->getScene(name)->getCamera()->getCameraNode());
			_world->SetDebugDraw(_debugDraw);
			_debugDraw->SetFlags(b2Draw::e_shapeBit);
		}
	};

	CScene::~CScene() 
	{
		delete _world;
		deactivate();	

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
	
	void CScene::tick(unsigned int msecs)
	{	
		this->simulate(msecs); // Empezar la simulación física.
	} // tick

	


	/************
		ACTORS
	************/

	bool CScene::add(CActor* actor)
	{
		if(!actor->attachToScene(this))
			return false;
		else 
		{
			_actors.push_back(actor);
			return true;
		}
	} // addActor


	//--------------------------------------------------------


	void CScene::remove(CActor* actor)
	{
		actor->detachFromScene();		

		TActors::iterator actorIndex = std::find(_actors.begin(), _actors.end(), actor);
		if (actorIndex != _actors.end())
			_actors.erase(actorIndex); // FRS El delete es responsabilidad del creador (Logic::CPhysics)		
	} // removeActor



	
	/******************
		SIMULATION
	*****************/
	
	void CScene::simulate(unsigned int timeDiff)
	{	
		float32 timeStep = timeDiff * 0.001f;
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		_debugDraw->clear();
		_world->Step(timeStep, velocityIterations, positionIterations);
		_world->DrawDebugData();
		



	} // simulate	

	//--------------------------------------------------------

} // namespace Physics