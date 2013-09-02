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

#include <SkyX.h>

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

#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "GRAPHICS::SCENE>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Graphics 
{
	// WARNING : if != 1.0, This scale must be taken into account when setting and getting arbitrary particle attributes !
	const float CScene::HHFX_WORLD_SCALE = 8.0f;

	CScene::CScene(const std::string& name) : _name(name), _viewport(0), 
		_staticGeometry(0), _hhfxScene(0), _hhfxTimeSinceUpdate(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, name);
		_camera = new CCamera(name,this);
		_baseCamera = new CCamera("base" + name, this);

		//SkyX init
		if(_name != "dummy_scene") {
			_skyXBasicController = new SkyX::BasicController();
			_skyX = new SkyX::SkyX(_sceneMgr, _skyXBasicController);
			_skyX->create();
			_root->addFrameListener(_skyX);
			BaseSubsystems::CServer::getSingletonPtr()
				->getRenderWindow()->addListener(_skyX);
			_skyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,-1));

		}

		_hhfxSceneInit(); // Init Hell Heaven FX Scene
	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		_root->removeFrameListener(_skyX);
		BaseSubsystems::CServer::getSingletonPtr()
				->getRenderWindow()->removeListener(_skyX);
		deactivate();	
		_hhfxSceneDeinit(); // Hell Heaven FX		
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		delete _camera;
		_root->destroySceneManager(_sceneMgr);
			
	} // ~CScene

	
	//--------------------------------------------------------

	void CScene::activateCompositor(std::string name)
	{	
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, name, true);
	}	
	//--------------------------------------------------------

	void CScene::deactivateCompositor(std::string name)
	{	
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, name, false);
	}	
	//--------------------------------------------------------

	void CScene::activateBaseCam()
	{
		buildStaticGeometry(); // FRS Internamente sólo se ejecuta en el primer uso de la escena ->
		// analizar si más conveniente hacerlo en un paso previo a los dos tipos de activate

		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()
				->getRenderWindow()->addViewport(_baseCamera->getCamera());
			_viewport->setBackgroundColour(Ogre::ColourValue::Black);
		
		_baseCamera->getCamera()->setAspectRatio(
			Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));
		
		/*
		Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "Glow");
			activateCompositor("Glow");

		GlowMaterialListener *gml = new GlowMaterialListener(); // FRS y este new? no se pierde en el limbo? WTF?
		Ogre::MaterialManager::getSingletonPtr()->addListener(gml);
		*/

		// FRS esto también, quizá mejor en un paso previo, junto con todas las sentencias repetidas en los activates
		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.7f,0.7f,0.7f)); 

		// FRS Lo suyo sería introducirlas mediante un CShadows o algo asin + attachToScene 
		//Sombras Chulas - Consumen mucho*/
		//_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

	}
	
	//--------------------------------------------------------



	/******************
		ICOMPONENT
	******************/

	// TODO FRS Este activate hay que repasarlo, está bien hacer todo esto en cada activate?
	void CScene::activate()
	{
		buildStaticGeometry(); // FRS Se debe construir en cada activación?

		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->addViewport(_camera->getCamera());

		_camera->getCamera()->setAspectRatio( // FRS Esto hay que hacerlo en cada activate?
			Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));

		_viewport->setBackgroundColour(Ogre::ColourValue::Black);

		/*Ogre::CompositorInstance* comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "Glow");
			comp->setEnabled(true);

		GlowMaterialListener *gml = new GlowMaterialListener();
		Ogre::MaterialManager::getSingletonPtr()->addListener(gml);*/

		/* PRUEBAS PEP */
		/*Ogre::CompositorInstance* comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "BW");
			comp->setEnabled(false);

		/*comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "RadialBlur");
			comp->setEnabled(false);*/
		//BWMaterialListener *bwml = new BWMaterialListener();
		//Ogre::MaterialManager::getSingletonPtr()->addListener(bwml);

		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.7f,0.7f,0.7f));

		// FRS Lo suyo sería introducirlas mediante un CShadows o algo asin + attachToScene 
		//Sombras Chulas - Consumen mucho*/
		//_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

		_hhfxCompositorLoad(); // Hell Heaven FX 
		if(_name != "dummy_scene") 
			setSkyXPreset(_skyXPresets[1]);

	} // activate

	
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
		TSceneElements::const_iterator it = _dynamicElements.begin();
		TSceneElements::const_iterator end = _dynamicElements.end();
		for(; it != end; ++it)
			(*it)->tick(secs);

	} // tick



	/************************
		SCENE ELEMENTS
	************************/

	//---------- GENERIC SCENE ELEMENTS (p.e. billboards, particles, entities etc)-----------

	bool CScene::add(CSceneElement* sceneElement) {	

		if(!sceneElement->attachToScene(this))
			return false;
		
		switch( sceneElement->getType() )
		{
		case TGraphicalType::STATIC:
			_staticElements.push_back(sceneElement);
			break;
		case TGraphicalType::DYNAMIC:
			_dynamicElements.push_back(sceneElement);
			break;
		}
		return true;

	}// addSceneElement

	//----------------------------------------------------------

	void CScene::remove(CSceneElement* sceneElement) {	

		sceneElement->detachFromScene();		

		switch( sceneElement->getType() )
		{
		case TGraphicalType::STATIC:
			_staticElements.remove(sceneElement);
			break;
		case TGraphicalType::DYNAMIC:
			_dynamicElements.remove(sceneElement);
			break;
		}	
	} // removeSceneElement


	//--------------------------------------------------------

	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticElements.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TSceneElements::const_iterator it = _staticElements.begin();
			TSceneElements::const_iterator end = _staticElements.end();
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

	

	//------------ INIT & DEINIT -------------------------------------------------------------------------

	void CScene::_hhfxSceneInit() 
	{	
		// retrieve the HellHeaven's scene from an empty fx. for each Ogre::SceneManager a HHFXScene is associated.
		Ogre::MovableObject	*dummyMO = _sceneMgr->createMovableObject("HHFX");
			if (dummyMO) {	
				_hhfxScene = &( static_cast<IHHFXOgre*>(dummyMO)->GetHHFXScene() );			
				_sceneMgr->destroyMovableObject(dummyMO); // we got the hh scene, destroy the dummy effect
			}
			assert(_hhfxScene && "failed creating HHFXScene !");	
			
		// Solo si no somos dummy scene
		if(_name != "dummy_scene") {
			_hhfxScene->SetCollisionCallback(this, &_hhfxCollisionCheck);  		// bind the collision callback 
			_hhfxScene->SetWorldScale( HHFX_WORLD_SCALE ); 
			_root->addFrameListener(this);
		}
		
	}

	//-------------------------------------------------------------------------------------

	void CScene::_hhfxSceneDeinit() 
	{
		// Hell Heaven FX 
		_hhfxScene->Clear(); // clear the scene before shutting down ogre since the hhfx ogre implementation holds some Ogre objects.
		_root->removeFrameListener(this); // FRS Nos borramos como oyentes de eventos de FrameRender de Ogre
		//_hhfxCompositorUnload(); // UNDONE FRS Los compositors no se descargan de mem?		
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




	//----------- COLLISION CALLBACK --------------------------------------------------------------------------

	// HACK FRS Esto hay que migrarlo a LOGIC -> map? y usar los valores correctos
	// TODO FRS Habría que tener en cuenta colisiones con planos superiores (por fuerzas no gravitatorias?)
	bool CScene::_hhfxCollisionCheck(void *arg, const Ogre::Vector3 &start, 
		const Ogre::Vector3 &direction, float length, SContactReport &contactReport)
	{
	//	LOG("Particle Position = " << start.x << " , " << start.y << " , " << start.z)

		// CONSTANTS    					//TODO CServer::getSingletonPtr()->getRingRadio(ring)trasnslate		
		static const int RING_HEIGHT = 100;
		static const int RING_HEIGHT_OFFSET = 2;
		static const int RING_RAD[]		= {58, 73, 58};
		static const int RING_WIDTH[]	= {50, 75, 50};
		static const int	RING_WIDTH_INT[] = {   RING_WIDTH[0] / 3,    RING_WIDTH[1] / 4,	0.4* RING_WIDTH[2]};
		static const int	RING_WIDTH_EXT[] = {2* RING_WIDTH[0] / 3, 3* RING_WIDTH[1] / 4, 0.6* RING_WIDTH[2]};

		static const Ogre::Plane RING_PLANES[] = { 
			Ogre::Plane(	Vector3::UNIT_Y,  RING_HEIGHT + RING_HEIGHT_OFFSET), 
			Ogre::Plane(	Vector3::UNIT_Y,			    RING_HEIGHT_OFFSET), 	
			Ogre::Plane(	Vector3::UNIT_Y, -RING_HEIGHT + RING_HEIGHT_OFFSET)
		};
		static const int RING_RAD_SQR[] = { RING_RAD[0] * RING_RAD[0], 
											RING_RAD[1] * RING_RAD[1],
											RING_RAD[2] * RING_RAD[2]}; 
		static const int RAD_SQR_MIN[] = { 
			pow( (float) RING_RAD[0] - RING_WIDTH_INT[0], 2), // TODO FRS posiblemente rinda más multiplicarlo por sí mismo explicitamente
			pow( (float) RING_RAD[1] - RING_WIDTH_INT[1], 2),
			pow( (float) RING_RAD[2] - RING_WIDTH_INT[2], 2)}; 
		static const int RAD_SQR_MAX[] = {
			pow( (float) RING_RAD[0] + RING_WIDTH_EXT[0], 2),
			pow( (float) RING_RAD[1] + RING_WIDTH_EXT[1], 2), 
			pow( (float) RING_RAD[2] + RING_WIDTH_EXT[2], 2)}; 

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
				if (contactSqrDist < RAD_SQR_MAX[ringIndex] && contactSqrDist > RAD_SQR_MIN[ringIndex]) {						
					contactReport.m_Time = result.second;
					contactReport.m_Normal = RING_PLANES[ringIndex].normal;			
				} else {
					result.first = false;
				}
			}

		return result.first;
	}



	//-----------FRAME LISTENER IMPL--------------------------------------------------------------------------

	// TODO FRS _hhfxTimeSinceUpdate podría desbordarse si no hay particulas...
	bool CScene::frameStarted(const Ogre::FrameEvent& evt)
	{	
		LOG("[HHFX] ParticleCount = " << _hhfxScene->GetParticleCount() )

		_hhfxTimeSinceUpdate += evt.timeSinceLastFrame;
		if(_viewport || _hhfxTimeSinceUpdate > _HHFX_UPDATE_TIME_MAX)
			// && _hhfxScene->GetParticleCount() )  UNDONE  siempre devuelve 0 WTF!
		{
			LOG("["<< _name <<"] Frame Started: Time Since Update = " << _hhfxTimeSinceUpdate)
			_hhfxScene->Update(_hhfxTimeSinceUpdate); // update the hhfx scene
			_hhfxTimeSinceUpdate = 0;
		} 
		return true;
	}

	//-------------------------------------------------------------------------------------

	// FRS Return True to continue rendering, false to drop out of the rendering loop.
	bool CScene::frameRenderingQueued(const Ogre::FrameEvent& evt)
	{	
		if(_hhfxTimeSinceUpdate) // Si no se acaba de hacer Update (time = 0) no renderizamos
			return true;

		// Aplicamos transformaciones de la camara que esté renderizando actualmente el FX: Camera o BaseCamera.
		Ogre::Camera* fxCamera = _viewport? _viewport->getCamera() : _camera->getCamera();
		const Vector3& camPos =		fxCamera->getParentNode()->getPosition();	  // El nodo es el que contiene el transform
		const Quaternion& camOri =	fxCamera->getParentNode()->getOrientation(); // la camara mantiene pos = 0 (relativa al nodo)
	
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

	//-------------------------------------------------------------------------------------
	 
		SkyXSettings CScene::_skyXPresets[6] = {
			// Sunset
			SkyXSettings(Ogre::Vector3(8.85f, 7.5f, 20.5f),  -0.08f, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30, Ogre::Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3, 4), false, true, 300, false, Ogre::Radian(270), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0.35, 0.2, 0.92, 0.1), Ogre::Vector4(0.4, 0.7, 0, 0), Ogre::Vector2(0.8,1)),
			// Clear
			SkyXSettings(Ogre::Vector3(17.16f, 7.5f, 20.5f), 0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0017f, 0.000675f, 30, Ogre::Vector3(0.57f, 0.54f, 0.44f), -0.991f, 2.5f, 4), false),
			// Thunderstorm 1
			SkyXSettings(Ogre::Vector3(12.23, 7.5f, 20.5f),  0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 1, 4), false, true, 300, false, Ogre::Radian(0), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0.25, 0.4, 0.5, 0.1), Ogre::Vector4(0.45, 0.3, 0.6, 0.1), Ogre::Vector2(1,1), true, 0.5, Ogre::Vector3(1,0.976,0.92), 2),
			// Thunderstorm 2
			SkyXSettings(Ogre::Vector3(10.23, 7.5f, 20.5f),  0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 0.5, 4), false, true, 300, false, Ogre::Radian(0), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0, 0.02, 0.34, 0.24), Ogre::Vector4(0.29, 0.3, 0.6, 1), Ogre::Vector2(1,1), true, 0.5, Ogre::Vector3(0.95,1,1), 2),
			// Desert
			SkyXSettings(Ogre::Vector3(7.59f, 7.5f, 20.5f), 0, -0.8f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0072f, 0.000925f, 30, Ogre::Vector3(0.71f, 0.59f, 0.53f), -0.997f, 2.5f, 1), true),
			// Night
			SkyXSettings(Ogre::Vector3(21.5f, 7.5, 20.5), 0.03, -0.25, SkyX::AtmosphereManager::Options(), true)
	};

	//-------------------------------------------------------------------------------------

	void CScene::setSkyXPreset(const SkyXSettings& preset)
	{
		_skyX->setTimeMultiplier(preset.timeMultiplier);
		_skyXBasicController->setTime(preset.time);
		_skyXBasicController->setMoonPhase(preset.moonPhase);
		_skyX->getAtmosphereManager()->setOptions(preset.atmosphereOpt);

		// Layered clouds
		if (preset.layeredClouds)
		{
			// Create layer cloud
			if (_skyX->getCloudsManager()->getCloudLayers().empty())
			{
				_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
			}
		}
		else
		{
			// Remove layer cloud
			if (!_skyX->getCloudsManager()->getCloudLayers().empty())
			{
				_skyX->getCloudsManager()->removeAll();
			}
		}

		_skyX->getVCloudsManager()->setWindSpeed(preset.vcWindSpeed);
		_skyX->getVCloudsManager()->setAutoupdate(preset.vcAutoupdate);

		SkyX::VClouds::VClouds* vclouds = _skyX->getVCloudsManager()->getVClouds();

		vclouds->setWindDirection(preset.vcWindDir);
		vclouds->setAmbientColor(preset.vcAmbientColor);
		vclouds->setLightResponse(preset.vcLightResponse);
		vclouds->setAmbientFactors(preset.vcAmbientFactors);
		vclouds->setWheater(preset.vcWheater.x, preset.vcWheater.y, false);

		if (preset.volumetricClouds)
		{
			// Create VClouds
			if (!_skyX->getVCloudsManager()->isCreated())
			{
				// SkyX::MeshManager::getSkydomeRadius(...) works for both finite and infinite(=0) camera far clip distances
				_skyX->getVCloudsManager()->create(_skyX->getMeshManager()->getSkydomeRadius(_viewport->getCamera()));
			}
		}
		else
		{
			// Remove VClouds
			if (_skyX->getVCloudsManager()->isCreated())
			{
				_skyX->getVCloudsManager()->remove();
			}
		}

		vclouds->getLightningManager()->setEnabled(preset.vcLightnings);
		vclouds->getLightningManager()->setAverageLightningApparitionTime(preset.vcLightningsAT);
		vclouds->getLightningManager()->setLightningColor(preset.vcLightningsColor);
		vclouds->getLightningManager()->setLightningTimeMultiplier(preset.vcLightningsTM);


		_skyX->update(0);
	}

	

} // namespace Graphics
