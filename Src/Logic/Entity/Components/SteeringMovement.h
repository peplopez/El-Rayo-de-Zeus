/**
@file SteeringMovement.h

Contiene la declaración de la clase CSteringMovement. Se encarga de
mover un avatar utilizando movimientos cinéticos o dinámicos.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __Logic_SteeringMovement_H
#define __Logic_SteeringMovement_H

#include "Logic/Entity/Component.h"
#include "AI/Movement.h"
#include "AI/Server.h"
#include "AI/KinematicMovement.h"

namespace Logic 
{
	/**
	Componente que se encarga de trasladar una entidad de un punto a otro
	usando SteeringMovements. 
	Acepta mensajes del tipo MOVE_TO. Los parámetros relevantes del mensaje
	son el _vector, que indica el destino del movimiento, y el _int que indica 
	el tipo de movimiento (ver los tipos en IMovement).
	*/
	class CSteeringMovement : public IComponent
	{
		DEC_FACTORY(CSteeringMovement);

	public:
		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CSteeringMovement() : IComponent(), _arrived(false), _maxLinearSpeed(0.05f), _maxAngularSpeed(0.01f),
			_maxLinearAccel(0.001f), _maxAngularAccel(0.001f), _currentMovement(0), 
			_currentMovementType(AI::IMovement::MOVEMENT_NONE), _yaw(0) { };
		/**
		Destructor
		*/
		~CSteeringMovement(void);
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa los atributos
		speed, angularSpeed, accel y angularAccel, que indican los valores máximos 
		de velocidad y aceleración lineales y angulares.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es necesario.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);
		/**
		Este componente sólo acepta mensajes de tipo MOVE_TO.
		*/
		virtual bool accept(const TMessage &message);
		/**
		Si se recibe un mensaje de tipo MOVE_TO, almacena el vector como 
		destino del movimiento y el tipo de movimiento.
		*/
		virtual void process(const TMessage &message);


	private:
		/** 
		Valores máximos de velocidades y aceleraciones.
		*/
		float _maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel;
		/**
		Este campo indica si se ha alcanzado el objetivo
		*/
		bool _arrived;
		/**
		Destino del movimiento.
		*/
		Vector3 _currentTarget, _target;
		/**
		Tipo de movimiento.
		*/
		int _currentMovementType, _movType;
		/**
		Instancia del movimiento actual.
		*/
		AI::IMovement* _currentMovement;
		/**
		Instancia de un movimiento de giro que se utilizará para mantener la dirección.
		*/
		AI::IMovement* _yaw;
		/**
		Propiedades actuales del movimiento. En esta variable se almacenan las velocidades actuales, que 
		serán consultadas por los movimientos dinámicos */
		AI::IMovement::DynamicMovement _currentProperties;

		/**
		Envía un mensaje para cambiar la animación actual.

		@param animation Nueva animación.
		*/
		void sendAnimationMessage(std::string animation);

	};

	REG_FACTORY(CSteeringMovement);

} //namespace Logic

#endif //__Logic_SteeringMovement_H
