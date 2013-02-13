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

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageBool.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"


namespace Logic 
{
	IMP_FACTORY(CAvatarController);
	
	//---------------------------------------------------------

	bool CAvatarController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CAvatarController::activate()
	{
		_sentidoDerecha=_entity->getSense();
		
		if (_entity->isPlayer())
		_entity->setJumping(false); // Pablo
		if (_sentidoDerecha)
		{
			_walkingLeft=false;
			_walkingRight=true;
		}
		else
		{
			_walkingLeft=true;
			_walkingRight=false;
		}
		if (_entity->isPlayer())
			stopMovement();
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate(){} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::accept(const CMessage *message)
	{
		return message->getType() == Message::CONTROL;//notese que quito NPC_CONTROL
	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(CMessage *message)
	{switch(message->getType())
		{
		case Message::CONTROL:
			{
			if(message->getAction() == Message::GO_UP)
				goUp();
			else if(message->getAction() == Message::GO_DOWN)
				goDown();
			else if(message->getAction() == Message::WALK_LEFT)
				walkLeft();
			else if(message->getAction() == Message::WALK_RIGHT)
				walkRight();
			else if(message->getAction() == Message::WALK_STOP)
				stopMovement();
			else if(message->getAction() == Message::JUMP) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
				jump();
			else if(message->getAction() == Message::WALK_BACK)
				 {	
					CMessageBoolFloat* maux = static_cast<CMessageBoolFloat*>(message);
					_sentidoColision=maux->getBool();
					_correccionGrados=maux->getFloat();
					walkBack();
				 }
			else if(message->getAction() == Message::CHANGE_DIRECTION)
				 {	
					CMessageBool* maux = static_cast<CMessageBool*>(message);
					 _sentidoColision=maux->getBool();
					changeDirection(_sentidoColision);
				 }
				else if(message->getAction() == Message::CHANGE_BASE)
				 {	
					CMessageFloat* maux = static_cast<CMessageFloat*>(message);
					changeBase(maux->getFloat());
				 }
			else if(message->getAction() == Message::TURN)
				{
					CMessageFloat* maux = static_cast<CMessageFloat*>(message);
					turn(maux->getFloat());
				}
			}
	}

	} // process
	
	//---------------------------------------------------------

		// Pablo
	void CAvatarController::jump()
	{
		Logic::CMessage *m = new Logic::CMessage();
		m->setType(Logic::Message::AVATAR_WALK);
		m->setAction(Logic::Message::JUMP);  // Pablo
		_entity->emitMessage(m);
		//_initialJump = true;
	}
	
	void CAvatarController::turn(float amount) 
	{
		_entity->yaw(amount);

	} // turn
	
	//---------------------------------------------------------


		void CAvatarController::walkRight()
		{
			_walkingRight = true;
			// Cambiamos la animación
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::AVATAR_WALK);
			m->setAction(Logic::Message::WALK_RIGHT);
			_entity->emitMessage(m);

			_entity->setSense(LogicalPosition::DERECHA);
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("RunKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);

		}
		
		void CAvatarController::walkLeft()
		{
			_walkingLeft = true;
			

			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::AVATAR_WALK);
			m->setAction(Logic::Message::WALK_LEFT);
			_entity->emitMessage(m);


			// Cambiamos la animación
			_entity->setSense(LogicalPosition::IZQUIERDA);
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("RunKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);

		}

	//---------------------------------------------------------

		void CAvatarController::walkBack()
		{			
			_walkBack=true; //para retroceder en las colisiones   
			/*
					CMessageBoolFloat* maux = static_cast<CMessageBoolFloat*>(message);
					_sentidoColision=maux->getBool();
					_correccionGrados=maux->getFloat();
			*/
			Logic::CMessageBoolFloat *m = new Logic::CMessageBoolFloat();
			m->setType(Logic::Message::AVATAR_WALK);
			m->setAction(Logic::Message::WALK_BACK);
			m->setBool(_sentidoColision);
			m->setFloat(_correccionGrados);
			_entity->emitMessage(m);


			// Cambiamos la animación
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("RunKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);
		}
	
	//---------------------------------------------------------

	void CAvatarController::stopMovement() 
	{
		_walkingLeft = _walkingRight = false;

		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		
		Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::AVATAR_WALK);
		m->setAction(Logic::Message::WALK_STOP);
		_entity->emitMessage(m,this);


			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("IdleKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);

	} // stopWalk
	
	void CAvatarController::changeDirection(const bool newDirection)
	{		
				CMessageString *m = new CMessageString();
				m->setType(Message::SET_SHADER);
				m->setString("marine");
				_entity->emitMessage(m,this);
				if (_entity->getType().compare("Player")==0)
					return;
	
			//	if (_entity->getType().compare("AnimatedEntity")==0)
				//{
				//if (!_walkingLeft && !_walkingRight)
			//	{
					if (newDirection)
						walkLeft();
					else
						walkRight();
//}
					//	}
			}
	
	void CAvatarController::changeBase(int base)
	{
		if(_entity->getJumping()==false && !_walkingLeft && !_walkingRight)
			{
		Logic::CMessageFloat *m = new Logic::CMessageFloat();
			m->setType(Logic::Message::CHANGE_RING);
			m->setFloat(base);
			m->setAction(Logic::Message::CHANGE_BASE);
		_entity->emitMessage(m,this);
		}
	}

	
	void CAvatarController::goUp()
	{
		Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CHANGE_RING);
		m->setAction(Logic::Message::GO_UP);
		_entity->emitMessage(m,this);

	}

	
	void CAvatarController::goDown()
	{
		
				Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CHANGE_RING);
		m->setAction(Logic::Message::GO_DOWN);
		_entity->emitMessage(m,this);

	}

	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		// Si nos estamos desplazando calculamos la próxima posición
		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.
	/*	if(_walking || _walkingBack || _strafingLeft || _strafingRight)
		{
			Vector3 direction(Vector3::ZERO);
			Vector3 directionStrafe(Vector3::ZERO);

			if(_walking || _walkingBack)
			{
				direction = Math::getDirection(_entity->getYaw());
				//_entity->setYaw(Math::fro);
				//_entity->setOrientation(0-_entity->getPosition().x,(-125)-_entity->getPosition().y,0-_entity->getPosition().z);
				if(_walkingBack)
					direction *= -1;
			}

			if(_strafingLeft || _strafingRight)
			{
				directionStrafe = 
						Math::getDirection(_entity->getYaw() + Math::PI/2);
				if(_strafingRight)
					directionStrafe *= -1;
			}

			direction += directionStrafe;
			direction.normalise();
			direction *= msecs * _speed;

			// Enviar un mensaje para que el componente físico mueva el personaje
			TMessage message;
			message._type = Message::AVATAR_WALK;
			message._vector3 = direction;
			_entity->emitMessage(message);

			//Vector3 newPosition = _entity->getPosition() + direction;
			//_entity->setPosition(newPosition);
		}
		*/
	} // tick

} // namespace Logic

