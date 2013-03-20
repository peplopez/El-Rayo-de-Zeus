//---------------------------------------------------------------------------
// Light.cpp
//---------------------------------------------------------------------------

/**
@file Light.cpp

Contiene la implementación de la clase que representa una luz.

@see Graphics::CLight

@author Emilio Santalla
@date Marzo, 2013
*/

#include "Light.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreSceneManager.h>

namespace Graphics 
{

	CLight::CLight(const std::string &name, CScene* scene)
			: _name(name), _light(0), _scene(scene), _loaded(false)
	{
		load();
	}

	//-------------------------------------------------------

	CLight::~CLight() 
	{
		unload();	
	} // ~CEntity
	
	//-------------------------------------------------------
		
	bool CLight::load()
	{
		try{
			_light = _scene->getSceneMgr()->createLight(_name);
		} catch(std::exception e){
			return false;
		}

		_loaded = true;

		return true;
	} // load
	
	//--------------------------------------------------------
		
	void CLight::unload()
	{
		if(_light)
		{
			// desacoplamos la entidad de su nodo
			_scene->getSceneMgr()->destroyLight(_light);
			_light = 0;
		}

	} // unload

	//--------------------------------------------------------

	void CLight::tick(float secs)
	{
	} // tick

	//--------------------------------------------------------

	void CLight::setType(const std::string &type)
	{
		if (_light)
		{
			if (type == "directional")
				_light->setType(Ogre::Light::LT_DIRECTIONAL);
			else if (type == "point")
				_light->setType(Ogre::Light::LT_POINT);
			else if (type == "spotlight")
				_light->setType(Ogre::Light::LT_SPOTLIGHT);
		}

	} // setType
	
	//--------------------------------------------------------
		
	void CLight::setPosition(const Vector3 &position)
	{
		assert(_light && "La luz no ha sido cargada en la escena");
		if(_light)
		{
			_light->setPosition(position);
		}

	} // setPosition
	
	//--------------------------------------------------------
		
	void CLight::setDirection(const Vector3 &direction)
	{
		assert(_light && "La luz no ha sido cargada en la escena");
		if(_light)
			_light->setDirection(direction);

	} // setDirection
	
	//--------------------------------------------------------

	Ogre::Light* CLight::getLight()
	{
		assert(_light && "La luz no ha sido cargada en la escena");
		return _light;

	} // getLight

	//--------------------------------------------------------

	void CLight::setDiffuseColour(float r, float g, float b) 
	{
		assert(_light && "La luz no ha sido cargada en la escena");
		if(_light)
			_light->setDiffuseColour(r, g, b);

	} // setDiffuseColour

	//--------------------------------------------------------

	void CLight::setSpecularColour(float r, float g, float b) 
	{
		assert(_light && "La luz no ha sido cargada en la escena");
		if(_light)
			_light->setSpecularColour(r, g, b);

	} // setDiffuseColour


	//--------------------------------------------------------
	
	void CLight::setVisible(bool visible)
	{
		assert(_light && "La luz no ha sido cargada en la escena");
		if(_light)
			_light->setVisible(visible);

	} // setVisible
	
	//--------------------------------------------------------
		
	const bool CLight::isVisible()
	{
		assert(_light && "La luz no ha sido cargada en la escena");
		return _light->isVisible();
	} // getPosition
	
	//--------------------------------------------------------
		
} // namespace Graphics