/**
@file Destruction.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CDestruction
@see Logic::IComponent

@author 
@date
*/

#include "Destruction.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Application/BaseApplication.h"


namespace Logic 
{
	IMP_FACTORY(CDestruction);
	
	//---------------------------------------------------------

	bool CDestruction::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_reloj=Application::CBaseApplication::getSingletonPtr()->getClock();
	
		return true;
	} // spaw
	
	//---------------------------------------------------------

	bool CDestruction::activate()
	{
		_destroyingSpeed=0.13f;
		_reloj->addTimeObserver(_entity->getEntityID(),this,2500);		
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CDestruction::deactivate()
	{
	} // deactivate
	
	//---------------------------------------------------------

	bool CDestruction::accept(const CMessage *message)
	{
		return (isAwake() && message->getType() == Message::CONTROL && 
					message->getAction() == Message::DESTROY);
	}
	
	//---------------------------------------------------------

	void CDestruction::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::DESTROY)
					destroy();
		}
	} // process
	
	//---------------------------------------------------------

	void CDestruction::destroy() 
	{
		if (!_destroying)
		{
			_destroying = true;
		}
	} 
	
	//---------------------------------------------------------

	void CDestruction::timeArrived()
	{
		_destroying=true;
	}
		
	void CDestruction::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	
		if (_destroying)
		{
			if (_entity->getComponent<CPhysics>()!=NULL)
				_entity->getComponent<CPhysics>()->deactivate();
			
			if (msecs>100) msecs=80;

			_destroyingSpeed-= 0.003f * msecs;   //gravedad 0.0003f
			float tickHeight = _destroyingSpeed * msecs/100;
			
			/*Logic::CMessageFloat *m = new Logic::CMessageFloat();
			m->setType(Logic::Message::AVATAR_MOVE);
			m->setAction(Logic::Message::JUMP);
			m->setFloat(tickHeight);
			_entity->emitMessage(m);*/
			
		
			//_entity->setLogicalPosition(_entity->getLogicalPosition());
			
			Vector3 position=_entity->fromLogicalToCartesian(_entity->getLogicalPosition());
			position.y+=tickHeight;	
			_entity->setPosition(position);
			_entity->getLogicalPosition()->setHeight(position.y);
			
		}		
	 // tick
	}
} // namespace Logic