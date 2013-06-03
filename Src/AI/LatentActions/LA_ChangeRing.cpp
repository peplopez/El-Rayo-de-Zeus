
#include "../../Logic/Entity/Components/Combat.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/PhysicalCharacter.h"
#include "../../Logic/Entity/Components/Jump.h"
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
		_turnSpeed= 0.005f/7; 
		sleepComponents();		//Desactivación de componentes
		std::cout<<"AI::StateMachine::ChangeRing"<<std::endl;
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setAction(Message::JUMP);				
		message->setUShort(Logic::JUMP);
		message->setBool(false);
		_entity->emitMessage(message);		
		turn();
		return SUSPENDED;
	}


	void CLA_ChangeRing::OnStop()
	{
		std::cout<<"AI::StateMachine::ChangeRing-OnStop"<<std::endl;
		awakeComponents();
	}


	CLatentAction::LAStatus CLA_ChangeRing::OnRun() 
	{
		if (this->getStatus()!=SUCCESS && this->getStatus()!=FAIL)
			return RUNNING;
		else 
			return this->getStatus();
	}


	CLatentAction::LAStatus CLA_ChangeRing::OnAbort() 
	{
		return FAIL;
	}

	bool CLA_ChangeRing::accept(const CMessage *message){return false;}

	void CLA_ChangeRing::process(CMessage *message)	{}
		
	void CLA_ChangeRing::jump() 
	{
		if (!_jumping)
		{
			_jumping = true;
			_maxHeightReached = false;
		}
	} // jump
	
	void CLA_ChangeRing::turn() 
	{
		if (!_turning)
			_turning = true;			
	} // jump

	//PeP
	void CLA_ChangeRing::tick(unsigned int msecs) 
	{	
		
		if (_turning)
		{	
			if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT || _entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
				_targetSense = Logic::LogicalPosition::LOOKING_CENTER;
		
			if (_initialRing == Logic::LogicalPosition::CENTRAL_RING)
			{
				if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT)
				{
					float tickRotation = Math::PI * _turnSpeed * msecs;
					_entity->yaw(tickRotation);
					_acumRotation += tickRotation;
					_mySense=Logic::LogicalPosition::RIGHT;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
					
					}
				}
				else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
				{
					float tickRotation = Math::PI * _turnSpeed * msecs;
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					_mySense=Logic::LogicalPosition::LEFT;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(_acumRotation - Math::PI/2);
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
					}
				}
			
			else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LOOKING_CENTER)
			{
				if (_targetSense == Logic::LogicalPosition::RIGHT)
				{   
					
					float tickRotation = Math::PI * _turnSpeed* msecs; 
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(_acumRotation - Math::PI/2);
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::RIGHT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;						
						if (_changeDone)
							finish(true);
					}
				}
				else if (_targetSense == Logic::LogicalPosition::LEFT)
				{
					float tickRotation = Math::PI *_turnSpeed* msecs;
					_entity->yaw(tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LEFT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
						if (_changeDone)
							finish(true);
					}
				}
				else 
				{
					_turning=false;
					jump();
					if (_entity->getComponent<CPhysicalCharacter>()!=NULL)
						_entity->getComponent<CPhysicalCharacter>()->disableCollisions();
				}
			}
		}
		else
		{
			if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT)
			{
				float tickRotation = Math::PI * _turnSpeed * msecs;
				_entity->yaw(-tickRotation);
				_acumRotation += tickRotation;
				_mySense=Logic::LogicalPosition::RIGHT;
				if (_acumRotation >= Math::PI/2)
				{
					_entity->yaw(+(_acumRotation - Math::PI/2));
					_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
					_targetSense = Logic::LogicalPosition::UNDEFINED;
					_acumRotation = 0;
					
				}
			}
			else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
			{
				float tickRotation = Math::PI * _turnSpeed * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
				_entity->yaw(+tickRotation);
				_acumRotation += tickRotation;
				_mySense=Logic::LogicalPosition::LEFT;
				if (_acumRotation >= Math::PI/2)
				{
					_entity->yaw(-(_acumRotation - Math::PI/2));
					_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
					_targetSense = Logic::LogicalPosition::UNDEFINED;
					_acumRotation = 0;
				}
			}
			
			else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LOOKING_CENTER)
			{
				if (_targetSense == Logic::LogicalPosition::RIGHT)
				{   					
					float tickRotation = Math::PI * _turnSpeed* msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
					_entity->yaw(+tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::RIGHT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
						if (_changeDone)
							finish(true);
					}
				}
				else if (_targetSense == Logic::LogicalPosition::LEFT)
				{
					float tickRotation = Math::PI *_turnSpeed* msecs;
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(+(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LEFT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
						if (_changeDone)
							finish(true);
					}
				}
				else 
				{
					_turning=false;
					jump();
					if (_entity->getComponent<CPhysicalCharacter>()!=NULL)
						_entity->getComponent<CPhysicalCharacter>()->disableCollisions();
				}
			}
		}//else
		}//turning

		if (_jumping)
		{
			//si estamos en trayectoria descendente activamos salida del salto
			Logic::Ring ring = _entity->getLogicalPosition()->getRing();
			
			if (ring != _initialRing)
			{
				_changeDone=true;
			}
			
			if (_changeDone && _entity->getLogicalPosition()->getHeight() + 3 * _jumpSpeed * msecs * 0.001 <= 0) 
			{
				if (_entity->getComponent<CPhysicalCharacter>()!=NULL)
					_entity->getComponent<CPhysicalCharacter>()->enableCollisions();
				_jumping=false;
				_targetSense=_mySense;
				_jumpSpeed=_initialJumpSpeed;
				turn();
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

		CLatentAction::tick();//no olvideis llamar al tick de CLatentAction
	}

	void CLA_ChangeRing::sleepComponents()
	{
		if (_entity->getComponent<CAvatarController>()!=NULL)
			_entity->getComponent<CAvatarController>()->sleep();	
		if (_entity->getComponent<CJump>()!=NULL)
			_entity->getComponent<CJump>()->sleep();

	}

	void CLA_ChangeRing::awakeComponents()
	{			
		if (_entity->getComponent<CAvatarController>()!=NULL)
			_entity->getComponent<CAvatarController>()->awake();	
		if (_entity->getComponent<CJump>()!=NULL)
			_entity->getComponent<CJump>()->awake();
	}	
} //namespace LOGIC