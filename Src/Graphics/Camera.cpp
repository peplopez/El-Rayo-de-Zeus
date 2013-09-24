//---------------------------------------------------------------------------
// Camera.cpp
//---------------------------------------------------------------------------

/**
@file Camera.cpp

Contiene la implementación de la clase que maneja la cámara.

@see Graphics::CCamera

@author David Llansó
@date Julio, 2010
*/

#include "Camera.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>

namespace Graphics 
{
	CCamera::CCamera(const std::string &name, CScene *scene)
		: _scene(0)
	{
		_name = name;
		_scene = scene;

		_camera = scene->getSceneMgr()->createCamera(name + "_camera");
			_camera->setNearClipDistance(20);
			_camera->setFarClipDistance(30000);
		//HACK: Valores cableados de las distancias para reenderizar. 
		// Deberían poder configurarse.

		// Creamos la estructura de nodos de la cámara. Los nodos cuelgan
		// de la raiz, son globales.		
		_targetNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(name + "_target_node");
		_camera->setAutoTracking(true, _targetNode);
		_camera->setFixedYawAxis(true);
	} // CCamera

	//--------------------------------------------------------

	CCamera::~CCamera() 
	{
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_targetNode);

	} // ~CCamera


	/********************
		GET's & SET's
	*******************/
	
	const Vector3 &CCamera::getPosition() const
	{
		return _camera->getPosition();
	}
	
	//--------------------------------------------------------
	
	const Quaternion &CCamera::getOrientation() const
	{
		return _camera->getOrientation();
	}

	//--------------------------------------------------------

	const Vector3 &CCamera::getTargetPosition() const
	{
		return _targetNode->getPosition();
	}

	//--------------------------------------------------------

	Ogre::Viewport* CCamera::getViewport() const
	{
		return _camera->getViewport();
	}


	//--------------------------------------------------------

	void CCamera::setPosition(const Vector3 &newPosition)
	{
		_camera->setPosition(newPosition);
	}

	//--------------------------------------------------------

	void CCamera::setTargetPosition(const Vector3 &newPosition)
	{
		_targetNode->setPosition(newPosition);
	}

	

} // namespace Graphics
