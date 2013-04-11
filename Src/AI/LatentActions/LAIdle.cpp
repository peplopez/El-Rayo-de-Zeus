#include "LAIdle.h"

#include "Application/BaseApplication.h"
#include "../../Logic/Entity/Components/Attack.h"

#include "../StateMachines/StateMachine.h"

#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageString.h"
namespace AI
{


//////////////////////////////
//	Implementación de CLAIdle
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
	CLatentAction::LAStatus CLAIdle::OnStart()
	{
		
		//while(
		
		std::cout<<"AI::StateMachine::idle"<<std::endl;
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setString("idle");
		message->setAction(Message::WALK_STOP);
		message->setBool(true);
		_entity->emitMessage(message);

		CMessageFloat *m2 = new CMessageFloat();	
		m2->setType(Message::SET_SCALE);
		m2->setFloat(1.0f);
		m2->setAction(Message::UNDEF);
		_entity->emitMessage(m2);

		//
		if (_entity->getComponent<CAttack>()!=NULL)
				_entity->getComponent<CAttack>()->setCovering(false);		
		// TODO PRÁCTICA IA
		// Al iniciar el wait, calculamos el instante de tiempo en 
		// el que se tiene que finalizar la acción. 
		// El tiempo de espera se guarda en _time.
		// Para obtener el instante actual podemos usar 
		// Application::CBaseApplication::getSingletonPtr()->getAppTime()
		//
		//_endingTime = Application::CBaseApplication::getSingletonPtr()->getAppTime() + _time; 
		// Otra opción es usar
		// BaseSubsystems::CServer::getSingletonPtr()->getAppTimer()
		// para sacar el timer de la aplicación y usar su método 
		// getMilliseconds.

		return RUNNING;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLAIdle::OnStop()
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
	CLatentAction::LAStatus CLAIdle::OnRun() 
	{
		// TODO PRÁCTICA IA
		// En cada paso de ejecución tendremos que comprobar si hemos
		// superado el tiempo de espera. Según lo hayamos superado o no,
		// la acción tendrá que pasar a un estado de terminado con éxito o
		// quedarse en el mismo estado en ejecución.
		/*if(Application::CBaseApplication::getSingletonPtr()->getAppTime() < _endingTime)
			return RUNNING;
		else 
			return SUCCESS;*/

		return RUNNING;
	}

	/**
	Método invocado cuando la acción ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier razón).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
	de OnStop().
	*/
	CLatentAction::LAStatus CLAIdle::OnAbort() 
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
	bool CLAIdle::accept(const CMessage *message)
	{		
		// La acción no acepta mensajes
		return false;
		return (message->getType() == Message::CONTROL && (message->getAction()==Message::LIGHT_ATTACK || message->getAction()==Message::HEAVY_ATTACK));
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAIdle::process(CMessage *message)
	{
		// TODO PRÁCTICA IA
		// La acción no procesa mensajes
		/*if(message->getType() == Message::CONTROL)
			 if ( message->getAction()==Message::LIGHT_ATTACK)
				finish(true); // bool = exito o fallo
			 else
				finish(false);*/
	}
	
	void CLAIdle::tick(unsigned int msecs) 
	{
		CLatentAction::tick();
	}

	void CLAIdle::sleepComponents(){}

	void CLAIdle::awakeComponents(){}
} //namespace LOGIC