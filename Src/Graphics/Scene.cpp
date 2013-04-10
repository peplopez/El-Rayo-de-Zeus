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

#include "BaseSubsystems/Server.h"

#include "Graphics/Light.h"
#include "Graphics/Camera.h"
#include "Graphics/Entity.h"
#include "Graphics/GlowMaterialListener.h"
#include "Graphics/Server.h"
#include "Graphics/SceneElement.h"

#include "Logic/Server.h"

#include <assert.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreStaticGeometry.h>


//#include <OgreColourValue.h>

//PT
//#include <OgreParticleSystem.h> // TODO FRS Por desvincular (al igual que billboardSet)
//#include <OgreCompositorManager.h>


namespace Graphics 
{

	CScene::CScene(const std::string& name) : _name(name), _viewport(0), 
			_staticGeometry(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
		_baseCamera = new CCamera("base" + name, this);
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

		Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, "Glow");
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, "Glow", true);

		GlowMaterialListener *gml = new GlowMaterialListener();
		Ogre::MaterialManager::getSingletonPtr()->addListener(gml);

		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.7f,0.7f,0.7f));

		// FRS Lo suyo sería introducirlas mediante un CShadows o algo asin + attachToScene 
		//Sombras Chulas - Consumen mucho*/
		//_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

	} // activate

	//--------------------------------------------------------
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

	//---------- GENERIC SCENE ELEMENTS (p.e. billboards, parcticles, etc)-----------

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
	//--------------------------------------------------------

		entity->isStatic() ?		
			_staticEntities.remove(entity) :
			_dynamicEntities.remove(entity);

	} // addEntity

	
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

	//-------------------PARTICLES------------------

	// TODO FRS quizá sea necesario temporizar las particulas sin padre para borrar su nodo
	// TODO FRS estamos limitando a una partícula por entidad

	// POS. RELATIVA (particulas hijas de otra entidad gráfica)
	void CScene::createParticleSystem(const std::string& templateName, const std::string& parentEntity) 
	{
		assert( getSceneMgr()->hasSceneNode( parentEntity + "_node") && "No existe la entidad de referencia" ); 
				
		_sceneMgr->getSceneNode( parentEntity + "_node")->attachObject( 
			_sceneMgr->createParticleSystem(parentEntity + "_ps", templateName) // Suponemos un único PS por entidad
		);
	}

	// POSICIÓN ABSOLUTA
	void CScene::createParticleSystem(const std::string& templateName, const Vector3& position) 
	{		 		
		 _sceneMgr->getRootSceneNode()
				->createChildSceneNode(position)
				->attachObject( 
			_sceneMgr->createParticleSystem("_ps", templateName)
		);
	}

	// TODO FRS DestructorSi fuera necesario...
		// Desvinculamos el sistema de partículas del nodo
		/*
		sceneNode->detachObject(pssmoke); 
		// Destruimos el nodo
		_sceneMgr->destroySceneNode(sceneNode); 
		// Destruimos el sistema de partículas
		_sceneMgr->destroyParticleSystem(pssmoke);
		*/
	

} // namespace Graphics
