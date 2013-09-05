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

#include <BaseSubsystems/Server.h>
#include <HHFX/IHHFXPublic.h>
#include <HHFX/RendererSubView.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreStaticGeometry.h>
#include <OgreViewport.h>

#include "Light.h"
#include "Camera.h"
#include "Entity.h"
#include "GlowMaterialListener.h"
#include "SceneElement.h"
#include "Server.h"

#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "GRAPHICS::SCENE>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif




namespace Graphics 
{
	// HACK WARNING : if != 1.0 el rendimiento empeora -> rehacer scripts .hfx
	// This scale must be taken into account when setting and getting arbitrary particle attributes !
	const float CScene::HHFX_WORLD_SCALE = 8.0f;

	CScene::CScene(const std::string& name) : _name(name), _isInit(false), _viewport(0), 
		_staticGeometry(0), _hhfxScene(0), _hhfxTimeSinceUpdate(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);	
		_playerCamera = new CCamera(name,this);
		_baseCamera = new CCamera("base" + name, this);
		_hhfxScene = _hhfxCreateScene(_sceneMgr);
		_viewport = Graphics::CServer::getSingletonPtr()->getViewport(); //no hay que liberarlo en el destructor -> se encarga CServer
				
	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{		
		deactivate();
		delete _baseCamera; // FRS andaba sin liberar
		delete _playerCamera;		
		_root->destroySceneManager(_sceneMgr);
	} // ~CScene
	

	//--------------------------------------------------------
	
	// FRS parte del activate() que solo debe ejecutarse en la primera activación
	void CScene::_init() {
		assert(!_isInit && "Escena ya inicializada previamente");
		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.7f,0.7f,0.7f));
		_buildStaticGeometry();		
		_hhfxInit(); // Init Hell Heaven FX Scene
		_playerCamera->getCamera()->setAutoAspectRatio(true);
		_baseCamera->getCamera()->setAutoAspectRatio(true);
		_isInit = true;		
	} // init

	//--------------------------------------------------------

	void CScene::_deinit() {
		assert(_isInit && "Escena no inicializada previamente");
		_hhfxDeinit(); // Hell Heaven FX	
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		_isInit = false;
	} // deinit

	//--------------------------------------------------------

	void CScene::activate()
	{		
		if(!_isInit) _init();
	} // deactivate

	//--------------------------------------------------------

	void CScene::deactivate()
	{		
		if(_isInit) _deinit();
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

	//--------------------------------------------------------



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

	
	//--------------------------------------------------------

	void CScene::_buildStaticGeometry()
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



	/*********************
		HELL HEAVEN FX
	*********************/


	//------------ INIT & DEINIT -------------------------------------------------------------------------

	IHHFXScene* CScene::_hhfxCreateScene(Ogre::SceneManager* sceneMgr) 
	{
		IHHFXScene* hhfxScene = 0;

		// retrieve the HellHeaven's scene from an empty fx. for each Ogre::SceneManager a HHFXScene is associated.
		Ogre::MovableObject	*dummyMO = sceneMgr->createMovableObject("HHFX");
			if (dummyMO) {	
				hhfxScene = &( static_cast<IHHFXOgre*>(dummyMO)->GetHHFXScene() );			
				sceneMgr->destroyMovableObject(dummyMO); // we got the hh scene, destroy the dummy effect
			}

		assert(hhfxScene && "failed creating HHFXScene !");	
		return hhfxScene;
	}

	//-------------------------------------------------------------------------------------

	void CScene::_hhfxInit() 
	{	
		assert(_hhfxScene && "[HHFX] Scene NULL!");

		// Solo si no somos dummy scene
		if(_name != "dummy_scene") {
			_hhfxScene->SetCollisionCallback(this, &_hhfxCollisionCheck);  		// bind the collision callback 
			_hhfxScene->SetWorldScale( HHFX_WORLD_SCALE ); 
			_root->addFrameListener(this); // FRS oyentes de eventos de FrameRender de Ogre
			// FRS os mantenemos como oyentes aunque la escena no esté activa para que los FX sigan ejecutando el update 
			// y así el regreso a la escena no muestre cambios bruscos.
		}
		
	} // _hhfxInit

	//-------------------------------------------------------------------------------------

	void CScene::_hhfxDeinit() 
	{	
		_hhfxScene->Clear(); // clear the scene before shutting down ogre since the hhfx ogre implementation holds some Ogre objects.
		_root->removeFrameListener(this); // FRS Nos borramos como oyentes de eventos de FrameRender de Ogre
	} // _hhfxSceneDeinit


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
		if(_viewport || _hhfxTimeSinceUpdate > _HHFX_INACTIVE_UPDATE_PERIOD)
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

		// Aplicamos transformaciones de la camara que esté renderizando actualmente el FX: PlayerCamera o BaseCamera.
		Ogre::Camera* fxCamera = _viewport->getCamera();
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

	

} // namespace Graphics
