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


#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageUInt.h"




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

	void CAltarStateSwitcher::deactivate(){} // deactivate
	
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
			else 
				stopSwitchingState();
		}
	} // process
	
	//---------------------------------------------------------


	void CAltarStateSwitcher::allowSwitching()
	{

		_switchingAllowed = true;

	}

	//---------------------------------------------------------

	void CAltarStateSwitcher::prohibitSwitching()
	{
			
		_switchingAllowed = false;

	}

	//---------------------------------------------------------

	void CAltarStateSwitcher::startSwitchingState()
	{
		if (_switchingAllowed && _target)
		{
			
			_switchingState = true;
			CMessage *m = new CMessage();
			m->setType(Message::CONTROL);
			m->setAction(Message::SWITCH_ALTAR);
			_target->emitMessage(m);
		}
	}

	//---------------------------------------------------------
	void CAltarStateSwitcher::stopSwitchingState()
	{
		
		if (_switchingState &&_switchingAllowed && _target)
		{
			_switchingState = false;
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
			
	} // tick
} // namespace Logic