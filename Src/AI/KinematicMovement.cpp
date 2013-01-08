/**
@file KinematicMovement.cpp

Contiene distintas clases que heredan de IMovement y que 
implementan distintos tipos de movimiento cinemáticos.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "KinematicMovement.h"
#include "Server.h"

namespace AI 
{
	/**
	Efectúa el movimiento.
		
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
	en él se devuelven los nuevos valores de velocidad.
	*/
	void CKinematicSeek::move(unsigned int msecs, DynamicMovement& currentProperties)
	{
		assert(_entity);
		// TODO	PRÁCTICA IA
		// Implementar el movimiento Seek (cinemático)
		// En _entity tenemos un puntero a la entidad
		// En _maxLinearSpeed tenemos la velocidad máxima a la que se puede mover la entidad
		// currentProperties es un parámetro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleración
		currentProperties.linearSpeed = _target - _entity->getPosition();
		if (currentProperties.linearSpeed.length() > _maxLinearSpeed) {
			currentProperties.linearSpeed.normalise();
			currentProperties.linearSpeed *= _maxLinearSpeed;
		}
	}
	/////////////////////////////////////////
	/**
	Efectúa el movimiento.
		
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
	en él se devuelven los nuevos valores de velocidad.
	*/
	void CKinematicArrive::move(unsigned int msecs, AI::IMovement::DynamicMovement &currentProperties)
	{
		assert(_entity);
		// TODO	PRÁCTICA IA
		// Implementar el movimiento Arrive (cinemático)
		// En _entity tenemos un puntero a la entidad
		// En _maxLinearSpeed tenemos la velocidad máxima a la que se puede mover la entidad.
		// IMovement::TIME_TO_TARGET nos da el tiempo que utilizamos para llegar al objetivo.
		// currentProperties es un parámetro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleración
		currentProperties.linearSpeed = (_target - _entity->getPosition());
		currentProperties.linearSpeed /= IMovement::TIME_TO_TARGET;
		if (currentProperties.linearSpeed.squaredLength() > _maxLinearSpeed2) {
			currentProperties.linearSpeed.normalise();
			currentProperties.linearSpeed *= _maxLinearSpeed;
		}
	}
	/////////////////////////////////////////
	/**
	Efectúa el movimiento.
		
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
	en él se devuelven los nuevos valores de velocidad.
	*/
	void CKinematicAlignToSpeed::move(unsigned int msecs, DynamicMovement& currentProperties)
	{
		assert(_entity);
		// TODO	PRÁCTICA IA
		// Implementar el movimiento AlignToSpeed.
		// En _entity tenemos un puntero a la entidad.
		// En _maxAngularSpeed tenemos la velocidad máxima a la que puede girar la entidad.
		// currentProperties es un parámetro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleración.
		// Puede ser útil usar el método IA::CServer::correctAngle, que nos sirve para asegurarnos de que, después
		// de restar dos ángulos, el resultado se mantiene entre -PI y PI.
		currentProperties.angularSpeed = (atan2(-currentProperties.linearSpeed.x, -currentProperties.linearSpeed.z) - _entity->getYaw());
		currentProperties.angularSpeed = CServer::correctAngle(currentProperties.angularSpeed);
		currentProperties.angularSpeed /= IMovement::TIME_TO_TARGET;
		if (abs(currentProperties.angularSpeed) > _maxAngularSpeed) {
			currentProperties.angularSpeed = Ogre::Math::Sign((float)currentProperties.angularSpeed) * _maxAngularSpeed;
		}
	}
} //namespace AI