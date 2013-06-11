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

/*para tener un acceso directo al gamestatus*/
#include "Logic/GameStatus.h"
#include "Logic/RingInfo.h"
#include "Logic/BaseInfo.h"
#include "Logic/AltarInfo.h"
#include "Logic/PlayerInfo.h"


#define DEBUG 1
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
	
		//_gameStatus=Application::CBaseApplication::getSingletonPtr()->getGameState()->getGameStatus();
		_gameStatus=CGameStatus::getSingletonPtr();
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
					message->getType() == Message::CONTROL
/*Pep, mensaje mio*/||message->getType() == Message::ALTAR_ACTIVATED );

	} // accept
	
	//---------------------------------------------------------

	void CAltarStateSwitcher::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::TRIGGER:
			if(message->getAction() == Message::TRIGGER_ENTER &&
				(_target = _entity->getMap()->getEntityByID(static_cast<CMessageUInt*>(message)->getUInt())) != NULL)
			{
				if(_target->getType() == "Altar")
				{
					_switchingAllowed = true;
				}
				else
				{
					_target = NULL;
				}
			}
			else if(message->getAction() == Message::TRIGGER_EXIT &&
				_target == _entity->getMap()->getEntityByID(static_cast<CMessageUInt*>(message)->getUInt()))
				{	
					_switchingAllowed = false;
					_target = NULL;
				}
		case Message::CONTROL:
			if(message->getAction() == Message::SWITCH_ALTAR)
				startSwitchingState();
			else if(message->getAction() == Message::WALK_RIGHT)
			{	
				stopSwitchingState(Logic::LogicalPosition::RIGHT);
			}
			else if(message->getAction() == Message::WALK_LEFT)
			{	
				stopSwitchingState(Logic::LogicalPosition::LEFT);
			}
			else
			{
				stopSwitchingState(Logic::LogicalPosition::RIGHT);
			}
			break;
			case Message::ALTAR_ACTIVATED:
			{
				_gameStatus->getPlayer(_entity->getOriginBase())->increaseAltarsActivated();
				/*CMessageBoolUShort *message = new CMessageBoolUShort();
				message->setType(Message::SET_ANIMATION);		
				message->setUShort(Logic::IDLE);
				message->setBool(true);
				_entity->emitMessage(message,this);*/
			}
		}
	} // process
	


	//---------------------------------------------------------

	void CAltarStateSwitcher::startSwitchingState()
	{
		
		if (_switchingAllowed && !_switchingState)
		{
			if (_entity->getComponent<CAvatarController>()!=NULL)
				_entity->getComponent<CAvatarController>()->sleep();
			_switchingState = true;
			
	/*		CMessageUInt *m = new CMessageUInt();
			m->setType(Message::CONTROL);
			m->setAction(Message::SWITCH_ALTAR);
			m->setUInt(_entity->getEntityID());
			_target->emitMessage(m);
				*/
			CMessageUInt *m = new CMessageUInt();
			m->setType(Message::CONTROL);
			m->setAction(Message::SWITCH_ALTAR);
			m->setUInt(_entity->getEntityID());
			_target->emitMessage(m);
			
			/*CMessageUInt *m = new CMessageUInt();
			m->setType(Message::CONTROL);
			m->setAction(Message::SWITCH_ALTAR);
			m->setUInt(_entity->getEntityID());
			_entity->emitMessage(m);*/

			/*CMessageBoolUShort *message = new CMessageBoolUShort();
			message->setType(Message::SET_ANIMATION);		
			message->setUShort(Logic::ACTIVATE_ALTAR);
			message->setBool(true);
			_entity->emitMessage(message,this);*/
		}
	}

	//---------------------------------------------------------
	void CAltarStateSwitcher::stopSwitchingState(Logic::Sense targetSense)
	{
		
		if (_switchingState && _targetSense == Logic::LogicalPosition::UNDEFINED &&  _switchingAllowed)
		{
			_targetSense = targetSense;
			
			CMessage *m = new CMessage();
			m->setType(Message::CONTROL);
			m->setAction(Message::STOP_SWITCH);
			_target->emitMessage(m);

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
				float tickRotation = Math::PI * 0.005f * msecs;
				_entity->yaw(tickRotation);
				_acumRotation += tickRotation;
				if (_acumRotation >= Math::PI/2)
				{
					_entity->yaw(-(_acumRotation - Math::PI/2));
					_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
					_targetSense = Logic::LogicalPosition::UNDEFINED;
					_acumRotation = 0;
				}
			}
			else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
			{
				float tickRotation = Math::PI * 0.005f * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
				_entity->yaw(-tickRotation);
				_acumRotation += tickRotation;
				if (_acumRotation >= Math::PI/2)
				{
					_entity->yaw(_acumRotation - Math::PI/2);
					_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
					_targetSense = Logic::LogicalPosition::UNDEFINED;
					_acumRotation = 0;
				}
			}

			else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LOOKING_CENTER)
			{
				if (_targetSense == Logic::LogicalPosition::RIGHT)
				{
					float tickRotation = Math::PI * 0.005f * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(_acumRotation - Math::PI/2);
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::RIGHT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_switchingState = false;
						_acumRotation = 0;
						_entity->getComponent<CAvatarController>()->awake();
					
					}
				}
				else if (_targetSense == Logic::LogicalPosition::LEFT)
				{
					float tickRotation = Math::PI * 0.005f * msecs;
					_entity->yaw(tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LEFT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_switchingState = false;
						_acumRotation = 0;
						_entity->getComponent<CAvatarController>()->awake();
						 
					}
				}
			}
		}
	} // tick
} // namespace Logic