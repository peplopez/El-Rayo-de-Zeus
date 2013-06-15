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

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "GRAPHICS::SCENE>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Graphics 
{
	// WARNING : if != 1.0, This scale must be taken into account when setting and getting arbitrary particle attributes !
	const float CScene::HHFX_WORLD_SCALE = 5.0f;

	CScene::CScene(const std::string& name) : _name(name), _viewport(0), 
			_staticGeometry(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
		_baseCamera = new CCamera("base" + name, this);

		_hhfxSceneSetup(); // Init Hell Heaven FX Scene
	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();		
		
		// Hell Heaven FX 
		_hhfxScene->Clear(); // clear the scene before shutting down ogre since the hhfx ogre implementation holds some Ogre objects.
		_hhfxCompositorUnload(); 

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

		comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "RadialBlur");
			comp->setEnabled(false);
		//BWMaterialListener *bwml = new BWMaterialListener();
		//Ogre::MaterialManager::getSingletonPtr()->addListener(bwml);


		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.7f,0.7f,0.7f));

		// FRS Lo suyo sería introducirlas mediante un CShadows o algo asin + attachToScene 
		//Sombras Chulas - Consumen mucho*/
		//_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

		_hhfxCompositorLoad(); // Hell Heaven FX 

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
			activateCompositor("Glow");

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

	void CScene::_hhfxSceneSetup() 
	{	
		// retrieve the HellHeaven's scene from an empty fx. for each Ogre::SceneManager a HHFXScene is associated.
		Ogre::MovableObject	*dummyMO = _sceneMgr->createMovableObject("HHFX");
			if (dummyMO) {	
				_hhfxScene = &( static_cast<IHHFXOgre*>(dummyMO)->GetHHFXScene() );			
				_sceneMgr->destroyMovableObject(dummyMO); // we got the hh scene, destroy the dummy effect
			}
			assert(_hhfxScene && "failed creating HHFXScene !");	

		// bind the collision callback 
		_hhfxScene->SetCollisionCallback(this, &_hhfxCollisionCheck); 
		_hhfxScene->SetWorldScale( HHFX_WORLD_SCALE ); 

		_root->addFrameListener(this);
	}

	//-------------------------------------------------------------------------------------

	void CScene::_hhfxCompositorLoad() 
	{
		// adding compositor for post fx
		Ogre::CompositorInstance*	comp = Ogre::CompositorManager::getSingleton().addCompositor(_camera->getViewport(), "HellHeavenOgre/Compositor/Distortion");
			assert(comp && "[HHFX ERROR] Cannot load compositor Distortion !" );
			comp->setEnabled(true);
	}

	//-------------------------------------------------------------------------------------

	void CScene::_hhfxCompositorUnload() 
	{
		// remove our compositor
		Ogre::CompositorManager::getSingleton().removeCompositor(_camera->getViewport(), "HellHeavenOgre/Compositor/Distortion");	
	}

	//-------------------------------------------------------------------------------------

	// HACK FRS Esto hay que migrarlo a LOGIC -> map? y usar los valores correctos
	// TODO FRS Habría que tener en cuenta colisiones con planos superiores (por fuerzas no gravitatorias?)
	bool CScene::_hhfxCollisionCheck(void *arg, const Ogre::Vector3 &start, 
		const Ogre::Vector3 &direction, float length, SContactReport &contactReport)
	{
		//LOG("Particle Position = " << start.x << " , " << start.y << " , " << start.z)

		// CONSTANTS
		static const int RING_HEIGHT = 50;
		static const int RING_HEIGHT_OFFSET = 1.4;
		static const Ogre::Plane RING_PLANES[] = { 
			Ogre::Plane(	Vector3::UNIT_Y,  RING_HEIGHT + RING_HEIGHT_OFFSET), 
			Ogre::Plane(	Vector3::UNIT_Y,			    RING_HEIGHT_OFFSET), 	
			Ogre::Plane(	Vector3::UNIT_Y, -RING_HEIGHT + RING_HEIGHT_OFFSET)
		};
		
		//TODO CServer::getSingletonPtr()->getRingRadio(ring)
		static const int RING_WIDTH = 50; // TODO FRS Si los anillos están escalados, hay que hacer 3 versiones
		static const int RING_RAD[] = {58, 73, 58};
		static const int RING_RAD_SQR[] = { RING_RAD[0] * RING_RAD[0], 
											RING_RAD[1] * RING_RAD[1],
											RING_RAD[2] * RING_RAD[2]}; 
		static const int RAD_SQR_MIN[] = { 
			(RING_RAD[0] - RING_WIDTH/2) * (RING_RAD[0] - RING_WIDTH/2),
			(RING_RAD[1] - RING_WIDTH/2) * (RING_RAD[1] - RING_WIDTH/2), 
			(RING_RAD[2] - RING_WIDTH/2) * (RING_RAD[2] - RING_WIDTH/2)}; 
		static const int RAD_SQR_MAX[] = {
			(RING_RAD[0] + RING_WIDTH/2) * (RING_RAD[0] + RING_WIDTH/2),
			(RING_RAD[1] + RING_WIDTH/2) * (RING_RAD[1] + RING_WIDTH/2), 
			(RING_RAD[2] + RING_WIDTH/2) * (RING_RAD[2] + RING_WIDTH/2)}; 

		int ringIndex = 0;
		for(; ringIndex < 3 && start.y < -RING_PLANES[ringIndex].d; ++ringIndex); // d = -height
			if(ringIndex == 3)
				return false; // Por debajo del anillo inferior -> Imposible ninguna colisión por efecto gravitatorio
				
		// CHECK RING INTERSECT
		Ray	traceRay(start, direction);
		std::pair<bool, Ogre::Real> result = traceRay.intersects( RING_PLANES[ringIndex] ); 

		if (result.first) { // Correcting Result 

			contactReport.m_Point = traceRay.getPoint(result.second);
			float contactSqrDist =	contactReport.m_Point.x * contactReport.m_Point.x +
									contactReport.m_Point.z * contactReport.m_Point.z;
			
			// stay on our "visible" ring
			if (contactSqrDist > RAD_SQR_MIN[ringIndex] && contactSqrDist < RAD_SQR_MAX[ringIndex]) {						
				contactReport.m_Time = result.second;
				contactReport.m_Normal = RING_PLANES[ringIndex].normal;			
			} else {
				result.first = false;
			}
		}

		return result.first;
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
		const Vector3& camPos = _camera->getPosition();
		const Quaternion& camOri = _camera->getOrientation();

		Matrix4 worldTransforms;
			worldTransforms.makeTransform(camPos, Vector3::UNIT_SCALE, camOri);
			worldTransforms = worldTransforms.transpose(); // FRS Transformación de cámara se aplica con la M transpuesta

		HellHeaven::CRendererSubView view; // setting render params for the particles renderers
			view.setHHFXScene(*_hhfxScene);
			view.setViewMatrix(worldTransforms);
			view.setSceneManager(*_sceneMgr);
			view.setUsePostFX(true); // FRS no se si sirve realmente para nada

		_hhfxScene->Render(view, camPos);

		return  true;
	}

	

} // namespace Graphics
