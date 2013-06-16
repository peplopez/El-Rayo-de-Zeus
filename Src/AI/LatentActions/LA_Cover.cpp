#include "LA_Cover.h"
#include "Logic/Entity/Components/Combat.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/AnimatedGraphics.h"
#include "../StateMachines/StateMachine.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"

namespace AI
{


//////////////////////////////
//	Implementación de CLA_Cover
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
	CLatentAction::LAStatus CLA_Cover::OnStart()
	{		
		//Desactivación de componentes
		sleepComponents();		
		std::cout<<"AI::StateMachine::Cover"<<std::endl;

		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setUShort(Logic::COVER_WITH_SHIELD );
		message->setBool(false);
		_entity->emitMessage(message);			
		return SUSPENDED;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLA_Cover::OnStop()
	{
		awakeComponents();
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
	CLatentAction::LAStatus CLA_Cover::OnRun() 
	{
		if (this->getStatus()!=SUCCESS && this->getStatus()!=FAIL)
			return RUNNING;
		else 
			return this->getStatus();
	}

	/**
	Método invocado cuando la acción ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier razón).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
	de OnStop().
	*/
	CLatentAction::LAStatus CLA_Cover::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo	
		awakeComponents();
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
	bool CLA_Cover::accept(const CMessage *message)
	{		
		return false; 
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_Cover::process(CMessage *message)
	{		
	}
	
	void CLA_Cover::tick(unsigned int msecs) 
	{
		CLatentAction::tick();
	}

	void CLA_Cover::sleepComponents()
	{
		if (_entity->hasComponent<CAvatarController>())
				_entity->getComponent<CAvatarController>()->sleep();	
		if (_entity->hasComponent<CJump>())
				_entity->getComponent<CJump>()->sleep();		
	}

	void CLA_Cover::awakeComponents()
	{ 
		if (_entity->hasComponent<CAvatarController>())
				_entity->getComponent<CAvatarController>()->awake();
		if (_entity->hasComponent<CJump>())
				_entity->getComponent<CJump>()->awake();	
		if (_entity->hasComponent<CCombat>())
				_entity->getComponent<CCombat>()->setCovering(false);		
	}

	
} //namespace LOGIC