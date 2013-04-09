/**
@file Camera.cpp

Contiene la implementaci�n del componente que controla la c�mara gr�fica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "BaseCamera.h"

#include "BaseSubsystems/Math.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

#include "GUI/InputManager.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"

namespace Logic 
{
	IMP_FACTORY(CBaseCamera);
	
	//---------------------------------------------------------

	bool CBaseCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicsCamera = _entity->getMap()->getGraphicScene()->getBaseCamera();
		if(!_graphicsCamera)
			return false;

		if(entityInfo->hasAttribute("radius"))
			_radius = entityInfo->getFloatAttribute("radius");

		if(entityInfo->hasAttribute("height"))
			_height = entityInfo->getFloatAttribute("height");

		if(entityInfo->hasAttribute("angularspeed"))
			_angularSpeed = entityInfo->getFloatAttribute("angularspeed");


	
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CBaseCamera::activate()
	{		
		_graphicsCamera->setTargetCameraPosition(Vector3::ZERO);
		_currentPos = Math::fromCylindricalToCartesian(0, _radius, _height);// Esto lo ponemos as� de momento para que salga desde arriba la camara.
		_graphicsCamera->setCameraPosition(_currentPos);
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CBaseCamera::deactivate()
	{

	} // deactivate
	

	//---------------------------------------------------------
		
	bool CBaseCamera::accept(const CMessage *message)
	{
		return message->getType() == Message::CAMERA;
	}

	//---------------------------------------------------------

	 void CBaseCamera::process(CMessage *message)
	{
		CMessageBoolFloat *maux = static_cast<CMessageBoolFloat*>(message);

	}

	//---------------------------------------------------------

	void CBaseCamera::tick(unsigned int msecs)
	{
		
		_degree += _angularSpeed * msecs/1000; 

		if (_degree > 360) 
			_degree -= 360;

		_currentPos  = Math::fromCylindricalToCartesian(_degree, _radius, _height);

		_graphicsCamera->setCameraPosition(_currentPos);
		_graphicsCamera->setTargetCameraPosition(Vector3::ZERO);
		
		
	} // tick

} // namespace Logic