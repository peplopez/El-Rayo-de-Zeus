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

#include "Camera.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"
#include "Logic/Entity/LogicalPosition.h"

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
		//_target->setPosition(Math::fromPolarToCartesian(0,60)); //esto no está bien aquí, pero si no está no calcula bien el vector dirección.
		//_target->fromLogicalToCartesian(0,60,_target->getBase(),_target->getRing()); //esto no está bien aquí, pero si no está no calcula bien el vector dirección.
		
		//anula lo que haya en el maps.txt sobre la posición del prota

		_currentPos = 4*_target->getPosition()+Vector3(0,_targetHeight*2,0);
		 _graphicsCamera->setCameraPosition(_currentPos);
		return true;

	} // activate
	
	//---------------------------------------------------------

	void CCamera::deactivate()
	{
		_target = 0;

	} // deactivate
	
	//---------------------------------------------------------

	void CCamera::changeHeight(const unsigned short &base, const LogicalPosition::Ring &ring)
	{
		
	}

	void CCamera::tick(unsigned int msecs)
	{
		if(_target)
		{			
			// Actualizamos la posición de la cámara.
			//este parrafo es para conseguir que el modelo mire en dirección perpendicular al vector centro camara
			Vector3 centro=Vector3::NEGATIVE_UNIT_Y;
		    centro.y=_target->getPosition().y;
			//Vector3 centro=Vector3(0,-125-250,0);
			

			Vector3 vectorCentroProtaCamara =  -(centro-_target->getPosition());
			vectorCentroProtaCamara.normalise();
			//Vector3 actualDirection=Math::getDirection(_target->getOrientation());
			//Vector3 directionPerp= Vector3::UNIT_Y.crossProduct(vectorCentroProtaCamara);
			//Quaternion rotacionDestino=actualDirection.getRotationTo(directionPerp);
			
			Matrix4 orientacion = _target->getOrientation();
			//std::cout<<vectorCentroProtaCamara<<std::endl;
			//std::cout<<Math::getDirection(orientacion)<<std::endl;
			
			//Math::yaw(Math::fromDegreesToRadians(-90),orientacion);
			Vector3 direction = vectorCentroProtaCamara; //-_distance * (Math::getDirection(orientacion))  ;
		
			direction.normalise();
			direction.y = _targetHeight;
			
			vectorCentroProtaCamara.normalise();
			vectorCentroProtaCamara.y=direction.y;
			//std::cout<<"vectorcentroprotacamara: "<<vectorCentroProtaCamara<<std::endl;
		
			//inercia de la camara
			Vector3 cameraTarget=CServer::getSingletonPtr()->getRingPositions(_target->getBase(),_target->getRing());
			//_currentPos += ((Vector3(_target->getPosition().x*1.75,cameraTarget.y+126,_target->getPosition().z*1.75)+Vector3(0,_targetHeight*2,0)) - _currentPos) * 0.035;
			_currentPos += ((Vector3(_target->getPosition().x*2.25,cameraTarget.y+126,_target->getPosition().z*1.75)+Vector3(0,_targetHeight*2,0)) - _currentPos) * 0.035;
		
			 _graphicsCamera->setCameraPosition(_currentPos);

			
			//_graphicsCamera->setCameraPosition( +4*position);
			//anillo x=0 y=-125 z=0
			// Y la posición hacia donde mira la cámara.
			//direction = _targetDistance * Math::getDirection(orientacion);

			direction = _targetDistance * direction;
			direction.y = _targetHeight;
			cameraTarget.y+=126;
			_graphicsCamera->setTargetCameraPosition(cameraTarget);
			
		}
		IComponent::tick(msecs);
	} // tick

} // namespace Logic

