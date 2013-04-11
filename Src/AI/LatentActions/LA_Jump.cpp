#include "LA_Jump.h"

#include "Application/BaseApplication.h"
#include "../StateMachines/StateMachine.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"

namespace AI
{


//////////////////////////////
//	Implementación de CLA_Jump
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
	CLatentAction::LAStatus CLA_Jump::OnStart()
	{
				//Desactivación de componentes
		sleepComponents();
		std::cout<<"AI::StateMachine::Jumping"<<std::endl;
		
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setAction(Message::JUMP);				
		message->setString("jump");
		message->setBool(true);
		_entity->emitMessage(message);		

		return RUNNING;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLA_Jump::OnStop()
	{
				//Reactivación de componentes
	
		awakeComponents();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		{
			_entity->getComponent<CBaseTraveler>()->resetChangingBase();		
			_entity->getComponent<CBaseTraveler>()->resetChangingRing();
		}
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
	CLatentAction::LAStatus CLA_Jump::OnRun() 
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
	CLatentAction::LAStatus CLA_Jump::OnAbort() 
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
	bool CLA_Jump::accept(const CMessage *message)
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
	void CLA_Jump::process(CMessage *message)
	{
		// TODO PRÁCTICA IA
		// La acción no procesa mensajes
		/*if(message->getType() == Message::CONTROL)
			 if ( message->getAction()==Message::LIGHT_ATTACK)
				finish(true); // bool = exito o fallo
			 else
				finish(false);*/
	}
	
	void CLA_Jump::tick(unsigned int msecs) 
	{
		CLatentAction::tick();
	}

	void CLA_Jump::sleepComponents()
	{
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
			_entity->getComponent<CBaseTraveler>()->sleep();
	}

	void CLA_Jump::awakeComponents()
	{	
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
			_entity->getComponent<CBaseTraveler>()->awake();
	}
} //namespace LOGIC