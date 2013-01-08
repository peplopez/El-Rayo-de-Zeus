/**
@file DynamicMovement.cpp

Contiene distintas clases que heredan de IMovement y que 
implementan distintos tipos de movimiento dinámico.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "DynamicMovement.h"
#include "Server.h"

namespace AI 
{

	/**
	Efectúa el movimiento.
		
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
	en él devuelve los nuevos valores de aceleración.
	*/
	void CDynamicSeek::move(unsigned int msecs, DynamicMovement& currentProperties)
	{
		assert(_entity);
		// TODO	PRÁCTICA IA
		// Implementar el movimiento Seek (dinámico)
		// En _entity tenemos un puntero a la entidad.
		// En _maxLinearAccel tenemos la aceleración máxima que puede tener la entidad.
		// currentProperties es un parámetro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleración.
		currentProperties.linearAccel = _target - _entity->getPosition();
		if (currentProperties.linearAccel.squaredLength() > _maxLinearAccel2) {
			currentProperties.linearAccel.normalise();
			currentProperties.linearAccel *= _maxLinearAccel;
		}
	}
	/////////////////////////////////////////
	/**
	Efectúa el movimiento.
		
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
	en él devuelve los nuevos valores de aceleración.
	*/
	void CDynamicArrive::move(unsigned int msecs, DynamicMovement &currentProperties)
	{
		assert(_entity);
		// TODO	PRÁCTICA IA
		// Implementar el movimiento Arrive (dinámico).
		// En _entity tenemos un puntero a la entidad.
		// En _maxLinearSpeed tenemos la velocidad máxima a la que se puede mover la entidad y 
		// en _maxLinearAccel la aceleración máxima.
		// Otras constantes útiles son IMovement::SLOW_RADIUS (radio a partir del cual empezamos a frenar
		// para aproximarnos a un punto) y IMovement::TIME_TO_TARGET (tiempo que utilizamos para llegar al objetivo).
		// currentProperties es un parámetro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleración.
		Vector3 targetVelocity = (_target - _entity->getPosition());
		float distance = targetVelocity.length();
		float targetSpeed;
		if (distance >= IMovement::SLOW_RADIUS) 
			targetSpeed = _maxLinearSpeed;
		else 
			targetSpeed = distance * _maxLinearSpeed / IMovement::SLOW_RADIUS;
		
		targetVelocity /= distance;
		targetVelocity *= targetSpeed;
		currentProperties.linearAccel = targetVelocity - currentProperties.linearSpeed;
		currentProperties.linearAccel/= IMovement::TIME_TO_TARGET;
		if (currentProperties.linearAccel.squaredLength() > _maxLinearAccel2) {
			currentProperties.linearAccel.normalise();
			currentProperties.linearAccel *= _maxLinearAccel;
		}
	}
}