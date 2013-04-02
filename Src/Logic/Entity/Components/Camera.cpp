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
#include "Camera.h"

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
	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	bool CCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
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

	
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CCamera::activate()
	{		
		_target = CServer::getSingletonPtr()->getPlayer(); 
		//_target->setPosition(Math::fromPolarToCartesian(0,60)); //esto no está bien aquí, pero si no está no calcula bien el vector dirección.
		//_target->fromLogicalToCartesian(0,60,_target->getBase(),_target->getRing()); //esto no está bien aquí, pero si no está no calcula bien el vector dirección.
		//anula lo que haya en el maps.txt sobre la posición del prota
		_currentPos = Vector3(0,-1500,0);// Esto lo ponemos así de momento para que salga desde arriba la camara.
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

		
	bool CCamera::accept(const CMessage *message)
	{
		return message->getType() == Message::CAMERA;
	}

	 void CCamera::process(CMessage *message)
	{
		CMessageBoolFloat *maux = static_cast<CMessageBoolFloat*>(message);
		if (maux->getBool())
			_targetDistance+=maux->getFloat();
		else
			_targetDistance-=maux->getFloat();
	}

	void CCamera::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	
		if(!_target)
			return;

		// Actualizamos la posición de la cámara.
		//este parrafo es para conseguir que el modelo mire en dirección perpendicular al vector centro camara
		Vector3 centro=Vector3::NEGATIVE_UNIT_Y; // TODO [ƒ®§] por que negative y no ZERO?
		centro.y=_target->getPosition().y;
		//Vector3 centro=Vector3(0,-125-250,0);
			

		Vector3 vectorCentroProtaCamara =  -(centro-_target->getPosition());
		vectorCentroProtaCamara.normalise();
		//Vector3 actualDirection=Math::getDirection(_target->getOrientation());
		//Vector3 directionPerp= Vector3::UNIT_Y.crossProduct(vectorCentroProtaCamara); // [ƒ®§] Si no son necesarios, mejor limpiar (ya se buscarán en git)
		//Quaternion rotacionDestino=actualDirection.getRotationTo(directionPerp);
			
		Matrix4 orientacion = _target->getOrientation();
		//std::cout<<vectorCentroProtaCamara<<std::endl; // TODO Para estas cosillas es mejor usar la macro LOG (ver AnimatedGraphics)
		//std::cout<<Math::getDirection(orientacion)<<std::endl;
			
		//Math::yaw(Math::fromDegreesToRadians(-90),orientacion);
		Vector3 direction = vectorCentroProtaCamara; //-_distance * (Math::getDirection(orientacion))  ;
		
		direction.normalise();
		direction.y = _targetHeight;
			
		vectorCentroProtaCamara.normalise();
		vectorCentroProtaCamara.y=direction.y;
		//std::cout<<"vectorcentroprotacamara: "<<vectorCentroProtaCamara<<std::endl;
		
		//inercia de la camara
		Vector3 cameraTarget=CServer::getSingletonPtr()->getRingPositions(_target->getLogicalPosition()->getBase(),_target->getLogicalPosition()->getRing());
		_currentPos =_currentPos+ (
			(Vector3(_target->getPosition().x*_targetDistance,cameraTarget.y+126,_target->getPosition().z*_targetDistance) +
			Vector3(0,_targetHeight*2,0)) -
			_currentPos) * 0.035f*0.05f*msecs;//0.05*
		
			_graphicsCamera->setCameraPosition(_currentPos);

			
		//_graphicsCamera->setCameraPosition( +4*position);
		//anillo x=0 y=-125 z=0
		// Y la posición hacia donde mira la cámara.
		//direction = _targetDistance * Math::getDirection(orientacion);

		direction = _targetDistance * direction;
		direction.y = _targetHeight;
		cameraTarget.y+=126;
		_graphicsCamera->setTargetCameraPosition(cameraTarget);
		
	} // tick

} // namespace Logic

