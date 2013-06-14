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
					_switchingAllowed = false;
					_target = 0;
				}
			}
			break;
		
		case Message::CONTROL:

			if(message->getAction() == Message::SWITCH_ALTAR)
				startSwitchingState();
			else if(message->getAction() == Message::WALK_RIGHT)	
				stopSwitchingState(Logic::LogicalPosition::RIGHT);
			else if(message->getAction() == Message::WALK_LEFT)	
				stopSwitchingState(Logic::LogicalPosition::LEFT);
			else
				stopSwitchingState(Logic::LogicalPosition::RIGHT);
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
			if (_entity->getComponent<CAvatarController>()!=NULL)
				_entity->getComponent<CAvatarController>()->sleep();
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
				float tickRotation = Math::PI * 0.005f * msecs; 
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
					float tickRotation = Math::PI * 0.005f * msecs; 
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
						CMessage *smMsg = new CMessage();
						smMsg->setType(Message::ALTAR_MS_ORDER);
						smMsg->setAction(Message::STOP_SWITCH);
						_entity->emitMessage(smMsg);

					
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
						CMessage *smMsg = new CMessage();
						smMsg->setType(Message::ALTAR_MS_ORDER);
						smMsg->setAction(Message::STOP_SWITCH);
						_entity->emitMessage(smMsg);
					
						 
					}
				}
			}
		}
	} // tick
} // namespace Logic