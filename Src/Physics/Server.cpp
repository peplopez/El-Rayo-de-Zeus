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

	CServer::CServer() :  _activeScene(0) 
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
		_dummyScene = createScene("dummy_scene"); // Creamos la escena dummy para cuando no hay ninguna activa.		
		setActiveScene(_dummyScene); // Por defecto la escena activa es la dummy

		return true;
	} // open

	//--------------------------------------------------------
		
	void CServer::close()  // FRS Se ejecuta justo antes de la delete de _instance
	{
		if(_activeScene){
			_activeScene->deactivate();
			_activeScene = 0;
		}

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

	void CServer::tick(unsigned int msecs) 
	{
		TScenes::const_iterator it = _scenes.begin();
		TScenes::const_iterator end = _scenes.end();

		for (; it != end; ++it)
			if(it->second != _dummyScene)
				it->second->tick(msecs);
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
		
		if(_activeScene == scene) // Si borramos la escena activa tenemos que quitarla.
			_activeScene = 0;
		_scenes.erase( scene->getName() );
		delete scene;

	} // removeScene

	void CServer::removeScene(const std::string& name)
	{
		removeScene( _scenes[name] );
	} // removeScene

	//--------------------------------------------------------
	

	void CServer::setActiveScene(CScene* scene)
	{
		// En caso de que hubiese una escena activa la desactivamos.
		if(_activeScene)
			_activeScene->deactivate();

		if(!scene) // Si se añade NULL ponemos la escena dummy.		
			_activeScene = _dummyScene;
		else {
			// Sanity check. Nos aseguramos de que la escena pertenezca 
			// al servidor. Aunque nadie más puede crear escenas...
			assert( _scenes[ scene->getName() ] == scene && 
				"PHYSICS::SERVER>> Esta escena no pertenece al servidor");

			_activeScene = scene;
		}

		_activeScene->activate(); 
	} // setActiveScene
	

	void CServer::setActiveScene(const std::string& name)
	{
		assert(_scenes.find(name) == _scenes.end() &&
			"PHYSICS::SERVER>> Esta escena no pertenece al servidor");
		setActiveScene( _scenes[name] );
	} // setActiveScene

	
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