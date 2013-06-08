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

#define TURN_FACTOR 0.001f

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

	bool CPlayerController::keyPressed(TKey key)
	{
		// TODO Preguntar al CServer quién es el player si es null y dejarlo guardado
		if(_controlledAvatar)
		{
			Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);

			Logic::CMessageUShort *m2 = new Logic::CMessageUShort();
				m2->setType(Logic::Message::CONTROL);

			Logic::CMessage *m3 = new Logic::CMessage();
				m3->setType(Logic::Message::HUD);

			switch(key.keyId)
			{
			case GUI::Key::W: //subir anillo superior
				if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::UPPER_RING)
					return true;			
				m->setAction(Logic::Message::GO_UP);
				_controlledAvatar->emitMessage(m);
				break;
			
			case GUI::Key::S: //bajar anillo inferior
				if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::LOWER_RING)
					return true;			
				m->setAction(Logic::Message::GO_DOWN); 
				_controlledAvatar->emitMessage(m);
				break;

			case GUI::Key::SPACE:
				if (_changeBaseAllowed)
					m->setAction(Logic::Message::CHANGE_BASE);
				else
					m->setAction(Logic::Message::JUMP); 
				    _controlledAvatar->emitMessage(m);
				break;

			case GUI::Key::A:
				m->setAction(Logic::Message::WALK_LEFT);
				_controlledAvatar->emitMessage(m);
				break;

			case GUI::Key::D:
				m->setAction(Logic::Message::WALK_RIGHT);
				_controlledAvatar->emitMessage(m);
				break;

			case GUI::Key::F:
				m->setAction(Logic::Message::SWITCH_ALTAR);
				_controlledAvatar->emitMessage(m);
				break;

			case GUI::Key::LSHIFT:
				m->setAction(Logic::Message::COVER);
				_controlledAvatar->emitMessage(m);
				break;

			case GUI::Key::NUMBER1:
				m2->setAction(Logic::Message::SHOW_BASE);
				m2->setUShort(1);								
				if (_controlledAvatar->getLogicalPosition()->getBase()==m2->getUShort())
					return false;
				_changeBaseAllowed = true;
				_controlledAvatar->emitMessage(m2);
				break;

			case GUI::Key::NUMBER2:
				m2->setAction(Logic::Message::SHOW_BASE);
				m2->setUShort(2);
				if (_controlledAvatar->getLogicalPosition()->getBase()==m2->getUShort())
					return false;
				_changeBaseAllowed = true;
				_controlledAvatar->emitMessage(m2);
				break;	

			case GUI::Key::NUMBER3:
				m2->setAction(Logic::Message::SHOW_BASE);
				m2->setUShort(3);
				if (_controlledAvatar->getLogicalPosition()->getBase()==m2->getUShort())
					return false;
				_changeBaseAllowed = true;;
				_controlledAvatar->emitMessage(m2);
				break;

			case GUI::Key::NUMBER4:
				m2->setAction(Logic::Message::SHOW_BASE);
				m2->setUShort(4);
				if (_controlledAvatar->getLogicalPosition()->getBase()==m2->getUShort())
					return false;
				_changeBaseAllowed = true;
				_controlledAvatar->emitMessage(m2);
				break;	

			case GUI::Key::NUMBER5:
				m2->setAction(Logic::Message::SHOW_BASE);
				m2->setUShort(5);
				if (_controlledAvatar->getLogicalPosition()->getBase()==m2->getUShort())
					return false;
				_changeBaseAllowed = true;
				_controlledAvatar->emitMessage(m2);
				break;

			case GUI::Key::NUMBER6:
				m2->setAction(Logic::Message::SHOW_BASE);
				m2->setUShort(6);
				if (_controlledAvatar->getLogicalPosition()->getBase()==m2->getUShort())
					return false;
				_changeBaseAllowed = true;
				_controlledAvatar->emitMessage(m2);
				break;	

			case GUI::Key::NUMBER7:
				m2->setAction(Logic::Message::SHOW_BASE);
				m2->setUShort(7);
				if (_controlledAvatar->getLogicalPosition()->getBase()==m2->getUShort())
					return false;
				_changeBaseAllowed = true;
				_controlledAvatar->emitMessage(m2);
				break;

			case GUI::Key::NUMBER8:
				m2->setAction(Logic::Message::SHOW_BASE);
				m2->setUShort(8);
				if (_controlledAvatar->getLogicalPosition()->getBase()==m2->getUShort())
					return false;
				_changeBaseAllowed = true;
				_controlledAvatar->emitMessage(m2);
				break;

			case GUI::Key::V:
				m3->setAction(Logic::Message::DISPLAY_HUD);
				_controlledAvatar->emitMessage(m3);
				break;

			case GUI::Key::F2:
				Physics::CServer::getSingletonPtr()->SwitchDebugDraw();
				break;

			/*case GUI::Key::ADD:
				m._string = "addPlayerToBase";
				break;
			case GUI::Key::M:
				m._string = "minusPlayerToBase";
				break;*/

			}
			
			return true;
		}
		return true;

	} // keyPressed

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
				return false;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		if(!_controlledAvatar)
			return false;
		else
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
				return false;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
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
			Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
			switch (axis)
			{
			case TJoyAxis::MOVEXAXIS:

				if (joystickState->_axes[TJoyAxis::MOVEXAXIS].abs > 5000)
				{
					m->setAction(Logic::Message::WALK_RIGHT);
					_controlledAvatar->emitMessage(m);
				}
				else if (joystickState->_axes[TJoyAxis::MOVEXAXIS].abs < -5000)
				{
					m->setAction(Logic::Message::WALK_LEFT);
					_controlledAvatar->emitMessage(m);
				}
					
				if (abs(joystickState->_axes[TJoyAxis::MOVEXAXIS].abs) < 20)
				{
						m->setAction(Logic::Message::WALK_STOP);
						_controlledAvatar->emitMessage(m);
				}

				return true;
				break;

			case TJoyAxis::MOVEYAXIS:

				if (joystickState->_axes[TJoyAxis::MOVEYAXIS].abs > 5000)
				{
					m->setAction(Logic::Message::GO_DOWN);
					_controlledAvatar->emitMessage(m);
				}

				if (joystickState->_axes[TJoyAxis::MOVEYAXIS].abs < -5000)
				{
					m->setAction(Logic::Message::GO_UP);
					_controlledAvatar->emitMessage(m);
				}
				
				if (abs(joystickState->_axes[TJoyAxis::MOVEYAXIS].abs) < 20)
				{
						m->setAction(Logic::Message::WALK_STOP);
						_controlledAvatar->emitMessage(m);
				}

					
				return true;
				break;
			}

		}
		return false;
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
			switch (button)
			{
			case Joystick::Button::MODIFIER:
				_joyModifierPressed = false;
				_changeBaseAllowed = false;
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::GOBACK_TO_BASE);
				_controlledAvatar->emitMessage(m);
				break;
			}
		}
		
		return true;
	}

	//--------------------------------------------------------

	bool CPlayerController::povMoved(const CJoystickState *joystickState)
	{
		if (_controlledAvatar)
		{
			if (_joyModifierPressed)
				showBase(joystickState);
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

	void CPlayerController::showBase(const CJoystickState *joystickState)
	{

		Logic::CMessageUShort *m = new Logic::CMessageUShort();
		m->setType(Logic::Message::CONTROL);
		_changeBaseAllowed = true;

		switch (joystickState->_pov)
		{
		case Joystick::POV::CENTERED:
			_changeBaseAllowed = false;
			m->setAction(Logic::Message::GOBACK_TO_BASE);
			break;

		case Joystick::POV::NORTH:
			if (_controlledAvatar->getLogicalPosition()->getBase() == 1)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(1);								
			break;

		case Joystick::POV::EAST:
			if (_controlledAvatar->getLogicalPosition()->getBase() == 2)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(2);								
			break;

		case Joystick::POV::SOUTH:
			if (_controlledAvatar->getLogicalPosition()->getBase() == 3)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(3);								
			break;

		case Joystick::POV::WEST:
			if (_controlledAvatar->getLogicalPosition()->getBase() == 4)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(4);								
			break;

		case Joystick::POV::NORTHEAST:
			if (_controlledAvatar->getLogicalPosition()->getBase() == 5)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(5);								
			break;

		case Joystick::POV::SOUTHEAST:
			if (_controlledAvatar->getLogicalPosition()->getBase() == 6)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(6);								
			break;


		case Joystick::POV::SOUTHWEST:
			if (_controlledAvatar->getLogicalPosition()->getBase() == 7)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(7);								
			break;

		case Joystick::POV::NORTHWEST:
			if (_controlledAvatar->getLogicalPosition()->getBase() == 8)
			{
				delete m;
				return;
			}
			m->setAction(Logic::Message::SHOW_BASE);
			m->setUShort(8);								
			break;
			
		}

		_controlledAvatar->emitMessage(m);

	}

	
} // namespace GUI
