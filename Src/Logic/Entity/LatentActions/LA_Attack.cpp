#include "LA_Attack.h"
#include "../Components/Attack.h"


#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"

namespace Logic
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
		std::cout<<_initialCombatState<<std::endl;
		switch(_initialCombatState)
		{
			case 0:
				{
					CMessageBoolString *message = new CMessageBoolString();
					message->setType(Message::SET_ANIMATION);
					message->setString("FireKatana");
					message->setAction(Message::LIGHT_ATTACK);
					message->setBool(false);
					_entity->emitMessage(message);
					break;
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
				}
			case 1:
			{
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("GetObject");
				message->setAction(Message::LIGHT_ATTACK);
				message->setBool(false);
				_entity->emitMessage(message);		
				break;
			}
		}
		return RUNNING;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLA_Attack::OnStop()
	{
			_entity->getComponent<CAttack>()->resetAttackFlags();
	
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
	CLatentAction::LAStatus CLA_Attack::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		_entity->getComponent<CAttack>()->resetAttackFlags();
	
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
		return (_comboOportunity && (message->getType() == Message::CONTROL && 
			(message->getAction() == Message::LIGHT_ATTACK||
			message->getAction() == Message::HEAVY_ATTACK)))
			||
			((message->getType()==Message::ANIMATION_MOMENT) || (message->getType()==Message::ANIMATION_FINISHED) || (message->getType()==Message::SET_ANIMATION));
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
		case Message::ANIMATION_FINISHED:
			{
				CMessageString* maux = static_cast<CMessageString*>(message);
				if (maux->getString().compare("FireKatana")==0 )
				{		
					finish(false);
					//_lightAttack=_heavyAttack=false;//stopMovement();
				}
				else if (maux->getString().compare("GetObject")==0)
				{
					finish(false);				
				}_comboOportunity=false;
				break;
			}
		case Message::SET_ANIMATION: //con esto quiero ver si se ha cancelado una animación
			{
				if (message->getAction()==Message::WALK_LEFT || message->getAction()==Message::WALK_RIGHT)
				finish(false);
				break;
			}	
		case Message::ANIMATION_MOMENT:
			{
				_comboOportunity=true;
				break;
				//querré saber cual animación es, de momento se que solo puedo recibir un tipo de animación
				/*float punto;
				if (_entity->getSense()==Logic::LogicalPosition::RIGHT)
					punto=_entity->getDegree()-10;
				else
					punto=_entity->getDegree()+10;
					//con este metodo vemos si con la espada le estamos dando
					attackPlace(punto,_entity->getRing(),_entity->getBase(),false);*/
			}
			case Message::CONTROL: //si estamos aquí es que el jugador ha aprovechado la oportunidad que tenia de realizar un combo.
			{

				if (_comboOportunity)
					if (message->getAction()==Message::LIGHT_ATTACK)
					{
						_comboOportunity=false;
						finish(true);
						break;
					}
			}
		}
			// TODO PRÁCTICA IA
		// La acción no procesa mensajes
	}


} //namespace LOGIC