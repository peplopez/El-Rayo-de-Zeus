/**
@file Jump2.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CJump2
@see Logic::IComponent

@author 
@date
*/

#include "Jump2.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "AvatarController.h"


#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"



namespace Logic 
{
	IMP_FACTORY(CJump2);
	
	//---------------------------------------------------------

	bool CJump2::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;				
	

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CJump2::activate()
	{

		return true;
	} // activate
	
	//---------------------------------------------------------

	void CJump2::deactivate(){} // deactivate
	
	//---------------------------------------------------------

	bool CJump2::accept(const CMessage *message)
	{
		return (message->getType() == Message::CONTROL && 
					message->getAction() == Message::JUMP);
	}
	
	//---------------------------------------------------------

	void CJump2::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::JUMP)
					jump();
		}
	} // process
	
	//---------------------------------------------------------

	void CJump2::jump() 
	{
		if (!_jumping)
		{
			_jumping = true;
			_initialHeight = _entity->getHeight();
		}

		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setString("IdleKatana");
		message->setBool(true);
		_entity->emitMessage(message,this);

	} // stopWalk
	
	//---------------------------------------------------------

	void CJump2::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (_jumping)
		{
			_jumpSpeed -= 0.0003 * msecs;
			float tickHeight = _entity->getHeight() + _jumpSpeed * msecs;
			if (tickHeight < 0) 
			{
				_jumping=false;
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::JUMP);
				m->setFloat(0);
				_entity->emitMessage(m);
				_jumpSpeed = 0.1f;
				if (_entity->getComponent<CAvatarController>()->isWalkingRight())
					_entity->getComponent<CAvatarController>()->walkRight();
				else if (_entity->getComponent<CAvatarController>()->isWalkingLeft())
					_entity->getComponent<CAvatarController>()->walkLeft();
			}
			else
			{
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::JUMP);
				m->setFloat(tickHeight);
				_entity->emitMessage(m);
			}
		}		
	} // tick

} // namespace Logic