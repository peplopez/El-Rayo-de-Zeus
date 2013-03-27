/**
@file CLA_Idle.h

En este fichero se implementan algunas acciones 
latentes básicas.

@author Jose Luis Löpez
@date Marzo 2013
*/

#pragma once

#ifndef __AI_CHANGELatentActions_H
#define __AI_CHANGELatentActions_H

#include "LatentAction.h"
#include "../../Application/Clock.h"
#include "Logic/Entity/Entity.h"

using namespace Logic;

namespace AI 
{

	/**
	Esta acción espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLA_Change : public CLatentAction, public Application::IClockListener
	{
	public:
		/**
		Constructor.
		
		@param time Tiempo de espera
		*/
		CLA_Change(CEntity* entity, Message::TActionType action) : CLatentAction(),_maxChangingRingTime(1500),_maxChangingBaseTime(2500) {this->setEntity(entity);_action=action;};
		/**
		Destructor
		*/
		~CLA_Change() {};

		/**
		Método invocado al principio de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al principio (y no durante toda la vida de la acción).
		<p>
		Al comenzar se obtiene el tiempo actual y se calcula el 
		tiempo en el que terminará la acción mediante el atributo _time

		@return Estado de la función; si se indica que la
		acción a terminado (LatentAction::Completed), se invocará
		al OnStop().
		*/
		virtual LAStatus OnStart();

		/**
		Método invocado al final de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al final (y no durante toda la vida de la acción).

		En la mayoría de los casos este método no hace nada.
		*/
		virtual void OnStop();

		/**
		Método invocado cíclicamente para que se continúe con la
		ejecución de la acción.
		<p>
		En cada paso de ejecución se obtiene el tiempo actual 
		y se comprueba si se debe acabar la acción.

		@return Estado de la acción tras la ejecución del método;
		permite indicar si la acción ha terminado o se ha suspendido.
		*/
		virtual LAStatus OnRun() ;

		/**
		Método invocado cuando la acción ha sido cancelada (el comportamiento
		al que pertenece se ha abortado por cualquier razón).

		La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada".

		@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
		de OnStop().
		*/
		virtual LAStatus OnAbort() ;
		/**
		Devuelve true si a la acción le interesa el tipo de mensaje
		enviado como parámetro.
		<p>
		Esta acción no acepta mensajes de ningún tipo.

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acción está en principio interesada
		por ese mensaje.
		*/

		virtual bool accept(const CMessage *message);
		/**
		Procesa el mensaje recibido. El método es invocado durante la
		ejecución de la acción cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void process( CMessage *message);
		
		virtual void sleepComponents();

		virtual void awakeComponents();
				////////////////////////////////////////
		// Métodos de IClockListener //
		////////////////////////////////////////
		/**
		Método que será invocado siempre que se termine una animación.
		Las animaciones en cíclicas no invocarán nunca este método.

		@param animation Nombre de la animación terminada.
		*/
		void timeArrived();

	protected:
		
		unsigned short _initialCombatState;

		Message::TActionType _action;

		float _maxChangingBaseTime;

		float _maxChangingRingTime;
		Application::IClock* _reloj;
	};

} //namespace AI 

#endif // __AI_CHANGELatentActions_H