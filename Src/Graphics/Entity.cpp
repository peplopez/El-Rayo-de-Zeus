//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"

#include "Graphics/Scene.h"

#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
		
	bool CEntity::load()
	{
		try{
			_entity = _scene->getSceneMgr()->createEntity(_name, _mesh);		
		} catch(std::exception e){
			return false;
		}

		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->
								createChildSceneNode(_name + "_node");
		
		_entityNode->attachObject(_entity);
		_loaded = true;

		// HACK Emily: cutre - para attach del arma en el Player
		if(!_name.compare("Mono")) // que es como llamamos al player en el mapa
		{
			Ogre::Entity *weapon = _scene->getSceneMgr()->createEntity("weapon", "Katana.mesh");
			_entity->attachObjectToBone("Bip01 R Hand",weapon);
		}

		return true;
	} // load
	
	//--------------------------------------------------------
		
	void CEntity::unload()
	{
		if(_entityNode)
		{
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}
		if(_entity)
		{
			_scene->getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
		}

	} // load

	//--------------------------------------------------------
		


	void CEntity::tick(float secs)
	{
	} // tick
	
	//--------------------------------------------------------
		
	void CEntity::setTransform(const Matrix4 &transform)
	{
		assert(_entityNode && "La entidad no ha sido cargada en la escena");
		if(_entityNode)
		{
			_entityNode->setPosition(transform.getTrans());
			_entityNode->setOrientation(transform.extractQuaternion());
		}

	} // setTransform
	
	//--------------------------------------------------------
		
	void CEntity::setOrientation(const Matrix3 &orientation)
	{
		assert(_entityNode && "La entidad no ha sido cargada en la escena");
		if(_entityNode)
			_entityNode->setOrientation(orientation);

	} // setOrientation
	
	//--------------------------------------------------------

	Ogre::SceneNode* CEntity::getEntityNode()
	{
		assert(_entityNode && "La entidad no ha sido cargada en la escena");
		return _entityNode;
	}

	//--------------------------------------------------------

	void CEntity::setMaterial(const std::string &materialName) 
	{
		assert(_entityNode && "La entidad no ha sido cargada en la escena");
		if(_entityNode)
			_entity->setMaterialName(materialName);
			//_entity->getChild(0)->setMaterialName();

	} // setMaterial

	//--------------------------------------------------------
	void CEntity::setSubEntityMaterial(const std::string &materialName, const unsigned int subEntityIndex) 
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entity->getSubEntity(subEntityIndex)->setMaterialName(materialName);
			//_entity->getChild(0)->setMaterialName();

	} // setSubEntityMaterial

	//--------------------------------------------------------
		
	void CEntity::setVisible(bool visible)
	{
		assert(_entityNode && "La entidad no ha sido cargada en la escena");
		if(_entityNode)
			_entityNode->setVisible(visible);

	} // setVisible
	
	//--------------------------------------------------------
		
	const bool CEntity::isVisible()
	{
		assert(_entityNode && "La entidad no ha sido cargada en la escena");
		return _entity->isVisible();
	} // getPosition
	
	//--------------------------------------------------------
		
	void CEntity::setPosition(const Vector3 &position)
	{
		assert(_entityNode && "La entidad no ha sido cargada en la escena");
		if(_entityNode)
			_entityNode->setPosition(position);

	} // setPosition
	
	//--------------------------------------------------------
		
	void CEntity::setScale(const Vector3 &scale)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entityNode->setScale(scale);

	} // setScale
	
	//--------------------------------------------------------
		
	void CEntity::setScale(const float scale)
	{
		assert(_entityNode && "La entidad no ha sido cargada en la escena");
		if(_entityNode)
			_entityNode->setScale( Vector3(scale,scale,scale) );

	} // setScale


} // namespace Graphics
