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
	//	CInputManager::getSingletonPtr()->addMouseListener(this);

	} // activate

	//--------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
	//	CInputManager::getSingletonPtr()->removeMouseListener(this);

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
			case GUI::Key::W: //subir anillo superior
				m._string = "goUp"; // Pablo
				break;
			
			case GUI::Key::S: //bajar anillo inferior
				m._string = "goDown";
				break;
			case GUI::Key::SPACE:
				m._string = "jump"; // Pablo
				break;
			case GUI::Key::A:
				m._string = "walkLeft";
				break;
			case GUI::Key::D:
				m._string = "walkRight";
				break;
			case GUI::Key::NUMBER1:
				m._string = "changeBase";
				m._float = 1;
				break;
			case GUI::Key::NUMBER2:
				m._string = "changeBase";
				m._float = 2;
				break;			
			case GUI::Key::NUMBER3:
				m._string = "changeBase";
				m._float = 3;
				break;
			case GUI::Key::NUMBER4:
				m._string = "changeBase";
				m._float = 4;
				break;			
			case GUI::Key::NUMBER5:
				m._string = "changeBase";
				m._float = 5;
				break;
			case GUI::Key::NUMBER6:
				m._string = "changeBase";
				m._float = 6;
				break;			
			case GUI::Key::NUMBER7:
				m._string = "changeBase";
				m._float = 7;
				break;
			case GUI::Key::NUMBER8:
				m._string = "changeBase";
				m._float = 8;
				break;
			case GUI::Key::NUMBER0:
				m._string = "changeBase";
				m._float = 0;
				break;
			case GUI::Key::V:
				m._string = "displayVisor";
				break;
			case GUI::Key::ADD:
				m._string = "addPlayerToBase";
				break;
			case GUI::Key::M:
				m._string = "minusPlayerToBase";
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
			case GUI::Key::A:
			case GUI::Key::D:
				//m._string = "stopStrafe";
				m._string = "walkStop";
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
