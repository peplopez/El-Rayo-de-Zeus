/**
@file AngularMovement.h

Contiene la declaración del componente que controla el movimiento 
angular de entidades.

@see Logic::CJump
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/

#include "Jump.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageBool.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"

//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CJump);

	bool CJump::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// Pablo. Inicializo la gravedad
		if(entityInfo->hasAttribute("gravity"))
			_initialGravity = entityInfo->getFloatAttribute("gravity");
		// Pablo. Inicializo la gravedad
		if(entityInfo->hasAttribute("jumpPower"))
			_jumpPower = entityInfo->getFloatAttribute("jumpPower");

		return true;
		}

	bool CJump::activate()
	{
		_jumping=false;
		if (_entity->isPlayer())
		{
			_entity->setJumping(false); // QUITAR SETJUMPING, NO SE TIENE EN CUENTA.
		}
		return true;
	}
		

	void CJump::deactivate(){}

	
	bool CJump::accept(const CMessage *message)
	{//aviso de que tanto accept como process son un poco hack, pero es es solo hasta tener un componente NPCCONTROLLER
		//return false; //de momento no recibo mensajes,luego lo kito
		
		//if (_entity->isPlayer())
		return message->getType() == Message::CONTROL;

		/*if (_entity->getType().compare("Player")==0)
			return message->getType() == Message::CONTROL;*/
		 if (_entity->getType().compare("AnimatedEntity")==0)
			return message->getType() == Message::NPC_CONTROL;
	}

		
	 void CJump::process(CMessage *message)
		 {
		switch(message->getType())
		{
		case Message::CONTROL:
			{			
			if(message->getAction() == Message::JUMP) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
					jump();
			}
		case Message::NPC_CONTROL:
			{
		
			}
		}
	 }

	void CJump::jump()
	{
		if (_jumping)return;
		_jumping = true;
		//_initialJumpPower=0.075;
		//_initialGravity=0.1;
		//_jumpPower=_initialJumpPower;
		_gravity=_initialGravity;
		//_gravity=0.1;
	}

	void CJump::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);
			//aqui la lógica de salto
			if (_jumping)
			{
				//_entity->setHeight(_entity->getHeight()+(_jumpPower+_gravity)*msecs);	
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::JUMP);
				m->setFloat(_entity->getHeight()+(_jumpPower+_gravity)*msecs);
				_entity->emitMessage(m);

				_gravity-=0.01;
				if ((_jumpPower+_gravity)<0 && (_jumpPower+_gravity)>-0.015) //si esta suma es menor que cero es que está bajando. 
				{
					CMessageBoolString *message = new CMessageBoolString();
					message->setType(Message::SET_ANIMATION);
					message->setString("Crouch");
					message->setBool(false);
					_entity->emitMessage(message,this);
				}

				if (_entity->getHeight()<0)
				{
					//_gravidty
					_jumping=false;
					Logic::CMessageFloat *m = new Logic::CMessageFloat();
					m->setType(Logic::Message::AVATAR_MOVE);
					m->setAction(Logic::Message::JUMP);
					m->setFloat(0);
					_entity->emitMessage(m);

					//_entity->setHeight(0);					
					CMessageBoolString *message = new CMessageBoolString();
					message->setType(Message::SET_ANIMATION);
					message->setString("IdleKatana");
					message->setBool(true);
					_entity->emitMessage(message,this);
				}
			//	Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getHeight(),_entity->getBase(),_entity->getRing());
			
			//	_entity->setPosition(newPosition);
			

			}

	} //fin de CJump:tick


} // namespace Logic