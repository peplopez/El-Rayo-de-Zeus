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

	CLight::~CLight() 
	{
		assert(!_scene && "¡¡Para destruir una luz ésta no puede pertenecer a una escena!!");
	} // ~CLight
	
	//-------------------------------------------------------
	
	bool CLight::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡La luz debe asociarse a una escena!!");
		
		// Si la luz está cargada por otro gestor de escena.
		if(_loaded) {			
			if(_scene != scene)
				return false;
			else
				return true; // Si ya estaba cargado en la escena se devuelve cierto.
		
		} else { // Si no está cargado forzamos su carga.		
			_scene = scene;
			return load();
		}
	} // attachToScene

	//-------------------------------------------------------		
		
	bool CLight::deattachFromScene()
	{
		// Si  la luz no está cargada no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!_loaded)
			return false;		
		else { // Si está cargada forzamos su descarga.
			assert(_scene && "¡¡La luz debe estar asociada a una escena!!");
			unload();
			_scene = 0;
			return true;
		}
	} // deattachFromScene

	//-------------------------------------------------------	

	bool CLight::load()
	{
		try{
			_light = _scene->getSceneMgr()->createLight(_name);
			_loaded = true;
			return true;
		} catch(std::exception e){
			return false;
		}

	} // load
	
	//--------------------------------------------------------
		
	void CLight::unload()
	{
		if(_light)
		{
			// desacoplamos la entidad de su nodo
			_scene->getSceneMgr()->destroyLight(_light);
			_light = 0;
			_loaded = false;
		}

	} // unload

	//--------------------------------------------------------

	void CLight::tick(float secs)
	{
	} // tick

	//--------------------------------------------------------

	/**********************
		GET's & SET's
	***********************/

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

	void CLight::setAttenuation(float range, float constant, float linear, float cuadratic)
	{
		assert(_light && "La luz no ha sido cargada en la escena");
		if(_light)
			_light->setAttenuation(range, constant, linear, cuadratic);

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