/**
@file Server.cpp

Contiene la implementación del servidor de física. 

@see Physics::CServer

@author Emilio Santalla
@date Marzo, 2013
*/
#include "Server.h"

#include "Physics/IObserver.h"
#include "Physics/Actor.h"
#include "Physics/Scene.h"

#include <assert.h>


namespace Physics {


	
	CServer *CServer::_instance = 0; // Única instancia del servidor

	//--------------------------------------------------------

	CServer::CServer()
	{
		assert(!_instance && "PHYSICS::SERVER>> Segunda inicialización de Physics::CServer no permitida!");
		_instance = this;
	} // CServer

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);
		_instance = 0;
	} // ~CServer

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "PHYSICS::SERVER>> Segunda inicialización de Physics::CServer no permitida!");

		new CServer();

		if (!_instance->open()) {
			Release();
			return false;
		}
		return true;
	} 

	//--------------------------------------------------------

	void CServer::Release()
	{
		if(_instance) {
			
			_instance->close();
			delete _instance;
		}
	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{				
		return true;
	} // open

	//--------------------------------------------------------
		
	void CServer::close()  // FRS Se ejecuta justo antes de la delete de _instance
	{

		TScenes::const_iterator it = _scenes.begin();
		TScenes::const_iterator end = _scenes.end();
			while(it != end)			
				removeScene( (*it++).second );
		
	} // close

	//--------------------------------------------------------

	void CServer::SwitchDebugDraw()
	{
		TScenes::const_iterator it = _scenes.begin();
		TScenes::const_iterator end = _scenes.end();
		for (; it != end; ++it)
			it->second->switchDebugDraw();
	}
	
	/***********
		TICK
	***********/

	void CServer::tick(float secs) 
	{
		TScenes::const_iterator it = _scenes.begin();
		TScenes::const_iterator end = _scenes.end();

		for (; it != end; ++it)
			it->second->tick(secs);
	} // tick

	
	/************
		SCENE
	**************/
		
	CScene* CServer::createScene(const std::string &name)
	{
		assert(_instance && "PHYSICS::SERVER>> Servidor no inicializado");			
		assert(_scenes.find(name)==_scenes.end() && "Ya se ha creado una escena con este nombre.");

		CScene *scene = new CScene(name);
			_scenes[name] =  scene;
		return scene;

	} // createScene

	//--------------------------------------------------------

	void CServer::removeScene(CScene* scene)
	{
		assert(_instance && "PHYSICS::SERVER>> Servidor no inicializado");
		_scenes.erase( scene->getName() );
		delete scene;

	} // removeScene

	void CServer::removeScene(const std::string& name)
	{
		removeScene( _scenes[name] );
	} // removeScene

	//--------------------------------------------------------
	

	void CServer::activate(CScene* scene)
	{
	
		// Sanity check. Nos aseguramos de que la escena pertenezca 
		// al servidor. Aunque nadie más puede crear escenas...
		assert( _scenes[ scene->getName() ] == scene && 
				"PHYSICS::SERVER>> Esta escena no pertenece al servidor");
		scene->activate(); 
	} // activate
	

	void CServer::activate(const std::string& name)
	{
		assert(_scenes.find(name) == _scenes.end() &&
			"PHYSICS::SERVER>> Esta escena no pertenece al servidor");
		activate( _scenes[name] );
	} // activate

	//--------------------------------------------------------
	
	void CServer::deactivate(CScene* scene)
	{
	
		// Sanity check. Nos aseguramos de que la escena pertenezca 
		// al servidor. Aunque nadie más puede crear escenas...
		assert( _scenes[ scene->getName() ] == scene && 
				"PHYSICS::SERVER>> Esta escena no pertenece al servidor");
		scene->deactivate(); 
	} // activate
	

	
	/***********
	   ACTORS
	***********/
	// UNDONE FRS
	//// TODO ƒ®§ Devolver flags / eventos de sucesos? -> p.e  PxControllerFlag::eCOLLISION_DOWN / onFloor(enter/exit)
	//// FRS Necesario para pasar posiciones relativas negativas (TLogicalPosition nos restringía a unsigned's)
	//void CServer::moveActor(Physics::CActor *actor, float diffDegrees, float diffHeight, char diffRing, char diffBase)
	//{
	//	assert(actor);	// Mover el actor tras transformar el destino a coordenadas lógicas
	//	actor->move(diffDegrees, diffHeight, diffRing, diffBase);
	//}



}