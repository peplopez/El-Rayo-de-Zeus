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
#include "Logic/Entity/Messages/MessageBoolString.h"



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
					message->getType() == Message::CONTROL);

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
				stopSwitchingState(Logic::Sense::RIGHT);
			}
			else if(message->getAction() == Message::WALK_LEFT)
			{	
				stopSwitchingState(Logic::Sense::LEFT);
			}
			else
			{
				stopSwitchingState(Logic::Sense::RIGHT);
			}

		}
	} // process
	


	//---------------------------------------------------------

	void CAltarStateSwitcher::startSwitchingState()
	{
		
		if (_switchingAllowed && !_switchingState)
		{

			_entity->getComponent<CAvatarController>()->sleep();
			_switchingState = true;
			
			CMessage *m = new CMessage();
			m->setType(Message::CONTROL);
			m->setAction(Message::SWITCH_ALTAR);
			_target->emitMessage(m);
				
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);		
			message->setString("Idle");
			message->setBool(true);
			_entity->emitMessage(message,this);
		}
	}

	//---------------------------------------------------------
	void CAltarStateSwitcher::stopSwitchingState(Logic::Sense targetSense)
	{
		
		if (_switchingState && _targetSense == Logic::Sense::UNDEFINED &&_switchingAllowed)
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
			if (_entity->getSense() == Logic::Sense::RIGHT)
				_entity->setSense(Logic::Sense::ROTATING_LEFT);
			else if (_entity->getSense() == Logic::Sense::LEFT)
				_entity->setSense(Logic::Sense::ROTATING_RIGHT);
		
			if (_entity->getSense() == Logic::Sense::ROTATING_LEFT)
			{
				float tickRotation = Math::PI * 0.005 * msecs;
				_entity->yaw(tickRotation);
				_acumRotation += tickRotation;
				if (_acumRotation >= Math::PI/2)
				{
					_entity->yaw(-(_acumRotation - Math::PI/2));
					_entity->setSense(Logic::Sense::LOOKING_CENTER);
					_acumRotation = 0;
				}
			}
			else if (_entity->getSense() == Logic::Sense::ROTATING_RIGHT)
			{
				float tickRotation = Math::PI * 0.005 * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
				_entity->yaw(-tickRotation);
				_acumRotation += tickRotation;
				if (_acumRotation >= Math::PI/2)
				{
					_entity->yaw(_acumRotation - Math::PI/2);
					_entity->setSense(Logic::Sense::LOOKING_CENTER);
					_acumRotation = 0;
				}
			}

			else if (_entity->getSense() == Logic::Sense::LOOKING_CENTER)
			{
				if (_targetSense == Logic::Sense::RIGHT)
				{
					float tickRotation = Math::PI * 0.005 * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(_acumRotation - Math::PI/2);
						_entity->setSense(Logic::Sense::RIGHT);
						_targetSense = Logic::Sense::UNDEFINED;
						_switchingState = false;
						_acumRotation = 0;
						_entity->getComponent<CAvatarController>()->awake();
					}
				}
				else if (_targetSense == Logic::Sense::LEFT)
				{
					float tickRotation = Math::PI * 0.005 * msecs;
					_entity->yaw(tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->setSense(Logic::Sense::LEFT);
						_targetSense = Logic::Sense::UNDEFINED;
						_switchingState = false;
						_acumRotation = 0;
						_entity->getComponent<CAvatarController>()->awake();
					}
				}
			}
		}
	} // tick
} // namespace Logic