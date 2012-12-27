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

		std::cout<<Math::fromPolarToCartesian(50,45)<<std::endl;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CCamera::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_target->setPosition(Math::fromPolarToCartesian(0,60)); //esto no está bien aquí, pero si no está no calcula bien el vector dirección.
		//anula lo que haya en el maps.txt sobre la posición del prota

		 _graphicsCamera->setCameraPosition(4*_target->getPosition()+Vector3(0,_targetHeight*2,0));
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
		

		if(_target)
		{
			std::pair<float,float> ja=Math::fromCartesianToPolar(Vector3(0,0,-55));
			// Actualizamos la posición de la cámara.
			Vector3 centro=Vector3(0,-125,0);
			Vector3 vectorCentroProtaCamara =  -(centro-_target->getPosition());
			vectorCentroProtaCamara.normalise();
			Vector3 actualDirection=Math::getDirection(_target->getOrientation());
			Vector3 directionPerp= Vector3::UNIT_Y.crossProduct(vectorCentroProtaCamara);
			Quaternion rotacionDestino=actualDirection.getRotationTo(directionPerp);
			
		//	_target->setOrientation(rotacionDestino.w,rotacionDestino.x,rotacionDestino.y,rotacionDestino.z);
		//	_target->setYaw();
			//	std::cout<<"dot: "<<directionPerp.dotProduct(vectorCentroProtaCamara)<<std::endl;
		//	std::cout<<directionPerp.crossProduct(vectorCentroProtaCamara)<<std::endl;
		//	std::cout<<"directionPerp: "<<directionPerp<<std::endl;
		//	position.y=-125;


			
			// Ogre::Quaternion quaternion =_target->getOrientation();

			// quaternion.from

			// convert orientation to a matrix
			/*Ogre::Matrix3 matrix3;

			quaternion.ToRotationMatrix( matrix3 );



			*/


			/*Logic::TMessage m;
			m._type = Logic::Message::CONTROL;
			m._string = "turn";
			m._float = -(float)1 * 0.001f;
			_target->emitMessage(m);

			*/
			Matrix4 orientacion = _target->getOrientation();
			//std::cout<<vectorCentroProtaCamara<<std::endl;
			//std::cout<<Math::getDirection(orientacion)<<std::endl;
			
			Math::yaw(Math::fromDegreesToRadians(-90),orientacion);
			Vector3 direction = vectorCentroProtaCamara; //-_distance * (Math::getDirection(orientacion))  ;
			//direction.x= vectorCentroProtaCamara.x*cos((double) 90) - vectorCentroProtaCamara.z*sin((double) 90);  //=vectorCentroProtaCamara*Ogre::rotate;
			//direction.z= vectorCentroProtaCamara.x*sin((double) 90) + vectorCentroProtaCamara.z*cos((double) 90);  //=vectorCentroProtaCamara*Ogre::rotate;
		//	std::cout<<"position: "<<_target->getPosition()<<std::endl;
			

			//Ogre::Vector3 src = _target->getOrientation() * Ogre::Vector3::UNIT_X;
			/*std::cout<<"target->getOrientation()"<<_target->getOrientation()<<std::endl;
			Ogre::Vector3 src =_target->getOrientation()* Ogre::Vector3::Vector3(1,1,1);
			std::cout<<"src: "<<src<<std::endl;
			std::cout<<direction<<std::endl;
			Ogre::Quaternion quat = src.getRotationTo(direction);
			_target->setOrientation(quat);
			*/

			direction.normalise();

		//	std::cout<<direction*vectorCentroProtaCamara<<std::endl;


			//Matrix3 rotationMatrix= Ogre::Matrix3::Matrix3(cos((double) 90),0,sin((double) 90),0,1,0,-sin((double) 90),0,cos((double) 90));
			//_entity->setOrientation(rotationMatrix);
		//	direction=rotationMatrix*vectorCentroProtaCamara;
		//	std::cout<<orientacion<<std::endl;
			

			/*
			xnuevo = xviejocos(ß) – zviejosin(ß)

			znuevo = xviejosin(ß) + zviejocos(ß)*/
			
			//Vector3 direction =  _distance *vectorCentroProtaCamara;
			
			/*direction.x=position.x;
			//direction.y=position.y-125+_height;
			direction.y=(position.y-125)*_distance;
			direction.z=position.z;	
			*/
			direction.y = _targetHeight;
			
			vectorCentroProtaCamara.normalise();
			vectorCentroProtaCamara.y=direction.y;
			//std::cout<<"vectorcentroprotacamara: "<<vectorCentroProtaCamara<<std::endl;
			_graphicsCamera->setCameraPosition(4*_target->getPosition()+Vector3(0,_targetHeight*2,0));
//			_graphicsCamera->setCameraPosition( +4*position);
			//anillo x=0 y=-125 z=0
			// Y la posición hacia donde mira la cámara.
			//direction = _targetDistance * Math::getDirection(orientacion);

			direction = _targetDistance * direction;
			direction.y = _targetHeight;

			//_graphicsCamera->setTargetCameraPosition(position+direction);
			
		}
IComponent::tick(msecs);
	} // tick

} // namespace Logic

