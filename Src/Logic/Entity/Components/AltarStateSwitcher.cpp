/**
@file AltarStateSwitcher.cpp

Contiene la implementación del componente que controla la 
capacidad de un Character de activar/desactivar altares
 
@see Logic::CAltarStateSwitcher
@see Logic::IComponent

@author Emilio Santalla
@date Emilio Santalla, 2013
*/

#include "AltarStateSwitcher.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"

#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Entity/Components/StateMachineExecutor.h"



#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::PHYSIC_CHARACTER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic 
{
	IMP_FACTORY(CAltarStateSwitcher);
	
	//---------------------------------------------------------

	bool CAltarStateSwitcher::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;				
	

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CAltarStateSwitcher::activate()
	{
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAltarStateSwitcher::deactivate()
	{
	} // deactivate
	
	//---------------------------------------------------------

	bool CAltarStateSwitcher::accept(const CMessage *message)
	{
		return (message->getType() == Message::TRIGGER ||
					message->getType() == Message::CONTROL ||
						message->getType() == Message::ALTAR_SWITCHED );

	} // accept
	
	//---------------------------------------------------------

	void CAltarStateSwitcher::process(CMessage *message)
	{
		switch(message->getType())
		{
		
		case Message::TRIGGER:
			if(message->getAction() == Message::TRIGGER_ENTER) 
			{
				CEntity *auxTarget = _entity->getMap()->getEntityByID(static_cast<CMessageUInt*>(message)->getUInt());
				if(auxTarget->getType() == "Altar")
				{
					_switchingAllowed = true;
					_target = auxTarget;
				}
			}
			else if(message->getAction() == Message::TRIGGER_EXIT)
			{

				CEntity *auxTarget = _entity->getMap()->getEntityByID(static_cast<CMessageUInt*>(message)->getUInt());
				if(auxTarget && auxTarget->getType() == "Altar")
				{	
					if(_switchingState)
						stopSwitchingState();
					_switchingAllowed = false;
					_target = 0;
				}
			}
			break;
		
		case Message::CONTROL:

			if(message->getAction() == Message::SWITCH_ALTAR)
			{
				if (_entity->hasComponent<CStateMachineExecutor>())
					//comprobaci�n de que estemos en el estado inicial (que es idle) para poder activar un altar. Si no se ignora la pulsaci�n de la tecla F	
					//esto es necesario porque controlamos así que intentemos activar desde un estado que no sea idle.
					if (_entity->getComponent<CStateMachineExecutor>()->getCurrentStateMachine()->getCurrentNode() 
										== _entity->getComponent<CStateMachineExecutor>()->getCurrentStateMachine()->getInitialNode())
						startSwitchingState();
				
			}
			else if(message->getAction() == Message::WALK_RIGHT ||
					message->getAction() == Message::WALK_LEFT ||
					message->getAction() == Message::JUMP)	
				stopSwitchingState();

			break;
		
		case Message::ALTAR_SWITCHED:
			CMessage *smMsg = new CMessage();
			smMsg->setType(Message::ALTAR_MS_ORDER);
			smMsg->setAction(Message::FINISH_SUCCESS);
			_entity->emitMessage(smMsg);	
			break;
		
		}
	} // process
	


	//---------------------------------------------------------

	void CAltarStateSwitcher::startSwitchingState()
	{
		
		if (_switchingAllowed && !_switchingState)
		{
			if (_entity->hasComponent<CAvatarController>())
			{
				_entity->getComponent<CAvatarController>()->stopMovement();
				_entity->getComponent<CAvatarController>()->sleep();
			}
			_switchingState = true;
			
			CMessageUInt *m = new CMessageUInt();
			m->setType(Message::CONTROL);
			m->setAction(Message::SWITCH_ALTAR);
			m->setUInt(_entity->getEntityID());
			_target->emitMessage(m);

			CMessage *m2 = new CMessage();
			m2->setType(Message::ALTAR_MS_ORDER);
			m2->setAction(Message::SWITCH_ALTAR);
			_entity->emitMessage(m2);
			
		}
	}

	//---------------------------------------------------------
	void CAltarStateSwitcher::stopSwitchingState()
	{
		
		if (_switchingState && _switchingAllowed && _entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_CENTER)
		{
			_switchingState = false;

			CMessage *m = new CMessage();
			m->setType(Message::CONTROL);
			m->setAction(Message::STOP_SWITCH);
			_target->emitMessage(m);

			_entity->getComponent<CAvatarController>()->awake();
			CMessage *smMsg = new CMessage();
			smMsg->setType(Message::ALTAR_MS_ORDER);
			smMsg->setAction(Message::STOP_SWITCH);
			_entity->emitMessage(smMsg);

		}

	}
	
	//---------------------------------------------------------

	void CAltarStateSwitcher::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);


		if (_switchingState)
		{	
			if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT || _entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
				_targetSense = Logic::LogicalPosition::LOOKING_CENTER;
		
			if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT)
			{
				if (_entity->hasComponent<CAvatarController>())
					_entity->getComponent<CAvatarController>()->sleep();
				
				if (_entity->hasComponent<CJump>())
					_entity->getComponent<CJump>()->sleep();

				float tickRotation = Math::PI * 0.005f * msecs;
				_entity->yaw(tickRotation);
				_acumRotation += tickRotation;
				if (_acumRotation >= Math::PI/2)
				{
					_entity->yaw(-(_acumRotation - Math::PI/2));
					_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
					_targetSense = Logic::LogicalPosition::UNDEFINED;
					_acumRotation = 0;
					_entity->getComponent<CAvatarController>()->awake();	
					_entity->getComponent<CJump>()->awake();
				}
			}
			else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
			{
				if (_entity->hasComponent<CAvatarController>())
					_entity->getComponent<CAvatarController>()->sleep();		
				if (_entity->hasComponent<CJump>())
					_entity->getComponent<CJump>()->sleep();

				float tickRotation = Math::PI * 0.005f * msecs; 
				_entity->yaw(-tickRotation);
				_acumRotation += tickRotation;
				if (_acumRotation >= Math::PI/2)
				{
					_entity->yaw(_acumRotation - Math::PI/2);
					_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
					_targetSense = Logic::LogicalPosition::UNDEFINED;
					_acumRotation = 0;
					_entity->getComponent<CAvatarController>()->awake();
					_entity->getComponent<CJump>()->awake();
				}
			}

		}
	} // tick
} // namespace Logic