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

#include <Logic/Entity/Entity.h>
#include <Logic/Entity/Messages/Message.h>
#include <Logic/Entity/Messages/MessageUShort.h>

#include <Logic/Maps/Map.h>

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
	keyType CPlayerController::getKeyType(const TKey &key)
	{
		switch (key.keyId)
		{
		case GUI::Key::W:
		case GUI::Key::S:
		case GUI::Key::A:
		case GUI::Key::D:
			return movement; //movement
		case GUI::Key::SPACE:
		case GUI::Key::F:
		case GUI::Key::LSHIFT:
			return action; //actions
		case GUI::Key::NUMBER1:
		case GUI::Key::NUMBER2:
		case GUI::Key::NUMBER3:
		case GUI::Key::NUMBER4:
		case GUI::Key::NUMBER5:
		case GUI::Key::NUMBER6:
		case GUI::Key::NUMBER7:
		case GUI::Key::NUMBER8:
			return base;//bases
		case GUI::Key::V:
		case GUI::Key::F2:
			return extra;//extras
		default:
			return notManaged;//tecla no procesada por el PlayerController
		}

	}

	//--------------------------------------------------------
	
	bool CPlayerController::keyPressed(TKey key)
	{
		// TODO Preguntar al CServer quién es el player si es null y dejarlo guardado
		if(_controlledAvatar)
		{
			keyType typeOfKey = getKeyType(key);

			switch (typeOfKey)
			{
			case movement:
				processMovement(key);
				break;
			case action:
				processAction(key);
				break;
			case base:
				showBase(fromKeyToBaseNumber(key));
				break;
			case extra:
				processExtra(key);
			case notManaged:
				return false;
			}
		}

		return true;
	}

	//--------------------------------------------------------
	
	void CPlayerController::processMovement(TKey key)
	{

		switch(key.keyId)
		{
		case GUI::Key::W: { //subir anillo superior
			if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::UPPER_RING)
				return;
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::GO_UP);
			_controlledAvatar->emitMessage(m);
			break;
			}
			
		case GUI::Key::S: {//bajar anillo inferior
			if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::LOWER_RING)
				return;	
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);	
			m->setAction(Logic::Message::GO_DOWN);
			_controlledAvatar->emitMessage(m);
			break;
			}

		case GUI::Key::A: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::WALK_LEFT);
			_controlledAvatar->emitMessage(m);
			break;
			}

		case GUI::Key::D: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::WALK_RIGHT);
			_controlledAvatar->emitMessage(m);
			break;
			}
		}
	}

	//--------------------------------------------------------
	
	void CPlayerController::processAction(TKey key)
	{
		
		switch(key.keyId)
		{
		case GUI::Key::SPACE: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			if (_changeBaseAllowed)
			{
				m->setAction(Logic::Message::CHANGE_BASE);
				_changeBaseAllowed = false;
			}
			else
			{
				m->setAction(Logic::Message::JUMP);
			}
			_controlledAvatar->emitMessage(m);
			break;
			}
		case GUI::Key::F: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::SWITCH_ALTAR);
			_controlledAvatar->emitMessage(m);
			break;
			}
		case GUI::Key::LSHIFT: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::COVER);
			_controlledAvatar->emitMessage(m);
			break;
			}
		}
	}

	//--------------------------------------------------------
	
	void CPlayerController::processExtra(TKey key)
	{

		switch(key.keyId)
		{
		case GUI::Key::V: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::HUD);
			m->setAction(Logic::Message::DISPLAY_HUD);
			_controlledAvatar->emitMessage(m);
			break;
			}
		case GUI::Key::F2:
			_controlledAvatar->getMap()->switchDebugDraw();
			break;
		}

	}

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{

			switch(key.keyId)
			{
			case GUI::Key::A:
			case GUI::Key::D: {
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::WALK_STOP);
				_controlledAvatar->emitMessage(m);
				break;
				}
			case GUI::Key::LSHIFT: {
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::NO_COVER);
				_controlledAvatar->emitMessage(m);
				break;
				}
			case GUI::Key::NUMBER1:
			case GUI::Key::NUMBER2:
			case GUI::Key::NUMBER3:
			case GUI::Key::NUMBER4:
			case GUI::Key::NUMBER5:
			case GUI::Key::NUMBER6:
			case GUI::Key::NUMBER7:
			case GUI::Key::NUMBER8: {
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::GOBACK_TO_BASE);
				_controlledAvatar->emitMessage(m);
				_changeBaseAllowed = false;
				break;
				}
			default:
				return false;
			}
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
			switch(mouseState.button)
			{
			case GUI::Button::LEFT:	{
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::LIGHT_ATTACK);
				_controlledAvatar->emitMessage(m);
				break;
				}
			case GUI::Button::RIGHT:
				{
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::HEAVY_ATTACK);
				_controlledAvatar->emitMessage(m);
				break;
				}
			default:
				return false;
			}
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
				break;

			case TJoyAxis::MOVEYAXIS:

				if (joystickState->_axes[TJoyAxis::MOVEYAXIS].abs > 20000)
				{
					if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::LOWER_RING)
						return true;	
					Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::CONTROL);
					m->setAction(Logic::Message::GO_DOWN);
					_controlledAvatar->emitMessage(m);
				}
				else if (joystickState->_axes[TJoyAxis::MOVEYAXIS].abs < -20000)
				{
					if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::UPPER_RING)
						return true;	
					Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::CONTROL);
					m->setAction(Logic::Message::GO_UP);
					_controlledAvatar->emitMessage(m);
				}
				break;
			
			default: 
				return false;
			}
		}

		return true;
	}

	//--------------------------------------------------------

	bool CPlayerController::buttonPressed(const CJoystickState *joystickState, TJoyButton button)
	{
		if (_controlledAvatar)
		{
			switch (button)
			{
			case Joystick::Button::ATTACK1: {
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::LIGHT_ATTACK);
				_controlledAvatar->emitMessage(m);
				break;
				}

			case Joystick::Button::ATTACK2: {
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::HEAVY_ATTACK);
				_controlledAvatar->emitMessage(m);
				break;
				}

			case Joystick::Button::JUMP:
				{
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				if (_changeBaseAllowed)
					m->setAction(Logic::Message::CHANGE_BASE);
				else
					m->setAction(Logic::Message::JUMP);
				_controlledAvatar->emitMessage(m);
				break;
				}

			case Joystick::Button::ACTIVATE: {			
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::SWITCH_ALTAR);
				_controlledAvatar->emitMessage(m);
				break;
				}

			case Joystick::Button::COVER: {
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::COVER);
				_controlledAvatar->emitMessage(m);
				break;
				}

			case Joystick::Button::MODIFIER: 
				_joyModifierPressed = true;

			default:
				return false;
			}
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
			case Joystick::Button::COVER: {
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::NO_COVER);
				_controlledAvatar->emitMessage(m);
				break;
			}

			case Joystick::Button::MODIFIER: {
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				_joyModifierPressed = false;
				_changeBaseAllowed = false;
				m->setAction(Logic::Message::GOBACK_TO_BASE);
				_controlledAvatar->emitMessage(m);
				break;
			}
			
			default:
				return false;
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
				showBase(fromPOVToBaseNumber(joystickState));
			else
				processMovement(joystickState);
		}

		return true;
	}

	//--------------------------------------------------------
	void CPlayerController::processMovement(const CJoystickState *joystickState)
	{


		switch (joystickState->_pov)
		{
		case Joystick::POV::CENTERED: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::WALK_STOP);
			_controlledAvatar->emitMessage(m);
			break;
			}

		case Joystick::POV::WEST:
		case Joystick::POV::NORTHWEST:
		case Joystick::POV::SOUTHWEST: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::WALK_LEFT);
			_controlledAvatar->emitMessage(m);
			break;
			}
			
		case Joystick::POV::EAST:
		case Joystick::POV::NORTHEAST:
		case Joystick::POV::SOUTHEAST: {
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::WALK_RIGHT);
			_controlledAvatar->emitMessage(m);
			break;
			}

		case Joystick::POV::NORTH: {
			if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::UPPER_RING)
				return;	
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::GO_UP);
			_controlledAvatar->emitMessage(m);
			break;
			}

		case Joystick::POV::SOUTH: {
			if (_controlledAvatar->getLogicalPosition()->getRing() == Logic::LogicalPosition::LOWER_RING)
				return;	
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::GO_DOWN);
			_controlledAvatar->emitMessage(m);
			break;
			}
		}
		

	}

	//--------------------------------------------------------

	void CPlayerController::showBase(int baseNumber)
	{
		if (_controlledAvatar->getLogicalPosition()->getBase() == baseNumber)
			return;

		_changeBaseAllowed = true;

		Logic::CMessage *msgStop = new Logic::CMessage();
		msgStop->setType(Logic::Message::CONTROL);
		msgStop->setAction(Logic::Message::WALK_STOP);
		_controlledAvatar->emitMessage(msgStop);

		Logic::CMessageUShort *m = new Logic::CMessageUShort();
		m->setType(Logic::Message::CONTROL);
		m->setAction(Logic::Message::SHOW_BASE);
		m->setUShort(baseNumber);
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
