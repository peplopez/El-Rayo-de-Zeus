/**
@file Camera.cpp

Contiene la implementación del componente que controla la cámara gráfica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Camera.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

namespace Logic 
{
	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	bool CCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicsCamera = _entity->getMap()->getScene()->getCamera();
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

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CCamera::activate()
	{		
		_target = CServer::getSingletonPtr()->getPlayer(); 

		if(!_target) {			
			// UNDONE _entity->setIsPlayer(true); // Activamos cámara como player -> manejar cámara -> TODO crear un CamaraController
			_target = CServer::getSingletonPtr()->getMap()->getEntityByType("Player");
		}

		return true;

	} // activate
	
	//---------------------------------------------------------

	void CCamera::deactivate()
	{
		_target = 0;

	} // deactivate
	
	//---------------------------------------------------------

	void CCamera::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_target)
		{
			Vector3 targetPos = _target->getPosition();

			// Actualizamos la posición de la cámara.
			Vector3 offset = -_distance * Math::getDirection(_target->getOrientation()); // Offset desde detrás del jugador
				offset.y = _height;
			_graphicsCamera->setCameraPosition(targetPos + offset);

			// Y la posición hacia donde mira la cámara 
				offset = _targetDistance * Math::getDirection(_target->getOrientation()); // Offset hacia delante del player
				offset.y = _targetHeight;
			_graphicsCamera->setTargetCameraPosition(targetPos + offset);
		}

	} // tick

} // namespace Logic

