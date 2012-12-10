/**
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
#include "Logic/Entity/Message.h"

#include <cassert>

#define TURN_FACTOR 0.001f

namespace GUI {

	CPlayerController::CPlayerController() : _controlledAvatar(0)
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

	} // activate

	//--------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);

	} // deactivate

	//--------------------------------------------------------

	bool CPlayerController::keyPressed(TKey key)
	{
		if(_controlledAvatar)
		{
			Logic::TMessage m;
			m._type = Logic::Message::CONTROL;
			switch(key.keyId)
			{
			case GUI::Key::W:
				m._string = "walk";
				break;
			case GUI::Key::S:
				m._string = "walkBack";
				break;
			case GUI::Key::A:
				m._string = "strafeLeft";
				break;
			case GUI::Key::D:
				m._string = "strafeRight";
				break;
			default:
				return false;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			Logic::TMessage m;
			m._type = Logic::Message::CONTROL;
			switch(key.keyId)
			{
			case GUI::Key::W:
			case GUI::Key::S:
				m._string = "stopWalk";
				break;

			case GUI::Key::A:
			case GUI::Key::D:
				m._string = "stopStrafe";
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
		if(_controlledAvatar)
		{
			Logic::TMessage m;
			m._type = Logic::Message::CONTROL;
			m._string = "turn";
			m._float = -(float)mouseState.movX * TURN_FACTOR;
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CPlayerController::mouseReleased(const CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace GUI
