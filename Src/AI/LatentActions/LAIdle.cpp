#include "LAIdle.h"
#include "../../Logic/Entity/Components/AnimatedGraphics.h"
#include "Application/BaseApplication.h"

#include "../../Logic/Entity/Components/Combat.h"

#include "../StateMachines/StateMachine.h"

#include "Logic/Entity/Messages/MessageBoolUShort.h"
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
		
		/*if (_anim=="")
			_anim = Graphics::AnimNames::IDLE;
		*/std::cout<<"AI::StateMachine::idle"<<std::endl;
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);

		//si es sinbad
		message->setUShort(Logic::IDLE);
		message->setAction(Message::WALK_STOP);
		message->setBool(true);
		_entity->emitMessage(message);

		
		if (_entity->getComponent<CCombat>()!=NULL)
				_entity->getComponent<CCombat>()->setCovering(false);		

		return SUSPENDED;
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
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAIdle::process(CMessage *message){}
	
	void CLAIdle::tick(unsigned int msecs) 
	{		
		CLatentAction::tick();
	}

	void CLAIdle::sleepComponents(){}

	void CLAIdle::awakeComponents(){}
} //namespace LOGIC