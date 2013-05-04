/**
@file Jump.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CJump
@see Logic::IComponent

@author 
@date
*/

#include "Jump.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "AvatarController.h"


#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"



namespace Logic 
{
	IMP_FACTORY(CJump);
	
	//---------------------------------------------------------

	bool CJump::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;				
	

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CJump::activate()
	{
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CJump::deactivate()
	{
	} // deactivate
	
	//---------------------------------------------------------

	bool CJump::accept(const CMessage *message)
	{
		return (isAwake() && message->getType() == Message::CONTROL && 
					message->getAction() == Message::JUMP);
	}
	
	//---------------------------------------------------------

	void CJump::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::JUMP)
					jump();
		}
	} // process
	
	//---------------------------------------------------------

	void CJump::jump() 
	{
		if (!_jumping)
		{
			_jumping = true;
			_justJumped = true;
		}
	} // stopWalk
	
	//---------------------------------------------------------

	void CJump::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (_jumping)
		{
			_jumpSpeed -= 0.0004f * msecs;  //gravedad 0.0003f
			float tickHeight = _jumpSpeed * msecs;

			//si estamos en trayectoria descendente activamos salida del salto
			if (_jumpSpeed < 0)
				_justJumped = false;
			
			if (_entity->getLogicalPosition()->getHeight() == 0 && !_justJumped) 
			{
				_jumping=false;
				_jumpSpeed = 0.13f;
				//assert(_entity->getComponent<CAvatarController>()==NULL);
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