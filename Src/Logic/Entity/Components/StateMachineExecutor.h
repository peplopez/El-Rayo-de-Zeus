/**
@file StateMachineExecutor.h

Contiene la declaración de la clase CStateMachineExecutor, que se encarga de
ejecutar máquinas de estado de la clase CStateMachine.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __Logic_StateMachineExecutor_H
#define __Logic_StateMachineExecutor_H

#include "Logic/Entity/Component.h"
#include "AI/StateMachine.h"

namespace Logic
{
	/**
	Componente que se encarga de ejecutar máquinas de estado.
	*/
	class CStateMachineExecutor : public IComponent
	{
		DEC_FACTORY(CStateMachineExecutor);
	
	public:
		/**
		Constructor
		*/
		CStateMachineExecutor(void) : _currentStateMachine(0), _currentAction(0) {};
		/**
		Destructor
		*/
		~CStateMachineExecutor(void) { 
			if (_currentStateMachine != 0) delete _currentStateMachine;
		};
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		behavior, que indica el nombre de la máquina de estado a ejecutar.

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
		Este método actualiza la máquina de estado. Si hay un cambio de estado, 
		se actualiza el valor del atributo _currentAction, que es el que contiene
		la acción latente que se está ejecutando. Por último, se llama al tick de
		la acción latente para que avance su ejecución.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);
		/**
		Este método delega en el método accept de la acción latente que se 
		está ejecutando (_currentAction).
		*/
		virtual bool accept(const TMessage &message);
		/**
		Este método delega en el método process de la acción latente que se 
		está ejecutando (_currentAction).
		*/
		virtual void process(const TMessage &message);

	private:
		/**
		Almacena la máquina de estado que se está ejecutando
		*/
		AI::CStateMachine<AI::CLatentAction> * _currentStateMachine;
		/**
		Acción que se está ejecutando.
		*/
		AI::CLatentAction* _currentAction;

	}; // class CStateMachineExecutor 

	REG_FACTORY(CStateMachineExecutor);

}// namespace Logic

#endif // __Logic_StateMachineExecutor_H
