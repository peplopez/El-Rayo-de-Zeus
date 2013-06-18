
#include "../../Logic/Entity/Components/Combat.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/PhysicalCharacter.h"
#include "../../Logic/Entity/Components/Jump.h"
#include "../../Logic/Entity/Components/AltarStateSwitcher.h"

#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"
#include "LA_ChangeRing.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageUIntString.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "../../Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Entity/Messages/MessageChar.h"


#include "../../Logic/Server.h"

namespace AI
{
////////////////////////////////
//	Implementación de CLA_ChangeRing
////////////////////////////////

	CLatentAction::LAStatus CLA_ChangeRing::OnStart()
	{					
		_changeDone = false;
		_initialJumpSpeed = (_action == Message::GO_DOWN ? 100 : 410); //elocidad de salto inicial en caso de dejarse caer o saltar arriba
		_initialRing = _entity->getLogicalPosition()->getRing();
		_jumpSpeed = _initialJumpSpeed;
		_jumpDecay =(_action == Message::GO_DOWN ? 400 : 570);
		_turnSpeedFactor= 0.005f/7; 
		
		sleepComponents();		//Desactivación de componentes
		std::cout<<"AI::StateMachine::ChangeRing"<<std::endl;
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setUShort(Logic::JUMP);				
		message->setBool(false);
		_entity->emitMessage(message);	
		turn();

		return SUSPENDED;
	}

	//---------------------------------------------------------

	void CLA_ChangeRing::OnStop()
	{
		std::cout<<"AI::StateMachine::ChangeRing-OnStop"<<std::endl;
		awakeComponents();
		if (_entity->hasComponent<CPhysicalCharacter>())
			_entity->getComponent<CPhysicalCharacter>()->enableCollisions();
	}

	//---------------------------------------------------------

	CLatentAction::LAStatus CLA_ChangeRing::OnRun() 
	{
		return RUNNING;
	}

	//---------------------------------------------------------

	CLatentAction::LAStatus CLA_ChangeRing::OnAbort() 
	{
		return FAIL;
	}

	//---------------------------------------------------------

	bool CLA_ChangeRing::accept(const CMessage *message){return false;}

	//---------------------------------------------------------

	void CLA_ChangeRing::process(CMessage *message)	{}
	
	//---------------------------------------------------------

	void CLA_ChangeRing::jump(unsigned int msecs) 
	{
		_jumpDelay -= msecs;
		if (!_jumping && _jumpDelay < 0)
		{
			_turning = false;
			_jumping = true;
			_maxHeightReached = false;
			if (_entity->hasComponent<CPhysicalCharacter>())
				_entity->getComponent<CPhysicalCharacter>()->disableCollisions();

		}
	} // jump
	
	//---------------------------------------------------------

	void CLA_ChangeRing::turn() 
	{
					
		if (!_turning)
		{
			if (_initialRing == Logic::Ring::CENTRAL_RING)
			{
				if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_CENTER)
					_jumpDelay = 700;
				else
					_targetSense = Logic::Sense::LOOKING_CENTER;
				if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_OUTSIDE)
					_turnSpeedFactor *=2;
			}
			
			else if ( _initialRing == Logic::Ring::LOWER_RING || _initialRing == Logic::Ring::UPPER_RING)
			{
				if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_OUTSIDE)
					_jumpDelay = 700;
				else
					_targetSense = Logic::Sense::LOOKING_OUTSIDE;
				if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_CENTER)
					_turnSpeedFactor *=2;
			}

			_turning = true;
		}
	} // turn

	//---------------------------------------------------------

	void CLA_ChangeRing::rotate(float yawAmount, unsigned int msecs)
	{
		float tickRotation;

		if (yawAmount > 0)
			tickRotation = Math::PI * _turnSpeedFactor * msecs;
		else
			tickRotation = - Math::PI * _turnSpeedFactor * msecs;
	
		_entity->yaw(tickRotation);
		_acumRotation += tickRotation;
		if (abs(_acumRotation) >= abs(yawAmount))
		{
			if (_acumRotation > 0)
				_entity->yaw(_acumRotation - yawAmount);
			else 
				_entity->yaw(-(_acumRotation - yawAmount));

			_entity->getLogicalPosition()->setSense(_targetSense);
			_targetSense = Logic::LogicalPosition::UNDEFINED;
			_acumRotation = 0.0f;
		}

	}

	//---------------------------------------------------------

	//PeP
	void CLA_ChangeRing::tick(unsigned int msecs) 
	{	
		CLatentAction::tick();

		if (_turning)
		{	
			if (_initialRing == Logic::LogicalPosition::CENTRAL_RING)
			{
				if (_entity->getLogicalPosition()->getSense() == Logic::Sense::RIGHT)
					rotate(Math::PI * 0.5f, msecs);
				else if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LEFT)
					rotate(-Math::PI * 0.5f, msecs);
				else if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_OUTSIDE)
					rotate(Math::PI, msecs);
				else if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_CENTER)
						jump(msecs);
			}
			else
			{
				if (_entity->getLogicalPosition()->getSense() == Logic::Sense::RIGHT)
					rotate(-Math::PI * 0.5f, msecs);
				else if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LEFT)
					rotate(Math::PI * 0.5f, msecs);
				else if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_CENTER)
					rotate(Math::PI, msecs);
				else if (_entity->getLogicalPosition()->getSense() == Logic::Sense::LOOKING_OUTSIDE)
					jump(msecs);
	
			}//else

		}//turning

		if (_jumping)
		{
			
			Logic::Ring ring = _entity->getLogicalPosition()->getRing();
			
			if (ring != _initialRing && _entity->getLogicalPosition()->getHeight() < 50)
			{
				_changeDone = true;
				if (_entity->hasComponent<CPhysicalCharacter>())
					_entity->getComponent<CPhysicalCharacter>()->enableCollisions();
			
			}
			
			//si estamos en trayectoria descendente activamos salida del salto
			if (_changeDone && _entity->getLogicalPosition()->getHeight() +  _jumpSpeed * msecs * 0.001 <= 0) 
			{

				_jumping = false;
				_jumpSpeed = _initialJumpSpeed;
				_jumpDelay = 0;
				finish(true);
			}
		
			else
	     	{
				float tickHeight = _jumpSpeed;
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::JUMP);
				m->setFloat(tickHeight);
				_entity->emitMessage(m);
				_jumpSpeed -= _jumpDecay * msecs * 0.001f; 

			}
		}

	}

	//---------------------------------------------------------

	void CLA_ChangeRing::sleepComponents()
	{
		if (_entity->hasComponent<CAvatarController>())
			_entity->getComponent<CAvatarController>()->sleep();	
		if (_entity->hasComponent<CJump>())
			_entity->getComponent<CJump>()->sleep();
	

	}

	//---------------------------------------------------------

	void CLA_ChangeRing::awakeComponents()
	{			
		if (_entity->hasComponent<CAvatarController>())
			_entity->getComponent<CAvatarController>()->awake();	
		if (_entity->hasComponent<CJump>())
			_entity->getComponent<CJump>()->awake();

	}	

} //namespace LOGIC