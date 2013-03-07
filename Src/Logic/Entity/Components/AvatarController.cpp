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

#include "Attack.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"


#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"




namespace Logic 
{
	IMP_FACTORY(CAvatarController);
	
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
		_sense = _entity->getSense();
		if (!_entity->isPlayer())
			_walkingRight=true;
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate(){} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::accept(const CMessage *message)
	{
		return (message->getType() == Message::CONTROL && 
					(message->getAction() == Message::WALK_LEFT ||
						message->getAction() == Message::WALK_RIGHT ||
							message->getAction() == Message::WALK_STOP));
	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTROL:
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
		}
	} // process
	
	//---------------------------------------------------------


	void CAvatarController::walkRight()
	{
		_walkingRight = true;
	
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setAction(Message::WALK_RIGHT);		
		message->setString("RunKatana");
		message->setBool(true);
		_entity->emitMessage(message,this);

	}

	//---------------------------------------------------------

	void CAvatarController::walkLeft()
	{
			
		_walkingLeft = true;
	//		_entity->getComponent<CAttack>()->resetAttackFlags();
	

		// Cambiamos la animación	
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);		
		message->setAction(Message::WALK_LEFT);
		message->setString("RunKatana");
		message->setBool(true);
		_entity->emitMessage(message,this);

	}

	//---------------------------------------------------------

	void CAvatarController::stopMovement() 
	{
		_walkingLeft = _walkingRight = false;
	///	_entity->getComponent<CAttack>()->resetAttackFlags();
	
		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);		
		message->setAction(Message::WALK_STOP);
		message->setString("IdleKatana");
		message->setBool(true);
		_entity->emitMessage(message,this);
		
	} // stopWalk
	
	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//si estamos andado hacia la derecha y no está girando
		if(_walkingRight && _sense != Logic::Sense::ROTATING_RIGHT && _sense != Logic::Sense::ROTATING_LEFT)
		{		
			if (_sense == Logic::Sense::RIGHT)
			{
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::WALK_RIGHT);
				m->setFloat(-_angularSpeed*msecs);
				_entity->emitMessage(m);
			}
			//rotar hacia izquierda
			else
			{
				_sense = Logic::Sense::ROTATING_RIGHT;
			}
		}
		//si estamos andado hacia la hacia la izquierda y no está rotando
		else if (_walkingLeft && _sense != Logic::Sense::ROTATING_RIGHT && _sense != Logic::Sense::ROTATING_LEFT)
		{
			if (_sense == Logic::Sense::LEFT)
			{
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::WALK_LEFT);
				m->setFloat(_angularSpeed*msecs);
				_entity->emitMessage(m);
			}
			//rotar hacia derecha
			else
			{
				_sense = Logic::Sense::ROTATING_LEFT;
			}
		}
		//rotacion a derechas
		if (_sense == Logic::Sense::ROTATING_RIGHT)
		{
			float tickRotation = Math::PI * 0.005 * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
			_entity->yaw(-tickRotation);
			_acumRotation += tickRotation;
			if (_acumRotation >= Math::PI)
			{
				_entity->yaw(_acumRotation - Math::PI);
				_sense = Logic::Sense::RIGHT;
				_acumRotation = 0;
			}
		}
		//rotacion a izquierdas
		else if (_sense == Logic::Sense::ROTATING_LEFT)
		{
			float tickRotation = Math::PI * 0.005 * msecs;
			_entity->yaw(tickRotation);
			_acumRotation += tickRotation;
			if (_acumRotation >= Math::PI)
			{
				_entity->yaw(-(_acumRotation - Math::PI));
				_sense = Logic::Sense::LEFT;
				_acumRotation = 0;
			}
		}

		_entity->setSense(_sense);
		//Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(), _entity->getHeight(), _entity->getBase(), _entity->getRing());
		//_entity->setPosition(newPosition);

			

	} // tick
} // namespace Logic