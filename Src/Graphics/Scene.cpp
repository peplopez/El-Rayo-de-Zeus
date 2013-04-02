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

#include "Graphics/Camera.h"
#include "Graphics/Entity.h"
#include "Graphics/GlowMaterialListener.h"
#include "Graphics/Server.h"
#include "Graphics/SceneElement.h"

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
			_staticGeometry(0), _directionalLight1(0), _directionalLight2(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
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
		_viewport->setBackgroundColour(Ogre::ColourValue::Black);
		
		// FRS Lo suyo sería introducirlas mediante un CShadows o algo asin + attachToScene 
		//Sombras Chulas - Consumen mucho*/
		//_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9f,0.9f,0.9f));

		// Además de la luz ambiente creamos una luz direccional que 
		// hace que se vean mejor los volúmenes de las entidades.
		_directionalLight1 = _sceneMgr->createLight("directional light1");
		//_directionalLight2 = _sceneMgr->createLight("directional light2");

		_directionalLight1->setDiffuseColour(.5f,.5f,.5f);
		//_directionalLight2->setDiffuseColour(.5f,.5f,.5f);

		_directionalLight1->setSpecularColour(.5f,.5f,.5f);
		//_directionalLight2->setSpecularColour(.5f,.5f,.5f);

		_directionalLight1->setType(Ogre::Light::LT_DIRECTIONAL);
		//_directionalLight2->setType(Ogre::Light::LT_DIRECTIONAL);

		_directionalLight1->setPosition(150, 150, 0);
		//_directionalLight2->setPosition(-150, 150, 0);

		_directionalLight1->setDirection(-150, -150, 0);
		//_directionalLight2->setDirection(150, -150, 0);

		Ogre::CompositorManager::getSingletonPtr()->addCompositor(_camera->getViewport(), "Glow");
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_camera->getViewport(), "Glow", true);

		GlowMaterialListener *gml = new GlowMaterialListener();
		Ogre::MaterialManager::getSingletonPtr()->addListener(gml);

		//_directionalLight->setType(Ogre::Light::LT_POINT);
		//_directionalLight->setPosition(0, 100, 0);
		

	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_directionalLight1)
		{
			_sceneMgr->destroyLight(_directionalLight1);
			_directionalLight1 = 0;
		}
		if(_directionalLight2)
		{
			_sceneMgr->destroyLight(_directionalLight2);
			_directionalLight2 = 0;
		}
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
	void CScene::remove(CSceneElement* sceneElement) {	sceneElement->deattachFromScene();			} 


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

	void CScene::remove(CEntity* entity)
	{
		entity->deattachFromScene();

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
