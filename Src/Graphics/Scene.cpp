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
#include "Logic\Server.h"
#include "Camera.h"
#include "Server.h"
#include "StaticEntity.h"
#include "BaseSubsystems/Server.h"

#include <assert.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreStaticGeometry.h>
#include <OgreColourValue.h>
#include <OgreBillboardSet.h> //Pablo
#include "Logic/Entity/Entity.h" //Pablo

namespace Graphics 
{

	CScene::CScene(const std::string& name) : _viewport(0), 
			_staticGeometry(0), _directionalLight(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
		_name = name;

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

	bool CScene::addEntity(CEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_dynamicEntities.push_back(entity);
		return true;

	} // addEntity

	//--------------------------------------------------------

	bool CScene::addStaticEntity(CStaticEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_staticEntities.push_back(entity);
		return true;

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::removeEntity(CEntity* entity)
	{
		entity->deattachFromScene();
		_dynamicEntities.remove(entity);

	} // addEntity

	//--------------------------------------------------------

	void CScene::removeStaticEntity(CStaticEntity* entity)
	{
		entity->deattachFromScene();
		_staticEntities.remove(entity);

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::activate()
	{
		buildStaticGeometry();
		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
						->addViewport(_camera->getCamera());
		_viewport->setBackgroundColour(Ogre::ColourValue::Black);

		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9f,0.9f,0.9f));

		// Además de la luz ambiente creamos una luz direccional que 
		// hace que se vean mejor los volúmenes de las entidades.
		_directionalLight = _sceneMgr->createLight("directional light");
		_directionalLight->setDiffuseColour(.5f,.5f,.5f);
		_directionalLight->setSpecularColour(.5f,.5f,.5f);
		_directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
		_directionalLight->setDirection(0, -150, 0);
		_directionalLight->setType(Ogre::Light::LT_POINT);
		//_directionalLight->setPosition(0, 100, 0);
		_directionalLight->setPosition(100, 100, 100);

	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_directionalLight)
		{
			_sceneMgr->destroyLight(_directionalLight);
			_directionalLight = 0;
		}
		if(_viewport)
		{
			BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->
					removeViewport(_viewport->getZOrder());
			_viewport = 0;
		}

	} // deactivate
	
	//--------------------------------------------------------

	
	void CScene::tick(float secs)
	{	
		TEntityList::const_iterator it = _dynamicEntities.begin();
		TEntityList::const_iterator end = _dynamicEntities.end();
		for(; it != end; it++)
			(*it)->tick(secs);

	} // tick

	//--------------------------------------------------------

	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticEntities.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TStaticEntityList::const_iterator it = _staticEntities.begin();
			TStaticEntityList::const_iterator end = _staticEntities.end();
			for(; it != end; it++)
				(*it)->addToStaticGeometry();

			_staticGeometry->build();
		}

	} // buildStaticGeometry



	// David LLanso Tutoria
	// Se añade un nuevo método para crear un Billboard desde la escena
	
	//le paso un string
	Ogre::BillboardSet* CScene::createBillboard(const std::string &name) 
	//Ogre::BillboardSet* CScene::createBillboard(const std::string &name, Ogre::Vector3 position) //no puedo pasarle
	//la position.... no se porq
	{
		Ogre::BillboardSet* _bbSet;
		_bbSet = _sceneMgr->createBillboardSet(name+"_billboard");
		Vector3 posAnilloCentral=Logic::CServer::getSingletonPtr()->getRingPositions(3,Logic::Ring::ANILLO_CENTRAL);

		//consigo la entidad de ogre llamada name
		Ogre::Entity* oe =_sceneMgr->getEntity(name);

		_bbSet->createBillboard(posAnilloCentral);


	   // se añade el billboard a la entidad en lugar de a la escena (hay que cambiar la linea con *)
	   // *  //_sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(_bbSetLife); 
		//_sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(_bbSet);
		Ogre::Entity* en = _sceneMgr->getEntity(name);
		_sceneMgr->getSceneNode(name)->attachObject(_bbSet);
		//_sceneMgr->getSceneNode()->

		return _bbSet; //debéría devolver el Ogre::BillboardSet

	}
	

} // namespace Graphics
