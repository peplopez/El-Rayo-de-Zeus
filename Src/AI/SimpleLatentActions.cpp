#include "SimpleLatentActions.h"

#include "WaypointGraph.h"
#include "Server.h"

#include "BaseSubsystems/Server.h"

#include <OgreTimer.h>

namespace AI 
{
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
	CLatentAction::LAStatus CLARouteTo::OnStart()
	{	
		// TODO PRÁCTICA IA
		// Al comenzar la ejecución, la acción envía un mensaje al 
		// componente que calcula las rutas. Mientras este componente
		// se ejecuta, la acción se queda suspendida, esperando recibir
		// el mensaje de que ha llegado a destino o no se ha podido 
		// calcular la ruta.

		// Enviar un mensaje a la entidad que se ocupa de calcular rutas
		AI::CWaypointGraph *wg = AI::CServer::getSingletonPtr()->getNavigationGraph();
		int waypoints = wg->getWaypointCount();
		int targetId = rand() % waypoints;
		Vector3 target = wg->getWaypoint(targetId);
		std::cout << "Navigating to: " << target << std::endl;
		sendMoveMessage(true, target);
		return SUSPENDED;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLARouteTo::OnStop()
	{
	}

	/**
	Método invocado cíclicamente para que se continúe con la
	ejecución de la acción.
	<p>
	En este caso no hace nada.

	@return Estado de la acción tras la ejecución del método;
	permite indicar si la acción ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLARouteTo::OnRun() {
		// TODO PRÁCTICA IA
		// Si el flujo de ejecución llega a entrar en este método
		// significa que hay algo que no va bien. Se supone que 
		// la acción, nada más iniciarse se queda suspendida (por 
		// lo que no se llega a llamar a este método) y cuando 
		// recibe los mensajes pasa directamente a terminar con
		// éxito o con fallo.
		return RUNNING;
	}

	/**
	Envía un mensaje de tipo ROUTE_TO.
	*/
	void CLARouteTo::sendMoveMessage(bool walk, Vector3 target)
	{
		// Envía un mensaje para calcular una ruta
		TMessage message2;
		message2._type = Message::ROUTE_TO;
		// El bool indica si el receptor debe calcular la ruta 
		// o debe dejar de recorrer la ruta actual y parar
		message2._bool = walk;
		// Destino de la ruta. Si el parámetro anterior es false
		// este valor no se utiliza
		message2._vector3 = target;
		_entity->emitMessage(message2, NULL);
	}

	/**
	Método invocado cuando la acción ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier razón).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
	de OnStop().
	*/
	CLatentAction::LAStatus CLARouteTo::OnAbort() 
	{
		// TODO PRÁCTICA IA
		// Cuando se interrumpe una acción de seguimiento de una ruta
		// hay que enviar un mensaje al componente de seguimiento de rutas
		// para que se detenga si hay una ruta a medioseguir.

		// Enviamos un mensaje al componente de enrutamiento para que pare.
		sendMoveMessage();
		return FAIL;
	}

	/**
	Devuelve true si a la acción le interesa el tipo de mensaje
	enviado como parámetro.
	<p>
	Esta acción acepta mensajes del tipo FAILED_ROUTE y FINISHED_ROUTE

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acción está en principio interesada
	por ese mensaje.
	*/

	bool CLARouteTo::accept(const TMessage &message)
	{
		// TODO PRÁCTICA IA
		// Esta acción acepta mensajes del tipo FAILED_ROUTE y FINISHED_ROUTE

		return (message._type == Message::FAILED_ROUTE || message._type == Message::FINISHED_ROUTE);
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.
	<p>
	Si recibe FINISHED_ROUTE la acción finaliza con éxito. Si recibe
	FAILED_ROUTE finaliza con fallo.

	@param msg Mensaje recibido.
	*/
	void CLARouteTo::process(const TMessage &message)
	{
		// TODO PRÁCTICA IA
		// Si se recibe un mensaje de fallo de la ruta hay que terminar con fallo.
		// Si es de finalización de la ruta hay que terminar con éxito.
		// Para terminar una acción latente usamos el método finish (ver LatentAction.h)

		if (message._type == Message::FAILED_ROUTE) {
			finish(false);
		} else if (message._type == Message::FINISHED_ROUTE) {
			finish(true);
		}
	}

//////////////////////////////
//	Implementación de CLAWait
//////////////////////////////

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
	CLatentAction::LAStatus CLAWait::OnStart()
	{
		// TODO PRÁCTICA IA
		// Al iniciar el wait, calculamos el instante de tiempo en 
		// el que se tiene que finalizar la acción. 
		// Para obtener el instante actual podemos usar 
		// BaseSubsystems::CServer::getSingletonPtr()->getAppTimer()
		// para sacar el timer de la aplicación y usar su método 
		// getMilliseconds.

		Ogre::Timer* t = BaseSubsystems::CServer::getSingletonPtr()->getAppTimer();
		_endingTime = t->getMilliseconds() + _time;
		return RUNNING; 
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLAWait::OnStop()
	{
	}

	/**
	Método invocado cíclicamente para que se continúe con la
	ejecución de la acción.
	<p>
	En cada paso de ejecución se obtiene el tiempo actual 
	y se comprueba si se debe acabar la acción.

	@return Estado de la acción tras la ejecución del método;
	permite indicar si la acción ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLAWait::OnRun() 
	{
		// TODO PRÁCTICA IA
		// En cada paso de ejecución tendremos que comprobar si hemos
		// superado el tiempo de espera. Según lo hayamos superado o no,
		// la acción tendrá que pasar a un estado de terminado con éxito o
		// quedarse en el mismo estado en ejecución.

		Ogre::Timer* t = BaseSubsystems::CServer::getSingletonPtr()->getAppTimer();
		unsigned long currentTime = t->getMilliseconds();
		return (currentTime > _endingTime) ? SUCCESS : RUNNING;
	}

	/**
	Método invocado cuando la acción ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier razón).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
	de OnStop().
	*/
	CLatentAction::LAStatus CLAWait::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	/**
	Devuelve true si a la acción le interesa el tipo de mensaje
	enviado como parámetro.
	<p>
	Esta acción no acepta mensajes de ningún tipo.

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acción está en principio interesada
	por ese mensaje.
	*/
	bool CLAWait::accept(const TMessage &message)
	{
		// TODO PRÁCTICA IA
		// La acción no acepta mensajes
		return false;
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAWait::process(const TMessage &message)
	{
		// TODO PRÁCTICA IA
		// La acción no procesa mensajes
	}



} //namespace AI 