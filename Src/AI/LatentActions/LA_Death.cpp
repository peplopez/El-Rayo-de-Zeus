#include "LA_Death.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include "../../Logic/Entity/Components/Combat.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "../../Logic/Entity/Components/PhysicalCharacter.h"

#include "../StateMachines/StateMachine.h"
#include "Logic/Entity/Messages/MessageAudio.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "../../Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"
#include "Logic/Maps/EntityFactory.h"

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

		std::cout<<"AI::StateMachine::WTF-I am death!! - "<< _entity->getName() << " - " << std::endl;

		//PT
		unsigned int currentTime = Application::CBaseApplication::getSingletonPtr()->getAppTime();
		_endingTime = currentTime + _time * 1000000;

		//init Respawn Layout and functions

		if (_entity->isPlayer()){

			std::cout<<"AI::StateMachine::WTF-I am death!! - I AM PLAYER " << std::endl;
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideHud"); //oculto el HUD
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideShop"); //oculto el SHOP

			ScriptManager::CServer::getSingletonPtr()->loadExeScript("RespawnPlayer");
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("initRespawn");
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showRespawn");

		}

		//message to changing to death model

		std::cout<<"AI::StateMachine::WTF-I am death!! - CHANGING TO DEATH MODEL " << std::endl;
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setUShort(Logic::DEATH);
		message->setAction(Message::WALK_STOP);
		message->setBool(false);
		_entity->emitMessage(message);
		std::cout<<"AI::StateMachine::WTF-I am death!! - SENDING MESSAGE TO CHANGING TO DEATH MODEL " << std::endl;

			//sleepComponents();

		//message to hearing death sound
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

		//for showing a black and white screen when player is death
		if (_entity->isPlayer())
			_scene->compositorEnable("BW");

		//PT
		//return SUSPENDED;
		return RUNNING;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLA_Death::OnStop()
	{

		//PT
		if (_entity->isPlayer())
		{
			std::cout<<"AI::StateMachine::OnStop - "<< _entity->getName() << " - " << std::endl;
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideRespawn"); //escondo la pantalla de respawn
			_scene->compositorDisable("BW"); //desactivo el compositor blanco y negro
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showHud"); //muestro el HUD
			//ScriptManager::CServer::getSingletonPtr()->executeProcedure("showShop"); //muestro el SHOP

			//PT. In case we were in Shop when we died (PlayerController and CameraController are deactivated
			//and when we Respawn we lost Player Controls.

			//Send msg
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::SHOP);
			m->setAction(Logic::Message::DEACTIVATE_SHOP);
			_entity->emitMessage(m);


			//respawn player
			respawn();

		}

		if(_entity->getType()=="NPC")
			respawn();

		awakeComponents();

		finish(true);

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


		//PT
		if (_entity->isPlayer()){
			if(Application::CBaseApplication::getSingletonPtr()->getAppTime() > _endingTime)
			{
				ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateTime",0);
				return SUCCESS;
			}
			else
			{
				int timeToRevive = (int) ((_endingTime-Application::CBaseApplication::getSingletonPtr()->getAppTime()) * 0.000001f);
				ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateTime",timeToRevive);
				return RUNNING;
			}
		}
		else if(_entity->getType()=="NPC")
		{
			if(Application::CBaseApplication::getSingletonPtr()->getAppTime() > _endingTime)
				return SUCCESS;
			else
				return RUNNING;
		}

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
					sleepComponents();
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
		if (_entity->hasComponent<CCombat>())
			_entity->getComponent<CCombat>()->resetAttackFlags();
		if (_entity->hasComponent<CAvatarController>())
			_entity->getComponent<CAvatarController>()->sleep();
		if (_entity->hasComponent<CBaseTraveler>())
			_entity->getComponent<CBaseTraveler>()->sleep();
		if (_entity->hasComponent<CPhysicalCharacter>())
			_entity->getComponent<CPhysicalCharacter>()->sleep();
		if (_entity->hasComponent<CJump>())
			_entity->getComponent<CJump>()->sleep();
	}

	void CLA_Death::awakeComponents()
	{
		if (_entity->hasComponent<CCombat>())
			_entity->getComponent<CCombat>()->resetAttackFlags();
		if (_entity->hasComponent<CAvatarController>())
			_entity->getComponent<CAvatarController>()->awake();
		if (_entity->hasComponent<CJump>())
			_entity->getComponent<CJump>()->awake();
		if (_entity->hasComponent<CBaseTraveler>())
			_entity->getComponent<CBaseTraveler>()->awake();
		if (_entity->hasComponent<CPhysicalCharacter>())
			_entity->getComponent<CPhysicalCharacter>()->awake();
	}

	void CLA_Death::respawn()
	{
			//PT Respawneo en la base Origen
			if (_entity->hasComponent<CBaseTraveler>())	
				_entity->getComponent<CBaseTraveler>()->respawnInBaseOrigin();

			//PT mando mensaje al componente LIFE para restaurar la vida al máximo
			CMessageBoolUShort *message = new CMessageBoolUShort();
			message->setType(Message::LIFE_RESTORE);
			_entity->emitMessage(message);
	}

} //namespace LOGIC