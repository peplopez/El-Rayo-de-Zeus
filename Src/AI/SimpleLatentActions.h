/**
@file CSimpleLatentActions.h

En este fichero se implementan algunas acciones 
latentes básicas.

@author Gonzalo Flórez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_SimpleLatentActions_H
#define __AI_SimpleLatentActions_H

#include "LatentAction.h"

#include "Logic/Entity/Entity.h"

using namespace Logic;

namespace AI 
{

	/**
	Acción latente que sigue una ruta desde el punto actual
	hasta un punto aleatorio del mapa de Galeon.
	<p>
	Utiliza el componente CRouteTo para calcular la ruta y 
	moverse.
	<p>
	Acepta mensajes de tipo FAILED_ROUTE y FINISHED_ROUTE, 
	que indican si debe terminar con fallo o con éxito.
	*/
	class CLARouteTo : public CLatentAction
	{
	public:
		/**
		Constructor.
		*/
		CLARouteTo(CEntity* entity) : CLatentAction(), _entity(entity) {};
		/**
		Destructor.
		*/
		~CLARouteTo() {};
		/**
		Devuelve true si a la acción le interesa el tipo de mensaje
		enviado como parámetro.
		<p>
		Esta acción acepta mensajes del tipo FAILED_ROUTE y FINISHED_ROUTE

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acción está en principio interesada
		por ese mensaje.
		*/
		virtual bool accept(const TMessage &message);
		/**
		Procesa el mensaje recibido. El método es invocado durante la
		ejecución de la acción cuando se recibe el mensaje.
		<p>
		Si recibe FINISHED_ROUTE la acción finaliza con éxito. Si recibe
		FAILED_ROUTE finaliza con fallo.

		@param msg Mensaje recibido.
		*/
		virtual void process(const TMessage &message);

	protected:
		/**
		Entidad que ejecuta la acción.
		*/
		CEntity* _entity;
		/**
		Método invocado al principio de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al principio (y no durante toda la vida de la acción).
		<p>
		En este caso, se debe enviar un mensaje al componente 
		CRouteTo y cambiar al estado SUSPENDED.

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
		En este caso no hace nada.

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
		Envía un mensaje de tipo ROUTE_TO.
		*/
		void sendMoveMessage(bool walk = false, Vector3 target = Vector3::ZERO);
	};

	/**
	Esta acción espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLAWait : public CLatentAction
	{
	public:
		/**
		Constructor.
		
		@param time Tiempo de espera
		*/
		CLAWait(unsigned long time) : CLatentAction(), _time(time) {};
		/**
		Destructor
		*/
		~CLAWait() {};

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
		virtual bool accept(const TMessage &message);
		/**
		Procesa el mensaje recibido. El método es invocado durante la
		ejecución de la acción cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void process(const TMessage &message);

	protected:
		/**
		Tiempo de espera
		*/
		unsigned long _time;
		/**
		Tiempo en el que se termina la acción
		*/
		unsigned long _endingTime;
	};

} //namespace AI 

#endif // __AI_SimpleLatentActions_H