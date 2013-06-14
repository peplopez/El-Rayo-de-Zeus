/**
@file AvatarController.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "AvatarController.h"

#include "Combat.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"


namespace Logic 
{
	IMP_FACTORY(CAvatarController);//*=0.92 y defrag
	
	//---------------------------------------------------------

	bool CAvatarController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;				
		
		if(entityInfo->hasAttribute("angularSpeed"))
			_angularSpeed=entityInfo->getFloatAttribute("angularSpeed");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CAvatarController::activate()
	{

		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate()
	{
	} // deactivate

	//---------------------------------------------------------

	void CAvatarController::awake()
	{
		IComponent::awake();

	} // awake

	//---------------------------------------------------------

	void CAvatarController::sleep()
	{
		IComponent::sleep();
	} // sleep

	//---------------------------------------------------------

	bool CAvatarController::accept(const CMessage *message)
	{
		return (message->getType() == Message::CONTROL	&& 
			(message->getAction() == Message::WALK_LEFT ||
			message->getAction() == Message::WALK_RIGHT ||
			message->getAction() == Message::WALK_STOP));
	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(CMessage *message)
	{
		if(message->getAction() == Message::WALK_LEFT)
		{
				walkLeft();
		}
		else if(message->getAction() == Message::WALK_RIGHT)
		{	
				walkRight();
		}
		else if(message->getAction() == Message::WALK_STOP)
		{
				stopMovement();
		}
	} // process
	
	//---------------------------------------------------------


	void CAvatarController::walkRight()
	{
		_walkingLeft = false;
		_walkingRight = true;
		
	}

	//---------------------------------------------------------

	void CAvatarController::walkLeft()
	{
		_walkingRight = false;	
		_walkingLeft = true;

	}

	//---------------------------------------------------------

	void CAvatarController::stopMovement() 
	{
		_walkingLeft = _walkingRight = false;
		Logic::CMessageFloat *m = new Logic::CMessageFloat();
		m->setType(Logic::Message::AVATAR_MOVE);
		m->setAction(Logic::Message::WALK_STOP);

	}
	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		
		//si estamos andado hacia la derecha y no está girando
		if(_walkingRight && _targetSense == Logic::LogicalPosition::UNDEFINED)
		{		
			if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT)
			{
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::WALK_RIGHT);
				m->setFloat(-_angularSpeed);
				_entity->emitMessage(m);
			}
			//rotar hacia derecha
			else
			{
				_targetSense = Logic::LogicalPosition::RIGHT;
			}
		}
		//si estamos andado hacia la hacia la izquierda y no está rotando
		else if (_walkingLeft && _targetSense == Logic::LogicalPosition::UNDEFINED)
		{
			if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
			{
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::WALK_LEFT);
				m->setFloat(_angularSpeed);
				_entity->emitMessage(m);
			}
			//rotar hacia izquierda
			else
			{
				_targetSense = Logic::LogicalPosition::LEFT;
			}
		}
		//rotacion a derechas
		else if (_targetSense == Logic::LogicalPosition::RIGHT)
		{
			float totalYaw = Math::PI;
			if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_CENTER)
				totalYaw = Math::PI * 0.5f;
			float tickRotation = totalYaw * 0.005f * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
			_entity->yaw(-tickRotation);
			_acumRotation += tickRotation;
			if (_acumRotation >= totalYaw)
			{
				_entity->yaw(_acumRotation - totalYaw);
				_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::RIGHT);
				_targetSense = Logic::LogicalPosition::UNDEFINED;
				_acumRotation = 0;
			}
		}
		//rotacion a izquierdas
		else if (_targetSense == Logic::LogicalPosition::LEFT)
		{
			float totalYaw = Math::PI;
			if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_CENTER)
				totalYaw = Math::PI * 0.5f;
			float tickRotation = totalYaw * 0.005f * msecs;
			_entity->yaw(tickRotation);
			_acumRotation += tickRotation;
			if (_acumRotation >= totalYaw)
			{
				_entity->yaw(-(_acumRotation - totalYaw));
				_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LEFT);
				_targetSense = Logic::LogicalPosition::UNDEFINED;
				_acumRotation = 0;
			}
		}	

	} // tick
} // namespace Logic