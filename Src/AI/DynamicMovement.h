/**
@file DynamicMovement.h

Contiene distintas clases que heredan de IMovement y que 
implementan distintos tipos de movimiento dinámico.

@author Gonzalo Flórez
@date Diciembre, 2010
*/

#pragma once

#ifndef __AI_DynamicMovement_H
#define __AI_DynamicMovement_H

#include "movement.h"

namespace AI 
{

	/**
	Movimiento Seek. 
	Intenta llegar a un punto de destino empleando siempre la máxima aceleración posible. 
	En realidad lo más probable es que el movimiento nunca llegue al destino sino que se pase de 
	largo, de la vuelta y permanezca oscilando hasta que hagamos que se detenga.
	*/
	class CDynamicSeek : public IMovement
	{
	public:

		/**
		Constructor
		*/
		CDynamicSeek(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
		  IMovement(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel) { };
		/**
		Efectúa el movimiento.
		
		@param msecs Tiempo que dura el movimiento.
		@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
		en él devuelve los nuevos valores de aceleración.
		*/
		void move(unsigned int msecs, DynamicMovement& currentProperties);

	}; // class CDynamicSeek

	/**
	Movimiento Arrive. 
	Intenta llegar a un punto de destino disminuyendo la velocidad según se acerca. De esta 
	manera nos aseguramos de que llegue sin pasarse de largo.
	*/
	class CDynamicArrive : public IMovement
	{
	public:

		/**
		Constructor
		*/
		CDynamicArrive(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
			IMovement(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel) { };
		/**
		Efectúa el movimiento.
		
		@param msecs Tiempo que dura el movimiento.
		@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
		en él devuelve los nuevos valores de aceleración.
		*/
		void move(unsigned int msecs, DynamicMovement& currentProperties);

	}; // class CDynamicArrive

} //namespace AI 

#endif // __AI_DynamicMovement_H
