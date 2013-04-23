/**
@file Camera.cpp

Contiene la implementación del componente que controla la cámara gráfica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/
//#include     <cmath>
#pragma warning (disable : 4244 ) 
#include "PlayerCamera.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"
#include "Logic/Entity/LogicalPosition.h"
#include "GUI/InputManager.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"

namespace Logic 
{
	IMP_FACTORY(CPlayerCamera);
	
	//---------------------------------------------------------

	bool CPlayerCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicsCamera = _entity->getMap()->getGraphicScene()->getCamera();
		if(!_graphicsCamera)
			return false;

		if(entityInfo->hasAttribute("distance"))
			_distance = entityInfo->getFloatAttribute("distance");

		if(entityInfo->hasAttribute("height"))
			_height = entityInfo->getFloatAttribute("height");

		if(entityInfo->hasAttribute("targetDistance"))
			_targetDistance = entityInfo->getFloatAttribute("targetDistance");

		if(entityInfo->hasAttribute("targetHeight"))
			_targetHeight = entityInfo->getFloatAttribute("targetHeight");

		if(entityInfo->hasAttribute("cameraSpeed"))
			_cameraSpeed = entityInfo->getFloatAttribute("cameraSpeed");
	
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CPlayerCamera::activate()
	{		
		_target = CServer::getSingletonPtr()->getPlayer(); 
		
 
		_currentPos = (2000, 600, 2000);// Esto lo ponemos así de momento para que salga desde arriba la camara

		_graphicsCamera->setCameraPosition(_currentPos);

		
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CPlayerCamera::deactivate()
	{
		_target = 0;
	} // deactivate
	
	//---------------------------------------------------------

	void CPlayerCamera::changeHeight(const unsigned short &base, const LogicalPosition::Ring &ring)
	{
	
	}

	//---------------------------------------------------------

	bool CPlayerCamera::accept(const CMessage *message)
	{
		return message->getType() == Message::CAMERA;
	}

	//---------------------------------------------------------

	 void CPlayerCamera::process(CMessage *message)
	{
		CMessageBoolFloat *maux = static_cast<CMessageBoolFloat*>(message);

		if (maux->getBool())
			_targetDistance+=maux->getFloat();
		else
			_targetDistance-=maux->getFloat();
	}

	//---------------------------------------------------------

	void CPlayerCamera::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	
		if(!_target || (msecs > 1000))
			return;
		

	
		Vector3 position = _target->getPosition();
		Vector3 direction = _distance * position.normalisedCopy(); ;
		direction.y = _height;

		Vector3 finalCameraPosition = position + direction;
		
		//inercia de la camara
		Vector3 deltaMove = ( finalCameraPosition 
			- _currentPos ) * _cameraSpeed * msecs * 0.001;


		_currentPos += deltaMove;


		_graphicsCamera->setCameraPosition(_currentPos);
			

		position = (_targetDistance +  _target->getLogicalPosition()->getRadio()) * _currentPos.normalisedCopy() ;
		position.y =  _target->getPosition().y + _targetHeight;
		
		_graphicsCamera->setTargetCameraPosition(position);
		
	} // tick

} // namespace Logic

