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
#include "Map/Entity.h"
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
			_angularSpeed = entityInfo->getFloatAttribute("angularSpeed");
		//if(entityInfo->hasAttribute("wanderAngularSpeed"))
			//_angularSpeed = entityInfo->getFloatAttribute("wanderAngularSpeed");

		if(entityInfo->hasAttribute("turnSpeedFactor"))
			_turnSpeedFactor = entityInfo->getFloatAttribute("turnSpeedFactor");

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
			walkLeft();
		else if(message->getAction() == Message::WALK_RIGHT)
			walkRight();
		else if(message->getAction() == Message::WALK_STOP)
			stopMovement();
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

	void CAvatarController::emitAngularSpeed(Logic::Sense sense)
	{
		Logic::CMessageFloat *m = new Logic::CMessageFloat();
		m->setType(Logic::Message::AVATAR_MOVE);

		 //si la maquina de estados dice que wander es true (es decir estamos andando y no corriendo) reducimos la velocidad aquí.
		float tempAngularSpeed=_wander?_angularSpeed/2:_angularSpeed;

		switch (sense)
		{
		case Logic::Sense::RIGHT: 
			m->setAction(Logic::Message::WALK_RIGHT);			
			m->setFloat(-tempAngularSpeed);
			break;
		case Logic::Sense::LEFT: 
			m->setAction(Logic::Message::WALK_LEFT);
			m->setFloat(tempAngularSpeed);
			break;
		}

		_entity->emitMessage(m);

	}

	//---------------------------------------------------------

	void CAvatarController::estimateRotation(Logic::Sense sense)
	{
		_targetSense = sense;
		if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_CENTER)
			_totalYaw = Math::PI * 0.5f;
		else if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_OUTSIDE)
			_totalYaw = -Math::PI * 0.5f;
		else
			_totalYaw = Math::PI;
		_entity->getLogicalPosition()->setSense(Logic::Sense::UNDEFINED);
	}

	//---------------------------------------------------------

	void CAvatarController::rotate(Logic::Sense sense, unsigned int msecs)
	{

		float tickRotation = Math::PI * _turnSpeedFactor * msecs;
		
		switch (sense)
		{
		case Logic::Sense::RIGHT:

			_entity->yaw(-tickRotation);
			_acumRotation += tickRotation;
			if (_acumRotation >= _totalYaw)
			{
				_entity->yaw(_acumRotation - _totalYaw);
				_entity->getLogicalPosition()->setSense(Logic::Sense::RIGHT);
				_targetSense = Logic::LogicalPosition::UNDEFINED;
				_acumRotation = 0.0f;
			}
			break;

		case Logic::Sense::LEFT:

			_entity->yaw(tickRotation);
			_acumRotation += tickRotation;
			if (_acumRotation >= _totalYaw)
			{
				_entity->yaw(-(_acumRotation - _totalYaw));
				_entity->getLogicalPosition()->setSense(Logic::Sense::LEFT);
				_targetSense = Logic::LogicalPosition::UNDEFINED;
				_acumRotation = 0.0f;
			}
			break;
		}

	}

	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		
		//si está andado hacia la derecha y no está rotando sobre si mismo
		if(_walkingRight && _targetSense == Logic::Sense::UNDEFINED)
		{		
			if (_entity->getLogicalPosition()->getSense() == Logic::Sense::RIGHT)
				emitAngularSpeed(Logic::Sense::RIGHT);	
			else //rotar hacia derecha
				estimateRotation(Logic::Sense::RIGHT);
		}
		//si está andado hacia la derecha y no está rotando sobre si mismo
		else if (_walkingLeft && _targetSense == Logic::Sense::UNDEFINED)
		{
			if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LEFT)
				emitAngularSpeed(Logic::Sense::LEFT);	
			else //rotar hacia izquierda
				estimateRotation(Logic::Sense::LEFT);
		}
		//rotacion a derechas
		else if (_targetSense == Logic::Sense::RIGHT)
		{
			rotate(Logic::Sense::RIGHT, msecs);
		}
		//rotacion a izquierdas
		else if (_targetSense == Logic::Sense::LEFT)
		{
			rotate(Logic::Sense::LEFT, msecs);
		}	

	} // tick
} // namespace Logic