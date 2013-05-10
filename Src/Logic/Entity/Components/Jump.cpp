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

		if(entityInfo->hasAttribute("jumpSpeed"))
		{
			_jumpSpeed = entityInfo->getFloatAttribute("jumpSpeed");
			_initialJumpSpeed = entityInfo->getFloatAttribute("jumpSpeed");
		}
	
		if(entityInfo->hasAttribute("jumpDecay"))
			_jumpDecay = entityInfo->getFloatAttribute("jumpDecay");


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
			_maxHeightReached = false;
		}
	} // stopWalk
	
	//---------------------------------------------------------

	void CJump::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (_jumping)
		{
			//si estamos en trayectoria descendente activamos salida del salto

			if (_jumpSpeed < 0)
				_maxHeightReached = true;
			
			if (_maxHeightReached && _entity->getLogicalPosition()->getHeight() == _lastTickHeight) 
			{
				_jumping=false;
				_jumpSpeed = _initialJumpSpeed;
			}
			else
			{
				float tickHeight = _jumpSpeed * msecs * 0.001;
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::JUMP);
				m->setFloat(tickHeight);
				_entity->emitMessage(m);
				_jumpSpeed -= _jumpDecay * msecs * 0.001f;
				_lastTickHeight = _entity->getLogicalPosition()->getHeight();
			}
		}		
	} // tick

} // namespace Logic