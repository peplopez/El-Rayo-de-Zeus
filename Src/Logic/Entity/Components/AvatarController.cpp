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
#include "Logic/Entity/Messages/MessageInt.h"
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
		
		if(entityInfo->hasAttribute("angularSpeed"))
			_angularSpeed=entityInfo->getFloatAttribute("angularSpeed");

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
		if (_entity->isPlayer())
		return message->getType() == Message::CONTROL ||
			   message->getType() == Message::ANIMATION_FINISHED;
			//notese que quito NPC_CONTROL
	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTROL:
			{
			if(message->getAction() == Message::GO_UP)
				goUp();
			else if(message->getAction() == Message::GO_DOWN)
				goDown();
			else if(message->getAction() == Message::WALK_LEFT)
			{
				if (!_lightAttack&&!_heavyAttack)
					walkLeft();
			}
			else if(message->getAction() == Message::WALK_RIGHT)
			{	
				if (!_lightAttack&&!_heavyAttack)
					walkRight();
			}
			else if(message->getAction() == Message::WALK_STOP)
			{
			//	if (!_lightAttack&&!_heavyAttack)
					stopMovement();
			}
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
					CMessageInt* maux = static_cast<CMessageInt*>(message);
					changeBase(maux->getInt());
				 }
			else if(message->getAction() == Message::TURN)
				{
					CMessageFloat* maux = static_cast<CMessageFloat*>(message);
					turn(maux->getFloat());
				}
			else if(message->getAction() == Message::LIGHT_ATTACK)
			{
				if (!_lightAttack&&!_heavyAttack)
					lightAttack();
			}
			else if(message->getAction() == Message::HEAVY_ATTACK)
			{
				if (!_lightAttack&&!_heavyAttack)
				heavyAttack();
			}
			break;
		}
			case Message::ANIMATION_FINISHED:
			{
					CMessageString* maux = static_cast<CMessageString*>(message);
					if (maux->getString().compare("FireKatana")==0)
						_lightAttack=_heavyAttack=false;stopMovement();
			}
	}

	} // process
	
	//---------------------------------------------------------

		// Pablo
	void CAvatarController::jump()
	{
		Logic::CMessage *m = new Logic::CMessage();
		m->setType(Logic::Message::CHANGE_HEIGHT);
		m->setAction(Logic::Message::JUMP);  // Pablo
		_entity->emitMessage(m);
		//_initialJump = true;
	}
	
	void CAvatarController::turn(float amount) 
	{
		_entity->yaw(amount);

	} // turn
	

	void CAvatarController::lightAttack() 
	{
		_lightAttack=true;
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setString("FireKatana");
		message->setBool(false);
		_entity->emitMessage(message,this);

	} // turn

	void CAvatarController::heavyAttack() 
	{		
		_heavyAttack=true;
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setString("FireKatana");
		message->setBool(false);
		_entity->emitMessage(message,this);

	} // turn


	//---------------------------------------------------------


		void CAvatarController::walkRight()
		{
			_walkingRight = true;
			// Cambiamos la animación
			/*Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::AVATAR_MOVE);
			m->setAction(Logic::Message::WALK_RIGHT);
			_entity->emitMessage(m);
			*/
			_entity->setSense(LogicalPosition::DERECHA);
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("RunKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);

		}
		
		void CAvatarController::walkLeft()
		{
			
			if (_lightAttack || _heavyAttack)
				return;
			_walkingLeft = true;
			

			/*Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::AVATAR_MOVE);
			m->setAction(Logic::Message::WALK_LEFT);
			_entity->emitMessage(m);*/


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
			m->setType(Logic::Message::AVATAR_MOVE);
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
		_walkingLeft = _walkingRight = _lightAttack=_heavyAttack= false;

		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		
		Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::AVATAR_MOVE);
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
			m->setType(Logic::Message::CHANGE_PLANE);
			m->setFloat(base);
			m->setAction(Logic::Message::CHANGE_BASE);
		_entity->emitMessage(m,this);
		}
	}

	
	void CAvatarController::goUp()
	{
		Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CHANGE_PLANE);
		m->setAction(Logic::Message::GO_UP);
		_entity->emitMessage(m,this);

	}

	
	void CAvatarController::goDown()
	{
		
		Logic::CMessage *m = new Logic::CMessage();
		m->setType(Logic::Message::CHANGE_PLANE);
		m->setAction(Logic::Message::GO_DOWN);
		_entity->emitMessage(m,this);

	}

	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
			
			Vector3 direction(Vector3::ZERO);

			if(_walkingLeft || _walkingRight || /*_initialJump*/ _entity->getJumping())
			{
				if(_walkingLeft || _walkingRight)
				{
					direction = Math::getDirection(_entity->getYaw() + Math::PI/2);
					//Matrix4 orientacion = _entity->getOrientation();
					//Math::yaw(Math::fromDegreesToRadians(_actualDegree),orientacion);
					if(_walkingRight){
						if(_sentidoDerecha==true)
						{
							_sentidoDerecha=false;
							_entity->yaw(Math::PI);
						}
						if (!_walkBack)
						{
							//_entity->setDegree(_entity->getDegree()-_angularSpeed*msecs); 
							Logic::CMessageFloat *m = new Logic::CMessageFloat();
							m->setType(Logic::Message::AVATAR_MOVE);
							m->setAction(Logic::Message::WALK_RIGHT);
							m->setFloat(_entity->getDegree()-_angularSpeed*msecs);
							_entity->emitMessage(m);
							_entity->yaw(Math::fromDegreesToRadians(_angularSpeed*msecs));
						}
					}
					else
					{
						if(_sentidoDerecha==false)
						{
							_entity->yaw(Math::PI);					
							_sentidoDerecha=true;
						}
						if (!_walkBack)
						{
							Logic::CMessageFloat *m = new Logic::CMessageFloat();
							m->setType(Logic::Message::AVATAR_MOVE);
							m->setAction(Logic::Message::WALK_LEFT);
							m->setFloat(_entity->getDegree()+_angularSpeed*msecs);
							_entity->emitMessage(m);

							//_entity->setDegree(_entity->getDegree()+_angularSpeed*msecs);
							_entity->yaw(Math::fromDegreesToRadians(-_angularSpeed*msecs));
						}			
					
					}
				
					if(_walkingLeft)
						direction *= -1;
				}

				direction.normalise();

			//	Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getHeight(),_entity->getBase(),_entity->getRing());
			
				//_entity->setPosition(newPosition);
				
			}

			if (_walkBack)
			{
				//stopMovement();   
				_walkBack=false;

				if (_sentidoColision) // la direccion
				{
					_entity->setDegree(_entity->getDegree()+((_angularSpeed+(_correccionGrados/msecs))*msecs)); 
					_entity->yaw(Math::fromDegreesToRadians(-(_angularSpeed*msecs)));
				}
				else
				{
					_entity->setDegree(_entity->getDegree()-((_angularSpeed+(_correccionGrados/msecs))*msecs)); 
					_entity->yaw(Math::fromDegreesToRadians((_angularSpeed)));
				}							
				
				_sentidoColision=false;
				Vector3 newPositionWalkBack=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getHeight(),_entity->getBase(),_entity->getRing());		
				
				_entity->setPosition(newPositionWalkBack);

			}		
	} // tick
} // namespace Logic