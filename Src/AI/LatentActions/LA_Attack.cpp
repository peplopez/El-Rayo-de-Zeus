#include "LA_Attack.h"
#include "../../Logic/Entity/Components/Attack.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/Jump2.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"

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
		std::cout<<_initialCombatState<<std::endl;		
		std::cout<<(int)_action<<std::endl;
		switch(_initialCombatState)
		{
			case 0:
			{
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("FireKatana");
				message->setAction(_action);
				message->setBool(false);
				_entity->emitMessage(message);
				break;
			}
			case 1:
			{
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("GetObject");
				message->setAction(_action);
				message->setBool(false);
				_entity->emitMessage(message);		
				break;
			}	
			case 2:
			{
				if (_action==Message::HEAVY_ATTACK)
				{CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("INDEFINIDO");
				message->setAction(_action);
				message->setBool(false);
				_entity->emitMessage(message);	
				_initialYaw=_entity->getYaw();
				_yawAmount=0;
				}
				else
				{
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("Damage");
				message->setAction(_action);
				message->setBool(false);
				_entity->emitMessage(message);	
				//_initialYaw=_entity->getYaw();
				//_yawAmount=0;				
				}
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
	CLatentAction::LAStatus CLA_Attack::OnRun() 
	{
		if (_initialCombatState==2 && _yawAmount>=0 && _action==Message::HEAVY_ATTACK)
		{
			_yawAmount++;
			_entity->yaw(_entity->getYaw()+0.3);
			if (_yawAmount>50) 
				{
					_yawAmount=-10;
					_entity->setYaw(_initialYaw);
				/*	CMessageString *msg = new CMessageString();
					msg->setType(Message::ANIMATION_FINISHED);
					msg->setAction(_action);
					msg->setString("Death");
					_entity->emitMessage(msg);*/
					finish(false);
				}				
		}
		// TODO PRÁCTICA IA
		// En cada paso de ejecución tendremos que comprobar si hemos
		// superado el tiempo de espera. Según lo hayamos superado o no,
		// la acción tendrá que pasar a un estado de terminado con éxito o
		// quedarse en el mismo estado en ejecución.
		/*if(Application::CBaseApplication::getSingletonPtr()->getAppTime() < _endingTime)
			return RUNNING;
		else 
			return SUCCESS;*/
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
		return/* (_comboOportunity && (message->getType() == Message::CONTROL && 
			(message->getAction() == Message::LIGHT_ATTACK||
			message->getAction() == Message::HEAVY_ATTACK)))
			||*/
			/*(message->getType()==Message::ANIMATION_MOMENT) &&  _initialCombatState!=2)*/
			((message->getType()==Message::ANIMATION_FINISHED) || (message->getType()==Message::SET_ANIMATION));
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
				}
				else if (maux->getString().compare("Damage")==0)
				{
					finish(false);				
				}_comboOportunity=false;
				break;
			}
		case Message::SET_ANIMATION: //con esto quiero ver si se ha cancelado una animación
			{
				if (message->getAction()==Message::WALK_LEFT || message->getAction()==Message::WALK_RIGHT)
				{
					finish(false);					
				}
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
		/*	case Message::CONTROL: //si estamos aquí es que el jugador quiere aprovechar la oportunidad que tenia de realizar un combo. Dependiendo de 
			{

				if (_comboOportunity)
				{
					if (_initialCombatState<1)
						if (message->getAction()==Message::LIGHT_ATTACK)
						{
							_comboOportunity=false;
							finish(true);
						}					
					if (_initialCombatState==1)//en el tercer ataque del combo
						if (message->getAction()==Message::HEAVY_ATTACK)
						{
							_comboOportunity=false;
							finish(true);
						}
					}
					break;
			}*/
		}
			// TODO PRÁCTICA IA
		// La acción no procesa mensajes
	}

	void CLA_Attack::sleepComponents()
	{
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->sleep();
		if (_entity->getComponent<CJump2>()!=NULL)
		_entity->getComponent<Logic::CJump2>()->sleep();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->sleep();
	}

	void CLA_Attack::awakeComponents()
	{
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->awake();
		if (_entity->getComponent<CJump2>()!=NULL)
		_entity->getComponent<Logic::CJump2>()->awake();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->awake();
	}
} //namespace LOGIC