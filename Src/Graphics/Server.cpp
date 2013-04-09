//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

/**
@file Server.cpp

Contiene la implementación de la clase principal de gráficos, la que permite crear
la ventana, etc.

@see Graphics::CServer

@author David Llansó
@date Julio, 2010
*/

#include "Server.h"
#include "Scene.h"
#include "Overlay.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgreOverlayManager.h>

namespace Graphics 
{
	CServer *CServer::_instance = 0; // Única instancia del servidor

	//--------------------------------------------------------


	CServer::CServer() : _root(0), _renderWindow(0), _activeScene(0), _dummyScene(0)
	{
		assert(!_instance && "GRAPHICS::SERVER>> Segunda inicialización de Graphics::CServer no permitida!");
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
		assert(!_instance && "GRAPHICS::SERVER>> Segunda inicialización de Graphics::CServer no permitida!");

		new CServer();

		if (!_instance->open())		{
			Release();
			return false;
		}
		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		if(_instance)		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		if(!BaseSubsystems::CServer::getSingletonPtr())
			return false;

		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_renderWindow = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow();

		//PT. Se carga el manager de overlays
		_overlayManager = Ogre::OverlayManager::getSingletonPtr();

		_dummyScene = createScene("dummy_scene"); // Creamos la escena dummy para cuando no hay ninguna activa.		
		setActiveScene(_dummyScene); // Por defecto la escena activa es la dummy

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		if(_activeScene)		{
			_activeScene->deactivate();
			_activeScene = 0;
		}

		TScenes::const_iterator it = _scenes.begin();
		TScenes::const_iterator end = _scenes.end();
			while(it != end)			
				removeScene( (*it++).second );

		// OVERLAYS
		_overlayManager->destroyAll(); // destroys all overlays

	} // close

	//--------------------------------------------------------
	

	//typedef std::pair<std::string,CScene*> TStringScenePar;

	CScene* CServer::createScene(const std::string& name)
	{
		assert(_instance && "GRAPHICS::SERVER>> Servidor no inicializado");			
		assert(_scenes.find(name) == _scenes.end() && "Ya se ha creado una escena con este nombre.");

		CScene *scene = new CScene(name);

		// UNDONE FRS Es mejor insertar pares que la inserción normal por índice?
		//TStringScenePar ssp(name,scene);
		//_scenes.insert(ssp);

		_scenes[name] =  scene;
		return scene;
	} // createScene

	//--------------------------------------------------------

	void CServer::removeScene(CScene* scene)
	{
		assert(_instance && "GRAPHICS::SERVER>> Servidor no inicializado");	

		if(_activeScene == scene) // Si borramos la escena activa tenemos que quitarla.
			_activeScene = 0;
		_scenes.erase(scene->getName());
		delete scene;

	} // removeScene

	//--------------------------------------------------------

	void CServer::removeScene(const std::string& name)
	{
		// UNDONE FRS Y para qué tenemos el acceso por índice [ ]?
		/*CScene* scene = (*_scenes.find(name)).second;
		removeScene(scene);*/

		removeScene( _scenes[name] );
	} // removeScene

	
	//--------------------------------------------------------

	//TODO en red, el server tendrá activas > 1 -> activateScene
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
				"GRAPHICS::SERVER>> Esta escena no pertenece al servidor");

			_activeScene = scene;
		}

		_activeScene->activate(); 
	} // setActiveScene

	//--------------------------------------------------------
	
	void CServer::activateBaseCam(CScene* scene)
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
				"GRAPHICS::SERVER>> Esta escena no pertenece al servidor");

			_activeScene = scene;
		}

		_activeScene->activateBaseCam(); 
	} // setActiveScene
	//--------------------------------------------------------

	void CServer::setActiveScene(const std::string& name)
	{
		assert(_scenes.find(name) == _scenes.end() &&
			"GRAPHICS::SERVER>> Esta escena no pertenece al servidor");
		setActiveScene( _scenes[name] );
	} // setActiveScene

	//--------------------------------------------------------
	
	// TODO FRS Es necesario pasar a través del overlayManager
	// El ancho y el alto deberían ser cosas independientes de los overlays, no?
	int CServer::getScreenWidth(){
		int aux(_overlayManager->getViewportWidth());
		return aux;
	} //get Width
	//--------------------------------------------------------

	int CServer::getScreenHeight(){
		return _overlayManager->getViewportHeight();
	} //get Height
	//--------------------------------------------------------



	/***********
		TICK
	***********/

	void CServer::tick(float secs) 
	{
		if(_activeScene != _dummyScene)
			_activeScene->tick(secs);
		if(_root)
		{
			// Actualizamos todas las ventanas de reenderizado.
			Ogre::WindowEventUtilities::messagePump();
			// Reenderizamos un frame
			_root->renderOneFrame(secs);
		}
	} // tick

} // namespace Graphics
