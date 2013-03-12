/**
@file Altar.cpp

Contiene la implementaci�n del componente que controla la 
capacidad de un Character de activar/desactivar altares
 
@see Logic::CAltar
@see Logic::IComponent

@author Emilio Santalla
@date Emilio Santalla, 2013
*/

#include "Altar.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"


#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageUIntString.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::ALTAR>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace Logic 
{
	IMP_FACTORY(CAltar);
	
	//---------------------------------------------------------

	bool CAltar::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;				

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CAltar::activate()
	{
		_acumTime = _switchingTime;
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAltar::deactivate()
	{	
	} // deactivate
	
	//---------------------------------------------------------

	bool CAltar::accept(const CMessage *message)
	{
		return (message->getType() == Message::CONTROL);

	} // accept
	
	//---------------------------------------------------------

	void CAltar::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::SWITCH_ALTAR)
			{
				startSwitchingState();
			}
			else if(message->getAction() == Message::STOP_SWITCH)
			{	
				stopSwitchingState();
			}
		}
	} // process
	
	//---------------------------------------------------------

	void CAltar::startSwitchingState()
	{
		LOG(_entity->getName() << ": cambiando de estado")	
		_switchingState = true;
		_revertingState = false;


	}

	//---------------------------------------------------------
	void CAltar::stopSwitchingState()
	{
		
		if (_switchingState)
		{
			LOG(_entity->getName() << ": volviendo al estado original")
			_switchingState = false;
			_revertingState = true;

		}
	}
	
	//---------------------------------------------------------

	void CAltar::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (_switchingState)
		{
			_acumTime -= msecs;
			if (_acumTime <= 0 )
			{
				_switchingState = false;
				_on = !_on;
				if (_on)
				{
					LOG(_entity->getName() << ": activado")
					CMessageUIntString *m = new CMessageUIntString();	
					m->setType(Message::SET_SUBENTITY_MATERIAL);
					m->setString("altaractivado");
					m->setUInt(0);
					_entity->emitMessage(m,this);
				}

				else 
				{
					LOG(_entity->getName() << ": desactivado")
					CMessageUIntString *m = new CMessageUIntString();	
					m->setType(Message::SET_SUBENTITY_MATERIAL);
					m->setString("Material.001");
					m->setUInt(0);
					_entity->emitMessage(m,this);

				}
				_acumTime = _switchingTime;
			}
		}
		if (_revertingState)
		{
			_acumTime += msecs;
			if (_acumTime >= _switchingTime)
			{
				if (_on)
					LOG(_entity->getName() << ": activado")
				else 
					LOG(_entity->getName() << ": desactivado")

				_revertingState = false;
				_acumTime = _switchingTime;
			}
		}
	
	} // tick
} // namespace Logic