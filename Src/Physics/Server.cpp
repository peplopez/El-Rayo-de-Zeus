/**
@file Server.cpp

Contiene la implementación del servidor de física. 

@see Physics::CServer

@author Emilio Santalla
@date Marzo, 2013
*/
#include "Server.h"


#include "IObserver.h"
#include "Actor.h"
#include "Scene.h"

#include <assert.h>


namespace Physics {


	// Única instancia del servidor
	CServer *CServer::_instance = NULL;

	//--------------------------------------------------------

	CServer::CServer() :  _scene(NULL) 
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

		_instance = this;
	} 

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);

		_instance = 0;
	} 

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "Segunda inicialización de Physics::CServer no permitida!");

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
	} 

	//--------------------------------------------------------

	bool CServer::open()
	{

		_scene = createScene();		

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		if(_scene)
		{
			_scene->deactivate();
			_scene = 0;
		}
	} // close

	//--------------------------------------------------------

	CScene* CServer::createScene ()
	{
		assert(_instance);
	
		// Crear la escena física
		CScene *scene = new CScene();
		return scene;
	}

	//--------------------------------------------------------

	void CServer::destroyScene ()
	{
		assert(_instance);

		if (_scene) {
			_scene->release();
			_scene = NULL;
		}
	}


	//--------------------------------------------------------

	bool CServer::activateScene()
	{
		return _scene->activate();
	}

	//--------------------------------------------------------

	bool CServer::deactivateScene()
	{
		return _scene->deactivate();
	}

	//--------------------------------------------------------

	void CServer::tick(unsigned int msecs) 
	{
		assert(_scene);

		// Empezar la simulación física.

		_scene->simulate();

	} 

	//--------------------------------------------------------

	Physics::CActor* CServer::createActor(const Logic::TLogicalPosition &position, const float angularWidth, 
											  const float height, bool isTrigger, IObserver *component) 
	{
		assert(_scene);

		Physics::CActor *actor = new Physics::CActor(position, angularWidth, height, isTrigger, component);

		// Añadir el actor a la escena
		_scene->addActor(actor);

		return actor;
	}


	//--------------------------------------------------------

	void CServer::destroyActor(Physics::CActor *actor)
	{
		assert(_scene);

		// Eliminar el actor de la escena
		_scene->removeActor(actor);

		// Liberar recursos
		actor->release();
	}

	//--------------------------------------------------------

	Logic::TLogicalPosition& CServer::getActorLogicPosition(CActor *actor)
	{
		assert(actor);

		// Devolver la posición y orientación en coordenadas lógicas
		return actor->getGlobalPose();
	}

	//--------------------------------------------------------

	// TODO ƒ®§ Tomando como ejemplo el moveController del CPhysicController, 
	// deberíamos añadir msecs como argumento, ¿o no?
	// TODO ƒ®§ Devolver flags de sucesos? -> p.e  PxControllerFlag::eCOLLISION_DOWN
	void CServer::moveActor(CActor *actor, const Logic::TLogicalPosition &pos)
	{
		assert(actor);
		// Mover el actor tras transformar el destino a coordenadas lógicas
		actor->move(pos);
	}

}