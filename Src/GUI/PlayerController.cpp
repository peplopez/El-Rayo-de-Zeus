/**
RECUERDOS DE MERCHE
@file PlayerController.cpp

Contiene la implementación de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llansó
@date Agosto, 2010
*/


#include "PlayerController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageString.h"

#include "Physics/Server.h"

#include <cassert>


namespace GUI {

	CPlayerController::CPlayerController() : _controlledAvatar(0), _changeBaseAllowed(false), _joyModifierPressed(false)
	{
		
	} // CPlayerController

	//--------------------------------------------------------

	CPlayerController::~CPlayerController()
	{
		deactivate();

	} // ~CPlayerController

	//--------------------------------------------------------

	void CPlayerController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);
		CInputManager::getSingletonPtr()->addJoystickListener(this);


	} // activate

	//--------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
		CInputManager::getSingletonPtr()->removeJoystickListener(this);

	} // deactivate

	//--------------------------------------------------------
	int CPlayerController::getKeyType(const TKey &key)
	{
		switch (key.keyId)
		{
		case GUI::Key::W:
		case GUI::Key::S:
		case GUI::Key::A:
		case GUI::Key::D:
			return 0; //movement
		case GUI::Key::SPACE:
		case GUI::Key::F:
		case GUI::Key::LSHIFT:
			return 1; //actions
		case GUI::Key::NUMBER1:
		case GUI::Key::NUMBER2:
		case GUI::Key::NUMBER3:
		case GUI::Key::NUMBER4:
		case GUI::Key::NUMBER5:
		case GUI::Key::NUMBER6:
		case GUI::Key::NUMBER7:
		case GUI::Key::NUMBER8:
			return 2;//bases
		case GUI::Key::V:
		case GUI::Key::F2:
			return 3;//extras
		default:
			return -1;//tecla no procesada por el PlayerController
		}

	}

	//--------------------------------------------------------
	
	bool CPlayerController::keyPressed(TKey key)
	{
		// TODO Preguntar al CServer quién es el player si es null y dejarlo guardado
		if(_controlledAvatar)
		{
			int keyType = getKeyType(key);

			switch (keyType)
			{
			case 0:
				processMovement(key);
				break;
			case 1:
				processAction(key);
				break;
			case 2:
				showBase(fromKeyToBaseNumber(key));
				break;
			case 3:
				processExtra(key);
			case -1:
				return false;
			}
		}

		return true;
	}

	//--------------------------------------------------------
	
	void CPlayerController::processMovement(TKey key)
	{
		Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
	
		switch(key.keyId)
		{
		case GUI::Key::W: //subir anillo superior
			if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::UPPER_RING)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::GO_UP);
			break;
			
		case GUI::Key::S: //bajar anillo inferior
			if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::LOWER_RING)
			{
				delete m;
				return;
			}			
			m->setAction(Logic::Message::GO_DOWN); 
			break;

		case GUI::Key::A:
			m->setAction(Logic::Message::WALK_LEFT);
			break;

		case GUI::Key::D:
			m->setAction(Logic::Message::WALK_RIGHT);	
			break;
		}

		_controlledAvatar->emitMessage(m);
	}

	//--------------------------------------------------------
	
	void CPlayerController::processAction(TKey key)
	{
		Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
		switch(key.keyId)
		{
		case GUI::Key::SPACE:
			if (_changeBaseAllowed)
				m->setAction(Logic::Message::CHANGE_BASE);
			else
				m->setAction(Logic::Message::JUMP); 	
			break;
		case GUI::Key::F:
			m->setAction(Logic::Message::SWITCH_ALTAR);
			break;
		case GUI::Key::LSHIFT:
			m->setAction(Logic::Message::COVER);
			break;
		}

		_controlledAvatar->emitMessage(m);
	}

	//--------------------------------------------------------
	
	void CPlayerController::processExtra(TKey key)
	{
		Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::HUD);
		switch(key.keyId)
		{
		case GUI::Key::V:
			m->setAction(Logic::Message::DISPLAY_HUD);
			_controlledAvatar->emitMessage(m);
			break;
		case GUI::Key::F2:
			delete m;
			Physics::CServer::getSingletonPtr()->SwitchDebugDraw();
			break;
		}

	}

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			switch(key.keyId)
			{
			case GUI::Key::A:
			case GUI::Key::D:
				m->setAction(Logic::Message::WALK_STOP);
				break;
			case GUI::Key::LSHIFT:
				m->setAction(Logic::Message::NO_COVER);
				break;
			case GUI::Key::NUMBER1:
			case GUI::Key::NUMBER2:
			case GUI::Key::NUMBER3:
			case GUI::Key::NUMBER4:
			case GUI::Key::NUMBER5:
			case GUI::Key::NUMBER6:
			case GUI::Key::NUMBER7:
			case GUI::Key::NUMBER8:
				m->setAction(Logic::Message::GOBACK_TO_BASE);
				_changeBaseAllowed = false;
				break;
			default:
				delete m;
				return false;
			}

			_controlledAvatar->emitMessage(m);
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
			switch(mouseState.button)
			{
			case GUI::Button::LEFT:				
				m->setAction(Logic::Message::LIGHT_ATTACK);						
				break;
			case GUI::Button::RIGHT:				
				m->setAction(Logic::Message::HEAVY_ATTACK);
				break;			
			default:
				delete m;
				return true;
			}
			_controlledAvatar->emitMessage(m);
		}

		return true;
	} // mousePressed

	//--------------------------------------------------------

	bool CPlayerController::mouseReleased(const CMouseState &mouseState)
	{
		return false;
	} // mouseReleased

	//--------------------------------------------------------

	bool CPlayerController::axisMoved(const CJoystickState *joystickState, TJoyAxis axis)
	{
		if (_controlledAvatar)
		{

			switch (axis)
			{
			case TJoyAxis::MOVEXAXIS:

				if (joystickState->_axes[TJoyAxis::MOVEXAXIS].abs > 10000)
				{
					Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::CONTROL);
					m->setAction(Logic::Message::WALK_RIGHT);
					_controlledAvatar->emitMessage(m);
				}
				else if (joystickState->_axes[TJoyAxis::MOVEXAXIS].abs < -10000)
				{
					Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::CONTROL);
					m->setAction(Logic::Message::WALK_LEFT);
					_controlledAvatar->emitMessage(m);
				}
					
				else if (abs(joystickState->_axes[TJoyAxis::MOVEXAXIS].abs) < 10000)
				{
					Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::CONTROL);
					m->setAction(Logic::Message::WALK_STOP);
					_controlledAvatar->emitMessage(m);
				}

				else
				{
					return true;
				}

				break;

			case TJoyAxis::MOVEYAXIS:

				if (joystickState->_axes[TJoyAxis::MOVEYAXIS].abs > 20000)
				{
					if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::LOWER_RING)
					{
						return true;	
					}
					Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::CONTROL);
					m->setAction(Logic::Message::GO_DOWN);
					_controlledAvatar->emitMessage(m);
				}

				else if (joystickState->_axes[TJoyAxis::MOVEYAXIS].abs < -20000)
				{
					if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::UPPER_RING)
					{
						return true;	
					}
					Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::CONTROL);
					m->setAction(Logic::Message::GO_UP);
					_controlledAvatar->emitMessage(m);
				}
				
				else 
				{
					return true;
				}
	
				break;
			}

		}
		return true;
	}

	//--------------------------------------------------------

	bool CPlayerController::buttonPressed(const CJoystickState *joystickState, TJoyButton button)
	{
		if (_controlledAvatar)
		{
			Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);

			switch (button)
			{
			case Joystick::Button::ATTACK1:
				m->setAction(Logic::Message::LIGHT_ATTACK);
				break;

			case Joystick::Button::ATTACK2:
				m->setAction(Logic::Message::HEAVY_ATTACK);
				break;

			case Joystick::Button::JUMP:
				if (_changeBaseAllowed)
					m->setAction(Logic::Message::CHANGE_BASE);
				else
					m->setAction(Logic::Message::JUMP);
				break;

			case Joystick::Button::ACTIVATE:
				m->setAction(Logic::Message::SWITCH_ALTAR);
				break;

			case Joystick::Button::COVER:
				m->setAction(Logic::Message::COVER);
				break;

			case Joystick::Button::MODIFIER:
				_joyModifierPressed = true;
				delete m;
				return true;
			}

			_controlledAvatar->emitMessage(m);

		}

		return true;
	}

	//--------------------------------------------------------

	bool CPlayerController::buttonReleased(const CJoystickState *joystickState, TJoyButton button)
	{
		if (_controlledAvatar)
		{
			Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);

			switch (button)
			{
			case Joystick::Button::COVER:
				m->setAction(Logic::Message::NO_COVER);
				break;

			case Joystick::Button::MODIFIER:
				_joyModifierPressed = false;
				_changeBaseAllowed = false;
				m->setAction(Logic::Message::GOBACK_TO_BASE);
				break;
			default:
				delete m;
				return true;
			}

			_controlledAvatar->emitMessage(m);
		}
		
		return true;
	}

	//--------------------------------------------------------

	bool CPlayerController::povMoved(const CJoystickState *joystickState)
	{
		if (_controlledAvatar)
		{
			if (_joyModifierPressed)
				showBase(fromPOVToBaseNumber(joystickState));
			else
				processMovement(joystickState);
		}

		return true;
	}

	//--------------------------------------------------------
	void CPlayerController::processMovement(const CJoystickState *joystickState)
	{
		Logic::CMessage *m = new Logic::CMessage();
		m->setType(Logic::Message::CONTROL);

		switch (joystickState->_pov)
		{
		case Joystick::POV::CENTERED:
			m->setAction(Logic::Message::WALK_STOP);
			break;

		case Joystick::POV::WEST:
		case Joystick::POV::NORTHWEST:
		case Joystick::POV::SOUTHWEST:
			m->setAction(Logic::Message::WALK_LEFT);
			break;
			
		case Joystick::POV::EAST:
		case Joystick::POV::NORTHEAST:
		case Joystick::POV::SOUTHEAST:
			m->setAction(Logic::Message::WALK_RIGHT);
			break;

		case Joystick::POV::NORTH:
			if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::UPPER_RING)
			{
				delete m;
				return;	
			}
			m->setAction(Logic::Message::GO_UP);
			break;

		case Joystick::POV::SOUTH:
			if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::LOWER_RING)
			{
				delete m;
				return;	
			}
			m->setAction(Logic::Message::GO_DOWN);
			break;
		}

		_controlledAvatar->emitMessage(m);

	}

	//--------------------------------------------------------

	void CPlayerController::showBase(int baseNumber)
	{
		if (_controlledAvatar->getLogicalPosition()->getBase() == baseNumber)
			return;

		Logic::CMessageUShort *m = new Logic::CMessageUShort();
			m->setType(Logic::Message::CONTROL);
		Logic::CMessage *m2 = new Logic::CMessage();
			m2->setType(Logic::Message::CONTROL);
		_changeBaseAllowed = true;

		switch (baseNumber)
		{
		case 0:
			_changeBaseAllowed = false;
			m->setAction(Logic::Message::GOBACK_TO_BASE);
			m2->setAction(Logic::Message::WALK_STOP);
			_controlledAvatar->emitMessage(m2);
			break;

		default:
			delete m2;
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(baseNumber);								
			break;
			
		}

		_controlledAvatar->emitMessage(m);

	}

	//--------------------------------------------------------

	int CPlayerController::fromPOVToBaseNumber(const CJoystickState* joystickState)
	{
		
		switch (joystickState->_pov)
		{
		case Joystick::POV::CENTERED:
			return 0;

		case Joystick::POV::NORTH:
			return 1;

		case Joystick::POV::EAST:
			return 2;		

		case Joystick::POV::SOUTH:
			return 3;	

		case Joystick::POV::WEST:
			return 4;

		case Joystick::POV::NORTHEAST:
			return 5;

		case Joystick::POV::SOUTHEAST:
			return 6;

		case Joystick::POV::SOUTHWEST:
			return 7;

		case Joystick::POV::NORTHWEST:
			return 8;	
		}

	}

	//--------------------------------------------------------

	int CPlayerController::fromKeyToBaseNumber(TKey key)
	{	
		switch (key.keyId)
		{

		case GUI::Key::NUMBER1:
			return 1;

		case GUI::Key::NUMBER2:
			return 2;		

		case GUI::Key::NUMBER3:
			return 3;	

		case GUI::Key::NUMBER4:
			return 4;

		case GUI::Key::NUMBER5:
			return 5;

		case GUI::Key::NUMBER6:
			return 6;

		case GUI::Key::NUMBER7:
			return 7;

		case GUI::Key::NUMBER8:
			return 8;
			
		}

	}


	
} // namespace GUI
