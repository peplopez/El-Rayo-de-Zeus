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

#include <assert.h>
#include <BaseSubsystems/Server.h>
#include <BaseSubsystems/Math.h>
#include <HHFX/IHHFXPublic.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayManager.h>
#include <OgreWindowEventUtilities.h>

#include "Scene.h"
#include "Overlay.h"



#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "GRAPHICS::SERVER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

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
		
		_overlayManager = Ogre::OverlayManager::getSingletonPtr();//PT. Se carga el manager de overlays

		_dummyScene = createScene("dummy_scene"); // Creamos la escena dummy para cuando no hay ninguna activa.		
				
		_initHHFX(); // Hell Heaven FX
		
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

	
	//-------------------------------------------------------------------------------------



	/*********************
		HELL HEAVEN FX
	********************/

	void CServer::_initHHFX() 
	{
		_preloadHHFXTextures();

		// set the default visibility flag for all the movable objects, because we will use posts effects that 
		// needs to filter objects for rendering
		Ogre::MovableObject::setDefaultVisibilityFlags(1); 

		// retrieve the HellHeaven's scene from an empty fx. for each Ogre::SceneManager a HHFXScene is associated.
		Ogre::MovableObject	*dummyMO = _dummyScene->getSceneMgr()->createMovableObject("HHFX");
			if (dummyMO) {	
				_hhfxBase =  &static_cast<IHHFXOgre*>(dummyMO)->GetHHFXScene().GetHHFXBase();			
				_dummyScene->getSceneMgr()->destroyMovableObject(dummyMO); // we got the hh scene, destroy the dummy effect
			}
			assert(_hhfxBase && "failed initialing HHFX !");
							
		// load a pack		
		bool hhfxPackLoaded = _hhfxBase->LoadPack("media/packs/hhfx", true); 
		OgreAssert(hhfxPackLoaded, "hhfx pack did not load correctly or contains no effects !");
	}


	// HACK FRS Dar un repaso y evaluar si se están cargando de antes, si este es el lugar apropiado, etc
	void CServer::_preloadHHFXTextures() 
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		// preload the distortion texture with hardware gamma correction
		/////////////////////////////////////////////////////////////////////////////////////////

		LOG("[HHFX] ---------- preload texture hardware gamma ----------");

		// ok then, load all the textures in the HellHeaven resource group
		std::string resourceGroup("HellHeaven");

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(resourceGroup);

		Ogre::StringVectorPtr strVectorPtr = Ogre::ResourceGroupManager::getSingleton().listResourceNames(resourceGroup);

		for (unsigned int i = 0; i < strVectorPtr->size(); ++i)
		{
			std::string texName = strVectorPtr->at(i);

			try
			{
				LOG("[HHFX] Trying to load " << texName << " as texture...");

				// should load all the textures with gamma correction 
				// except the ones used for distortion post effect
				if (!texName.compare("ParticleDeformBlur_01.dds") ||
					!texName.compare("RainDeform_01.dds") ||
					!texName.compare("RainDropsDeform_01.dds"))
				{
					Ogre::TexturePtr pTex = Ogre::TextureManager::getSingleton().load(
																			texName, resourceGroup,
																			Ogre::TEX_TYPE_2D, Ogre::MIP_DEFAULT,
																			1.0f, false, Ogre::PF_UNKNOWN, false);
				}
				else
				{
					// gamma corrected
					Ogre::TexturePtr pTex = Ogre::TextureManager::getSingleton().load(
																			texName, resourceGroup,
																			Ogre::TEX_TYPE_2D, Ogre::MIP_DEFAULT,
																			1.0f, false, Ogre::PF_A8R8G8B8, true);
				}
			}
			catch (Ogre::Exception e) // texture not loaded because it surely is not one
			{
				continue;
			}
		}

		LOG("[HHFX] ---------- done ----------");
	}




} // namespace Graphics
