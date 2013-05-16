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
#include "Physics\DebugDraw\Render.h"

#include <../Freeglut/freeglut_std.h>
#include <../Freeglut/freeglut_ext.h>

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
			int width = 500;
			int height = 500;
			unsigned int displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

			char *myargv [1];
			int myargc=1;
			myargv [0]=strdup (_name.c_str());

			glutInit(&myargc, myargv);
			glEnable(GL_DEPTH_TEST);
			glutInitDisplayMode (displayMode);
			glutInitContextVersion (3, 3);
			glutInitContextProfile(GLUT_CORE_PROFILE);
			#ifdef DEBUG
			glutInitContextFlags(GLUT_DEBUG);
			#endif
			glutInitWindowSize (width, height); 
			glutInitWindowPosition (300, 200);


			int window = glutCreateWindow (myargv [0]);

			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

			
			_debugDraw = new DebugDraw() ;
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


		_world->Step(timeStep, velocityIterations, positionIterations);



	} // simulate	

	//--------------------------------------------------------

} // namespace Physics