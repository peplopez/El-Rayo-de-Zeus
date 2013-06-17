#include "LA_Attack.h"

#include "../../Logic/Entity/Components/AnimatedGraphics.h"

#include "../../Logic/Entity/Components/Combat.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/Jump.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"

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
	CLatentAction::LAStatus CLA_Attack::OnStart()
	{
		
		//Desactivación de componentes
		sleepComponents();

		switch(_initialCombatState)
		{
		case 0:	{
			CMessageBoolUShort *message = new CMessageBoolUShort();
			message->setType(Message::SET_ANIMATION);

			if (_action==Message::LIGHT_ATTACK)
			{
				message->setUShort(Logic::ATTACK1);
				_animationSetedByMe = Logic::ATTACK1;
			}
			else
			{
				message->setUShort(Logic::ATTACK2);
				_animationSetedByMe = Logic::ATTACK2;
			}

			message->setBool(false);
			_entity->emitMessage(message);

		}	break;
		
		default:
			break;
		}
		return SUSPENDED;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLA_Attack::OnStop()
	{
		awakeComponents();
		if (_entity->hasComponent<CCombat>())
			_entity->getComponent<CCombat>()->resetAttackFlags();
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
	CLatentAction::LAStatus CLA_Attack::OnRun() 
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
	CLatentAction::LAStatus CLA_Attack::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		awakeComponents();
		if (_entity->hasComponent<CCombat>())
			_entity->getComponent<CCombat>()->resetAttackFlags();
	
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
	bool CLA_Attack::accept(const CMessage *message)
	{		
		// la accion latente de ataque solo acepta mensajes de ataque en el momento que la oportunidad de combo está activada.
		return 	(message->getType() == Message::ANIMATION_FINISHED);
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_Attack::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::ANIMATION_FINISHED: //ConditionFail
			CMessageUShort* rxMsg = static_cast<CMessageUShort*>(message);
			if ( _animationSetedByMe == rxMsg->getUShort() )
			{
					finish(false);
			}
			else
			{
				switch(_initialCombatState)
				{
				case 1:	{
					CMessageBoolUShort *txMsg = new CMessageBoolUShort();
					txMsg->setType(Message::SET_ANIMATION);
					txMsg->setUShort(Logic::ATTACK2);
					txMsg->setBool(false);
					_entity->emitMessage(txMsg);		
					_animationSetedByMe=Logic::ATTACK2;		
				}	break;
			
				case 2: {

					CMessageBoolUShort *txMsg = new CMessageBoolUShort();
					txMsg->setType(Message::SET_ANIMATION);
					txMsg->setUShort(Logic::ATTACK3);
					txMsg->setBool(false);
					_entity->emitMessage(txMsg);
					_animationSetedByMe=Logic::ATTACK3;	
				} break;	
				}		
			}//else
		
		break;
		}
	}

	void CLA_Attack::tick(unsigned int msecs) 
	{
		CLatentAction::tick();
	}

	void CLA_Attack::sleepComponents()
	{
		if (_entity->hasComponent<CCombat>())
			_entity->getComponent<CCombat>()->resetAttackFlags();

		if (_entity->hasComponent<CAvatarController>())
			_entity->getComponent<CAvatarController>()->sleep();

		if (_entity->hasComponent<CJump>())
			_entity->getComponent<Logic::CJump>()->sleep();

		if (_entity->hasComponent<CBaseTraveler>())
			_entity->getComponent<CBaseTraveler>()->sleep();
	}

	void CLA_Attack::awakeComponents()
	{
		if (_entity->hasComponent<CCombat>())
			_entity->getComponent<CCombat>()->resetAttackFlags();

		if (_entity->hasComponent<CAvatarController>())
			_entity->getComponent<CAvatarController>()->awake();

		if (_entity->hasComponent<CJump>())
			_entity->getComponent<Logic::CJump>()->awake();

		if (_entity->hasComponent<CBaseTraveler>())
			_entity->getComponent<CBaseTraveler>()->awake();
	}
} //namespace LOGIC