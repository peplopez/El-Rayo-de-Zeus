#include "LA_AltarSwitch.h"
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
//	Implementación de CLA_AltarSwitch
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
	CLatentAction::LAStatus CLA_AltarSwitch::OnStart()
	{
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setUShort(Logic::ACTIVATE_ALTAR);
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
	void CLA_AltarSwitch::OnStop()
	{
		CMessageBoolUShort *message = new CMessageBoolUShort(); //anulado
		message->setType(Message::SET_ANIMATION);		
		message->setUShort(Logic::IDLE);
		message->setBool(true);
		//_entity->emitMessage(message);
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
		CLatentAction::LAStatus CLA_AltarSwitch::OnRun() 
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
		CLatentAction::LAStatus  CLA_AltarSwitch::OnAbort() 
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
	bool CLA_AltarSwitch::accept(const CMessage *message)
	{		
		// La acción no acepta mensajes
		return message->getType() == Message::ALTAR_MS_ORDER &&
				message->getAction() == Message::FINISH_SUCCESS;
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_AltarSwitch::process(CMessage *message)
	{
		finish(true);
	}
	
	void CLA_AltarSwitch::tick(unsigned int msecs) 
	{		
		CLatentAction::tick();
	}

	void CLA_AltarSwitch::sleepComponents(){}

	void CLA_AltarSwitch::awakeComponents(){}
} //namespace LOGIC