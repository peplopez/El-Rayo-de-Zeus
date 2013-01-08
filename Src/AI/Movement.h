/**
@file Movement.h

Declaración de la interfaz IMovement, que define los 
métodos comunes de las clases de movimiento cinemático 
y dinámico.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __AI_Movement_H
#define __AI_Movement_H

#include "Logic/Entity/Entity.h"


using namespace Logic;

namespace AI 
{
	/**
	En esta interfaz se declaran los métodos comunes para las clases que
	implementarán el movimiento, tanto dinámico como cinemático.
	*/
	class IMovement 
	{
	public:
		/**
		Constante en la que se almacena un valor de tiempo que medirá los 
		milisegundos que queremos que transcurran hasta alcanzar el destino de un
		movimiento.
		*/
		const static float TIME_TO_TARGET;
		/**
		Constante en la que se almacena un valor de distancia que medirá a qué 
		distancia se empieza a ralentizar el movimiento para llegar a un destino.
		*/
		const static float SLOW_RADIUS;

		/**
		Esta estructura se utiliza como entrada y salida de los métodos de movimiento.
		*/
		struct DynamicMovement {
			Vector3 linearSpeed;
			double angularSpeed;
			Vector3 linearAccel;
			double angularAccel;

			DynamicMovement() : linearSpeed(Vector3::ZERO), angularSpeed(0), 
				linearAccel(Vector3::ZERO), angularAccel(0) {};
		};

		/**
		Distintos tipos de movimiento para el método de factoría.
		*/
		enum EnmMovementType {
			MOVEMENT_NONE = 0,
			MOVEMENT_KINEMATIC_SEEK,
			MOVEMENT_KINEMATIC_ARRIVE,
			MOVEMENT_KINEMATIC_ALIGN_TO_SPEED,
			MOVEMENT_DYNAMIC_SEEK,
			MOVEMENT_DYNAMIC_ARRIVE
		};

		/** 
		Constructor.

		@param maxLinearSpeed Velocidad lineal máxima.
		@param maxAngularSpeed Velocidad angular máxima.
		@param maxLinearAccel Aceleración lineal máxima.
		@param maxAngularAccel Aceleración angular máxima.
		*/
		IMovement(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
			_entity(0), _maxLinearSpeed(maxLinearSpeed), _maxAngularSpeed(maxAngularSpeed), 
				_maxLinearAccel(maxLinearAccel), _maxAngularAccel(maxAngularAccel), 
				_maxLinearSpeed2(maxLinearSpeed * maxLinearSpeed), _maxLinearAccel2(maxLinearAccel * maxLinearAccel) {};
		/**
		Destructor
		*/
		~IMovement() {};
		/**
		Establece cuál es la entidad que se está moviendo.
		*/
		void setEntity(CEntity* entity) { _entity = entity; };
		/** 
		Establece la posición de destino del movimiento.
		*/
		void setTarget(Vector3 &target) { _target = target; };
		/**
		Método virtual que tendrán que implementar todos los movimientos.
		
		@param msecs Duración del movimiento.
		@param currentProperties Parámetro de entrada/salida en el que se pasan las velocidades
		y aceleraciones actuales y se devuelven modificadas de acuerdo al tipo de movimiento.
		*/
		virtual void move(unsigned int msecs, DynamicMovement& currentProperties) = 0;
		/**
		Método de factoría que devuelve un movimiento a partir de un enumerado.
		*/
		static IMovement* getMovement(int type, float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel);

	protected:
		/**
		Entidad que se mueve. En general la vamos a usar para obtener la posición.
		*/
		CEntity* _entity;
		/**
		Destino del movimiento
		*/
		Vector3 _target;
		/** 
		Límites de velocidad y aceleración lineales y angulares 
		*/
		float _maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel;
		/** 
		Límites de velocidad y aceleración cuadráticos. Se pueden usar para compararlos 
		con la distancia al cuadrado, que es más barata de calcular que la distancia normal.
		*/
		float _maxLinearSpeed2, _maxLinearAccel2;


	}; // class IMovement 

} //namespace AI 

#endif //__AI_Movement_H