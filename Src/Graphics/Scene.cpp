//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp

Contiene la implementación de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llansó
@date Julio, 2010
*/

#include "Scene.h"

#include <assert.h>

#include <HHFX/IHHFXPublic.h>
#include <HHFX/RendererSubView.h>

#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreStaticGeometry.h>
#include <OgreViewport.h>

#include <BaseSubsystems/Server.h>

#include "Light.h"
#include "Camera.h"
#include "Entity.h"
#include "GlowMaterialListener.h"
#include "SceneElement.h"



namespace Graphics 
{

	CScene::CScene(const std::string& name) : _name(name), _viewport(0), 
			_staticGeometry(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
		_baseCamera = new CCamera("base" + name, this);

		_initHHFXScene(); // Init Hell Heaven FX Scene
	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		delete _camera;
		_root->destroySceneManager(_sceneMgr);

	} // ~CScene

	//--------------------------------------------------------

	void CScene::activate()
	{
		buildStaticGeometry(); // FRS Se debe construir en cada activación?

		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
						->addViewport(_camera->getCamera());

		_camera->getCamera()->setAspectRatio(
        Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));

		_viewport->setBackgroundColour(Ogre::ColourValue::Black);

		Ogre::CompositorInstance* comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "Glow");
			comp->setEnabled(true);

		GlowMaterialListener *gml = new GlowMaterialListener();
		Ogre::MaterialManager::getSingletonPtr()->addListener(gml);

		/* PRUEBAS PEP */
		comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "BW");
			comp->setEnabled(false);
		
		/** Bloom CACA */
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, "Bloom", false);
		//Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, "Bloom", true);

		comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "RadialBlur");
			comp->setEnabled(false);
		//BWMaterialListener *bwml = new BWMaterialListener();
		//Ogre::MaterialManager::getSingletonPtr()->addListener(bwml);


		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.7f,0.7f,0.7f));

		// FRS Lo suyo sería introducirlas mediante un CShadows o algo asin + attachToScene 
		//Sombras Chulas - Consumen mucho*/
		//_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

		_loadHHFXCompositors(); // Hell Heaven FX 

	} // activate

	//--------------------------------------------------------

	void CScene::activateCompositor(std::string name)
	{	
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, name, true);
	}
	void CScene::deactivateCompositor(std::string name)
	{	
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, name, false);
	}
	void CScene::activateBaseCam()
	{
		buildStaticGeometry();
		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
						->addViewport(_baseCamera->getCamera());

		_baseCamera->getCamera()->setAspectRatio(
        Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));

		
		_viewport->setBackgroundColour(Ogre::ColourValue::Black);

		Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "Glow");
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, "Glow", true);

		GlowMaterialListener *gml = new GlowMaterialListener();
		Ogre::MaterialManager::getSingletonPtr()->addListener(gml);

		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.7f,0.7f,0.7f));

		// FRS Lo suyo sería introducirlas mediante un CShadows o algo asin + attachToScene 
		//Sombras Chulas - Consumen mucho*/
		//_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

	}


	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_viewport)
		{
			BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->
					removeViewport(_viewport->getZOrder());
			_viewport = 0;
		}
	} // deactivate
	
	//--------------------------------------------------------

	// FRS Sólo se actualizan las entidades dinámicas
	// Y en concreto, sólo implementa el tick el CAnimatedEntity : CEntity
	void CScene::tick(float secs)
	{	
		TEntities::const_iterator it = _dynamicEntities.begin();
		TEntities::const_iterator end = _dynamicEntities.end();
		for(; it != end; it++)
			(*it)->tick(secs);

	} // tick



	/************************
		SCENE ELEMENTS
	************************/

	//---------- GENERIC SCENE ELEMENTS (p.e. billboards, particles, etc)-----------

	bool CScene::add(CSceneElement* sceneElement) {		return sceneElement->attachToScene(this);	}
	void CScene::remove(CSceneElement* sceneElement) {	sceneElement->detachFromScene();			} 


	//---------- ENTITIES -------------------------

	bool CScene::add(CEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;

		else {
			entity->isStatic() ?
				_staticEntities.push_back(entity) :
				_dynamicEntities.push_back(entity);

			return true;
		}
	} // addEntity

	//--------------------------------------------------------

	void CScene::remove(CEntity* entity)
	{
		entity->detachFromScene();

		entity->isStatic() ?		
			_staticEntities.remove(entity) :
			_dynamicEntities.remove(entity);

	} // addEntity

	//--------------------------------------------------------

	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticEntities.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TEntities::const_iterator it = _staticEntities.begin();
			TEntities::const_iterator end = _staticEntities.end();
				for(; it != end; it++)
					(*it)->addToStaticGeometry();

			_staticGeometry->build();
		}

	} // buildStaticGeometry
	

	//---------- LIGHTS -------------------------

	bool CScene::add(CLight *light)
	{
		if(!light->attachToScene(this))
			return false;
		_lights.push_back(light);
		return true;

	} // addBillboard

	
	//--------------------------------------------------------

	void CScene::remove(CLight* light)
	{
		light->deattachFromScene();
		_lights.remove(light);
	} // removeBillboard




	/*********************
		HELL HEAVEN FX
	*********************/

	void CScene::_initHHFXScene() 
	{
		// set the default visibility flag for all the movable objects, because we will use posts effects that 
		// needs to filter objects for rendering
		Ogre::MovableObject::setDefaultVisibilityFlags(1); // TODO mover a Graphics Server?

		// retrieve the HellHeaven's scene from an empty fx. for each Ogre::SceneManager a HHFXScene is associated.
		Ogre::MovableObject	*dummyMO = _sceneMgr->createMovableObject("HHFX");
			if (dummyMO) {	
				_hhfxScene = &( static_cast<IHHFXOgre*>(dummyMO)->GetHHFXScene() );			
				_sceneMgr->destroyMovableObject(dummyMO); // we got the hh scene, destroy the dummy effect
			}
			assert(_hhfxScene && "failed creating HHFxXScene !");	
			
							
		// load a pack		
		bool hhfxPackLoaded = _hhfxScene->GetHHFXBase().LoadPack("media/packs/hhfx", true); // TODO FRS Verifica que esto tenga que hacerse para cada hhfxScene y que Base no sea comun a todos
		OgreAssert(hhfxPackLoaded, "hhfx pack did not load correctly or contains no effects !");
		// TODO Esto solo hace falta la primera vez, el resto no vuelve a recargar el packete
		// HACK FRS Fix esto con un const o leyendolo de mapa				

		// bind the collision callback (i.e. used by Rain.hfx)
//		m_hhfxScene->SetCollisionCallback(this, &_IntersectScene); TODO FRS
		_hhfxScene->SetWorldScale(8); // ?? TODO const float	kWorldFxScale = 1.0f;

		_root->addFrameListener(this);
	}

	//-------------------------------------------------------------------------------------

	void CScene::_loadHHFXCompositors() 
	{
		// adding compositor for post fx
		Ogre::CompositorInstance*	comp = Ogre::CompositorManager::getSingleton().addCompositor(_camera->getViewport(), "HellHeavenOgre/Compositor/Distortion");
			assert(comp && "[HHFX ERROR] Cannot load compositor Distortion !" );
		//	comp->setEnabled(true);
	}


	//-----------FRAME LISTENER IMPL--------------------------------------------------------------------------

	bool CScene::frameStarted(const Ogre::FrameEvent& evt)
	{
		_hhfxScene->Update(evt.timeSinceLastFrame); // update the hhfx scene
		return true;
	}

	//-------------------------------------------------------------------------------------

	bool CScene::frameRenderingQueued(const Ogre::FrameEvent& evt)
	{	
		const Vector3& camPos = _camera->getCameraPosition();
		const Quaternion& camOri = _camera->getCameraOrientation();

		Matrix4 worldTransforms;
			worldTransforms.makeTransform(camPos, Vector3::UNIT_SCALE, camOri);
			worldTransforms = worldTransforms.transpose(); // FRS Transformación de cámara se aplica con la M transpuesta

		HellHeaven::CRendererSubView view; // setting render params for the particles renderers
			view.setHHFXScene(*_hhfxScene);
			view.setViewMatrix(worldTransforms);
			view.setSceneManager(*_sceneMgr);

		_hhfxScene->Render(view, camPos);

		return  true;
	}

	

} // namespace Graphics
