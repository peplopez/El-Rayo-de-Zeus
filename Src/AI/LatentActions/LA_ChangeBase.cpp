
#include "../../Logic/Entity/Components/Attack.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/Jump.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"
#include "LA_ChangeBase.h"
#include "Logic/Entity/Messages/MessageChar.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageUIntString.h"
#include "Logic/Entity/Messages/MessageUInt.h"


#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Server.h"

#include "Graphics/Scene.h"
namespace AI
{
////////////////////////////////
//	Implementaci�n de CLA_ChangeBase
////////////////////////////////

	/**
	M�todo invocado al principio de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al principio (y no durante toda la vida de la acci�n).
	<p>
	Al comenzar se obtiene el tiempo actual y se calcula el 
	tiempo en el que terminar� la acci�n mediante el atributo _time

	@return Estado de la funci�n; si se indica que la
	acci�n a terminado (LatentAction::Completed), se invocar�
	al OnStop().
	*/
	CLatentAction::LAStatus CLA_ChangeBase::OnStart()
	{		
		//Desactivaci�n de componentes
		sleepComponents();		
		_contador=1.0f;
		_actionScale=Message::Z_AXIS;
		_reloj=Application::CBaseApplication::getSingletonPtr()->getClock();
		_desencogiendo=0;
		std::cout<<"AI::StateMachine::ChangeBase"<<std::endl;		
		//_velocidad=0.01f;
		_velocidad=2*0.0001f;				
		CMessageString *m = new CMessageString();	
		m->setType(Message::SET_MATERIAL);
		m->setString("transito");
		_entity->emitMessage(m);
		
		_scene=_entity->getMap()->getGraphicScene();
	//	if (_entity->isPlayer())
		//	_scene->activateCompositor("RadialBlur");
		return RUNNING;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLA_ChangeBase::OnStop()
	{
		std::cout<<"AI::StateMachine::ChangeBase-OnStop"<<std::endl;	
	//	_reloj->removeTimeObserver(0);		
		//_reloj->removeTimeObserver(1);			
		awakeComponents();
	//	if (_entity->isPlayer())
		//	_scene->deactivateCompositor("RadialBlur");
	}

	/**
	M�todo invocado c�clicamente para que se contin�e con la
	ejecuci�n de la acci�n.
	<p>
	En cada paso de ejecuci�n se obtiene el tiempo actual 
	y se comprueba si se debe acabar la acci�n.

	@return Estado de la acci�n tras la ejecuci�n del m�todo;
	permite indicar si la acci�n ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLA_ChangeBase::OnRun() 
	{
		if (this->getStatus()!=SUCCESS && this->getStatus()!=FAIL)
			return RUNNING;
		else 
			return this->getStatus();
	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLA_ChangeBase::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		{	
			_entity->getComponent<CBaseTraveler>()->resetChangingBase();			
			//_entity->getComponent<CBaseTraveler>()->resetChangingRing();
		}
		return FAIL;
	}
	/**
	Devuelve true si a la acci�n le interesa el tipo de mensaje
	enviado como par�metro.
	<p>
	Esta acci�n no acepta mensajes de ning�n tipo.

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acci�n est� en principio interesada
	por ese mensaje.
	*/
	bool CLA_ChangeBase::accept(const CMessage *message)
	{		
		// la accion latente de ataque solo acepta mensajes de ataque en el momento que la oportunidad de combo est� activada.
		return	(message->getType()==Message::CONTROL);
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_ChangeBase::process(CMessage *message)
	{
		switch(message->getType())
		{			
			case Message::CONTROL:
			{								
				CMessageUInt *m = new CMessageUInt();	
				m->setType(Message::SET_INITIAL_MATERIAL);
				m->setUInt(1);
				_entity->emitMessage(m);	
				//finish(false);
			}
		}
	}

	//PeP
	void CLA_ChangeBase::tick(unsigned int msecs) 
	{
		CMessageFloat *m = new CMessageFloat();	
		m->setType(Message::SET_SCALE);
		m->setAction(_actionScale);
		if (_actionScale==Message::Z_AXIS)
		{
			if (_desencogiendo==0)

			if (_contador>0.3)
			{
				_contador=_contador + (-(_velocidad*msecs)+(_desencogiendo*(_velocidad*msecs)*2));		
				m->setFloat(_contador);
				_entity->emitMessage(m);
			}
			else
			{
				_actionScale=Message::Y_AXIS;
				_contador=1.0f;
				m->setAction(_actionScale);
			}

			else

			if (_contador<1)
			{
				_contador=_contador + (-(_velocidad*msecs)+(_desencogiendo*(_velocidad*msecs)*2));		
				m->setFloat(_contador);
				_entity->emitMessage(m);
			}
			else
			{
				_actionScale=Message::Y_AXIS;
				_contador=1.0f;
				m->setAction(_actionScale);				
				timeArrived();//aqu� es el final
			}
		}
		if (_actionScale==Message::Y_AXIS)
		{
			if (_desencogiendo==0)
	
				if (_contador>0.3)
				{							
					_contador=_contador + (-(_velocidad*msecs)+(_desencogiendo*(_velocidad*msecs)*2));
					m->setFloat(_contador);
					_entity->emitMessage(m);
				}
				else
				{
					_contador=0.0f;
					_actionScale=Message::Y_AXIS;
					m->setAction(_actionScale);	
					timeArrived();
				}
			else
				if (_contador<1)
				{							
					_contador=_contador + (-(_velocidad*msecs)+(_desencogiendo*(_velocidad*msecs)*2));
					m->setFloat(_contador);
					_entity->emitMessage(m);
				}
				else
				{
					_contador=0.3f;
					_actionScale=Message::Z_AXIS;
					m->setAction(_actionScale);	
				}
		}
		CLatentAction::tick();//no olvideis llamar al tick de CLatentAction

	}

	void CLA_ChangeBase::sleepComponents()
	{
		if (_entity->getComponent<CAvatarController>()!=NULL)
			_entity->getComponent<CAvatarController>()->sleep();
		if (_entity->getComponent<CJump>()!=NULL)
			_entity->getComponent<CJump>()->sleep();
	}


	void CLA_ChangeBase::awakeComponents()
	{
		if (_entity->getComponent<CAvatarController>()!=NULL)
			_entity->getComponent<CAvatarController>()->awake();
		if (_entity->getComponent<CJump>()!=NULL)
			_entity->getComponent<CJump>()->awake();

	}
	void CLA_ChangeBase::timeArrived()
	{//El primer if es para ignorar eventos externos si no estoy en el estado
		if (this->getStatus()!=SUCCESS && this->getStatus()!=FAIL && this->getStatus()!=RUNNING)
		{//cancelando viaje
			return;
		}
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		{			
			_entity->getComponent<CBaseTraveler>()->changeBase();
		}
		if (_desencogiendo==0) 
			_desencogiendo=1;
		else
		{
			_desencogiendo=0;
			finish(true);
		}
	}
	
} //namespace LOGIC