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
#include "Physics/ContactListener.h"
#include <Physics\DebugDraw\OgreB2DebugDraw.h>


#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Common\b2Math.h>
#include <Box2D\Common\b2Settings.h>


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
	CScene::CScene(const std::string& name) : _name(name), _world(0), _debugDraw(0), _worldListener(0), _debugDrawEnabled(false)
	{	
		b2Vec2 gravity(0, -20);
		_world = new b2World(gravity);

		if (_name != "dummy_scene")
		{
#ifdef _DEBUG
			_debugDraw = new OgreB2DebugDraw(Graphics::CServer::getSingletonPtr()->getScene(name)->getSceneMgr(), "debugDraw") ;
			_debugDraw->setAutoTracking(Graphics::CServer::getSingletonPtr()->getScene(name)->getCamera()->getCameraNode());
			_debugDraw->SetFlags(b2Draw::e_shapeBit);
			_world->SetDebugDraw(_debugDraw);
#endif		
			_worldListener = new CContactListener();
			_world->SetContactListener(_worldListener);

			CreateWorldEdges();
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
		float32 timeStep = msecs * 0.001f;
		int32 velocityIterations = 6;
		int32 positionIterations = 3;
#ifdef _DEBUG
		_debugDraw->clear();
#endif
		_world->Step(timeStep, velocityIterations, positionIterations); //simulación física
		if (_actorsToGhost.size() > 0)
			createGhostBodies();
		if (_actorsToUnghost.size() > 0)
			deleteGhostBodies();
#ifdef _DEBUG
		if (_debugDrawEnabled)
		{
			_world->DrawDebugData();
			_debugDraw->Render();
		}
#endif
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


	//--------------------------------------------------------

	void CScene::CreateWorldEdges()
	{

		CActor* leftWorldEdge = new CActor(180, -100, Logic::Ring::CENTRAL_RING, "static", 0);
		CActor* rightWorldEdge = new CActor(-180, -100, Logic::Ring::CENTRAL_RING, "static", 0);

		leftWorldEdge->createFixture(3, 100, 0, 0, 0, true);
		rightWorldEdge->createFixture(3, 100, 0, 0, 0, true);

		leftWorldEdge->attachToScene(this);
		rightWorldEdge->attachToScene(this);

	}

	//--------------------------------------------------------
	
	void CScene::deferredGhostBody(CActor* actor)
	{
		_actorsToGhost.push_back(actor);
	}

	//--------------------------------------------------------
	
	void CScene::deferredUnghostBody(CActor* actor)
	{
		_actorsToUnghost.push_back(actor);
	}

	//--------------------------------------------------------
	
	void CScene::createGhostBodies()
	{
		for (int i = 0; i < _actorsToGhost.size(); ++i)
			_actorsToGhost[i]->createGhostBody();
		
		_actorsToGhost.clear();
	}
	
	//--------------------------------------------------------

	void CScene::deleteGhostBodies()
	{
		for (int i = 0; i < _actorsToUnghost.size(); ++i)
			_actorsToUnghost[i]->deleteGhostBody();

		_actorsToUnghost.clear();
	}

	//--------------------------------------------------------
	void CScene::switchDebugDraw()
	{
		_debugDrawEnabled = !_debugDrawEnabled;

	}
	
} // namespace Physics