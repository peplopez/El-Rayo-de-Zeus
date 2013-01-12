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

namespace Graphics 
{
	CCamera::CCamera(const std::string &name, CScene *scene)
		: _scene(0)
	{
		_name = name;
		_scene = scene;

		// Creamos la estructura de nodos de la cámara. Los nodos cuelgan
		// de la raiz, son globales.
		_cameraNode = _scene->getSceneMgr()->getRootSceneNode()->
					createChildSceneNode(name + "_camera_node");
		_targetNode = scene->getSceneMgr()->getRootSceneNode()->
					createChildSceneNode(name + "_target_node");

		// Hacemos que el nodo de la cámara siempre esté mirando al nodo
		// objetivo.
		_cameraNode->setAutoTracking(true, _targetNode);
		// Fijamos el viraje de la cámara para se mantenga paralelo al
		// suelo.
		_cameraNode->setFixedYawAxis(true);

		_camera = scene->getSceneMgr()->createCamera(name + "_camera");
		//HACK: Valores cableados de las distancias para reenderizar. 
		// Deberían poder configurarse.
		_camera->setNearClipDistance(5);
		_camera->setFarClipDistance(500);
		// Finalmente adjuntamos la cámara a su nodo.
		_cameraNode->attachObject (_camera);

	} // CCamera

	//--------------------------------------------------------

	CCamera::~CCamera() 
	{
		// desacoplamos la cámara de su nodo
		_cameraNode->detachAllObjects();
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_cameraNode);
		_scene->getSceneMgr()->destroySceneNode(_targetNode);

	} // ~CCamera

	//--------------------------------------------------------
	
	const Vector3 &CCamera::getCameraPosition() 
	{
		return _cameraNode->getPosition();
	}

	//--------------------------------------------------------

	const Vector3 &CCamera::getTargetCameraPosition() 
	{
		return _targetNode->getPosition();
	}

	//--------------------------------------------------------

	const Quaternion &CCamera::getCameraOrientation() 
	{
		return _camera->getOrientation();
	}

	//--------------------------------------------------------

	void CCamera::setCameraPosition(const Vector3 &newPosition)
	{
		_cameraNode->setPosition(newPosition);
	}

	//--------------------------------------------------------

	void CCamera::setTargetCameraPosition(const Vector3 &newPosition)
	{
		_targetNode->setPosition(newPosition);
	}


} // namespace Graphics
