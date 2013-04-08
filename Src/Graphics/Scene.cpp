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

#include "Graphics/Billboard.h"
#include "Graphics/Light.h"
#include "Graphics/Camera.h"
#include "Graphics/Entity.h"
#include "Graphics/GlowMaterialListener.h"
#include "Graphics/Server.h"
#include "Graphics/StaticEntity.h"

#include "Logic/Server.h"

#include <assert.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreStaticGeometry.h>
#include <OgreColourValue.h>

//PT
#include <OgreParticleSystem.h> // TODO FRS Por desvincular (al igual que billboardSet)
#include <OgreCompositorManager.h>


namespace Graphics 
{

	CScene::CScene(const std::string& name) : _name(name), _viewport(0), 
			_staticGeometry(0), counterParticles(0)
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
		buildStaticGeometry();
		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
						->addViewport(_camera->getCamera());

		
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
		GRAPHICAL ELEMENTS
	************************/


	//---------- ENTITIES -------------------------

	bool CScene::add(CEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_dynamicEntities.push_back(entity);
		return true;

	} // addEntity

	//--------------------------------------------------------

	void CScene::remove(CEntity* entity)
	{
		entity->deattachFromScene();
		_dynamicEntities.remove(entity);
	} // addEntity



	//----------STATIC ENTITIES-------------------------

	bool CScene::add(CStaticEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_staticEntities.push_back(entity);
		return true;

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::remove(CStaticEntity* entity)
	{
		entity->deattachFromScene();
		_staticEntities.remove(entity);

	} // addStaticEntity


	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticEntities.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TStaticEntities::const_iterator it = _staticEntities.begin();
			TStaticEntities::const_iterator end = _staticEntities.end();
			for(; it != end; it++)
				(*it)->addToStaticGeometry();

			_staticGeometry->build();
		}

	} // buildStaticGeometry



	//---------- BILLBOARDS -------------------------

	bool CScene::add(CBillboard* billboard)
	{
		if(!billboard->attachToScene(this))
			return false;
		_billboards.push_back(billboard);
		return true;

	} // addBillboard

	
	//--------------------------------------------------------

	void CScene::remove(CBillboard* billboard)
	{
		billboard->deattachFromScene();
		_billboards.remove(billboard);
	} // removeBillboard


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

	//PT. Eliminacion de un sceneNode
	void CScene::deleteSceneNode(const std::string &name)
	{
		if(_sceneMgr->hasSceneNode(name+"_node"))
		{
			_sceneMgr->destroySceneNode(name+"_node");
		}

	}//deleteSceneNode



	//PT. Creacion de una particula
	Ogre::ParticleSystem* CScene::createParticula(const std::string &name1, const std::string &name2)
	{

	char numstr[21]; // enough to hold all numbers up to 64-bits
	sprintf(numstr, "%d", counterParticles);

	// Creamos nuestro sistema de partículas :)
	Ogre::ParticleSystem *pssmoke;
	pssmoke = _sceneMgr->createParticleSystem(name1+numstr, name2);

	// Creamos un nodo y atachamos la particula pssmoke a ese scenenode
	Ogre::SceneNode* sceneNode = _sceneMgr->createSceneNode(name1+"_particleSystemNode_"+numstr);
	sceneNode->attachObject(pssmoke);

	if(_sceneMgr->hasSceneNode(name1+"_node"))
	{
		_sceneMgr->getSceneNode(name1+"_node")->addChild(sceneNode);
	}

	// Desvinculamos el sistema de partículas del nodo
	/*
	sceneNode->detachObject(pssmoke);
 
	// Destruimos el nodo
	_sceneMgr->destroySceneNode(sceneNode);
 
	// Destruimos el sistema de partículas
	_sceneMgr->destroyParticleSystem(pssmoke);
	*/

	counterParticles++;

	return pssmoke;

	}//createParticula
	

} // namespace Graphics
