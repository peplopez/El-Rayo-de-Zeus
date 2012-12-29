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
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CEntity::CEntity(const std::string &name, const std::string &mesh)
		: _entity(0), _entityNode(0), _scene(0), _loaded(false)
	{
		_name = name;
		_mesh = mesh;

	} // CEntity

	//--------------------------------------------------------

	CEntity::~CEntity() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		
	} // ~CEntity
	
	//--------------------------------------------------------
		
	bool CEntity::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡La entidad debe asociarse a una escena!!");
		// Si la entidad está cargada por otro gestor de escena.
		if(_loaded && (_scene != scene))
			return false;

		// Si no está cargada forzamos su carga.
		if (!_loaded)
		{
			_scene = scene;
			return load();
		}

		// Si ya estaba cargada en la escena se devuelve cierto.
		return true;

	} // attachToScene
	
	//--------------------------------------------------------
		
	bool CEntity::deattachFromScene()
	{
		// Si la entidad no está cargada no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!_loaded)
			return false;
		// Si la entidad está cargada forzamos su descarga.
		else
		{
			assert(_scene && "¡¡La entidad debe estar asociada a una escena!!");
			unload();
			_scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	bool CEntity::load()
	{
		try
		{
			_entity = _scene->getSceneMgr()->createEntity(_name, _mesh);
		}
		catch(std::exception e)
		{
			return false;
		}
		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->
								createChildSceneNode(_name + "_node");
		_entityNode->attachObject(_entity);
		_loaded = true;

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
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
		{
			_entityNode->setPosition(transform.getTrans());
			_entityNode->setOrientation(transform.extractQuaternion());
		}

	} // setTransform
	
	//--------------------------------------------------------
		
	void CEntity::setOrientation(const Matrix3 &orientation)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entityNode->setOrientation(orientation);

	} // setOrientation
	
	//--------------------------------------------------------
		
	void CEntity::setVisible(bool visible)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entityNode->setVisible(visible);

	} // setVisible
	
	//--------------------------------------------------------
		
	const bool CEntity::getVisible()
	{
		if(_entityNode)
			return _entity->isVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition
	
	//--------------------------------------------------------
		
	void CEntity::setPosition(const Vector3 &position)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
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
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
		{
			Vector3 scaleVector(scale,scale,scale);
			_entityNode->setScale(scaleVector);
		}

	} // setScale


} // namespace Graphics
