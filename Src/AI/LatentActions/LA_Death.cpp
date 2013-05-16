#include "LA_Death.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include "../../Logic/Entity/Components/Attack.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"

#include "../StateMachines/StateMachine.h"
#include "Logic/Entity/Messages/MessageAudio.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "../../Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

namespace AI
{
//////////////////////////////
//	Implementación de CLA_Death
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
	CLatentAction::LAStatus CLA_Death::OnStart()
	{
		sleepComponents();
		std::cout<<"AI::StateMachine::WTF-I am Death!!"<<std::endl;
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setUShort(Logic::DEATH);
		message->setAction(Message::WALK_STOP);
		message->setBool(false);
		_entity->emitMessage(message);

		std::string _audio = "media\\audio\\fallecimiento.wav";
		Logic::CMessageAudio *maudio=new Logic::CMessageAudio();		
		maudio->setType(Message::AUDIO);			
		maudio->setPath(_audio);
		maudio->setId("muerte");
		maudio->setPosition(_entity->getPosition());
		maudio->setNotIfPlay(false);
		maudio->setIsPlayer(false);
		_entity->emitMessage(maudio);
		
		_scene=_entity->getMap()->getGraphicScene();
		if (_entity->isPlayer())
			_scene->activateCompositor("BW");
		return SUSPENDED;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLA_Death::OnStop()
	{
		awakeComponents();
		if (_entity->isPlayer())
			_scene->deactivateCompositor("BW");
		if (_entity->isPlayer())
			Application::CBaseApplication::getSingletonPtr()->setState("gameOver"); 
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
	CLatentAction::LAStatus CLA_Death::OnRun() 
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
	CLatentAction::LAStatus CLA_Death::OnAbort() 
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
	bool CLA_Death::accept(const CMessage *message)
	{		
		return	(message->getType()==Message::ANIMATION_FINISHED);
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_Death::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::ANIMATION_FINISHED: //ConditionFail
			{
				CMessageUShort* maux = static_cast<CMessageUShort*>(message);
				if (maux->getUShort()==Logic::DEATH)
				{
				//		finish(true);
					//el finish es para cambiar a otro estado, pero de momento este el estado en el que quiero que permanezca. Otro posible estado sería desapareciendo quiza...
				}
				break;
			}
		}
	}
	
	void CLA_Death::tick(unsigned int msecs) 
	{
		CLatentAction::tick();
	}

	void CLA_Death::sleepComponents()
	{
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->sleep();
		if (_entity->getComponent<CJump>()!=NULL)
		_entity->getComponent<Logic::CJump>()->sleep();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->sleep();
	}

	void CLA_Death::awakeComponents()
	{
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->awake();
		if (_entity->getComponent<CJump>()!=NULL)
		_entity->getComponent<Logic::CJump>()->awake();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->awake();
	}

} //namespace LOGIC