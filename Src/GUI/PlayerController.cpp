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
				if (_controlledAvatar->getRing()==Logic::LogicalPosition::ANILLO_SUPERIOR)
					return false;
				m._string = "goUp"; // Pablo
				break;			
			case GUI::Key::S: //bajar anillo inferior
				if (_controlledAvatar->getRing()==Logic::LogicalPosition::ANILLO_INFERIOR)
					return false;
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
				if (_controlledAvatar->getBase()==m._float)
					return false;

				break;
			case GUI::Key::NUMBER2:
				m._string = "changeBase";
				m._float = 2;
				if (_controlledAvatar->getBase()==m._float)
					return false;
				break;			
			case GUI::Key::NUMBER3:
				m._string = "changeBase";
				m._float = 3;
				if (_controlledAvatar->getBase()==m._float)
					return false;
				break;
			case GUI::Key::NUMBER4:
				m._string = "changeBase";
				m._float = 4;
				if (_controlledAvatar->getBase()==m._float)
					return false;
				break;			
			case GUI::Key::NUMBER5:
				m._string = "changeBase";
				m._float = 5;
				if (_controlledAvatar->getBase()==m._float)
					return false;
				break;
			case GUI::Key::NUMBER6:
				m._string = "changeBase";
				m._float = 6;
				if (_controlledAvatar->getBase()==m._float)
					return false;
				break;			
			case GUI::Key::NUMBER7:
				m._string = "changeBase";
				m._float = 7;
				if (_controlledAvatar->getBase()==m._float)
					return false;
				break;
			case GUI::Key::NUMBER8:
				m._string = "changeBase";
				m._float = 8;
				if (_controlledAvatar->getBase()==m._float)
					return false;
				break;
			case GUI::Key::NUMBER0:
				m._string = "changeBase";
				m._float = 0;
				if (_controlledAvatar->getBase()==m._float)
					return false;
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
		{//PEP: he estado mirando para que no se pare el personaje si tienes pulsada una tecla de dirección al juguetear con los controles
			//TKey teclaA=TKey(65,GUI::Key::A);
			//TKey teclaD=TKey(68,GUI::Key::D);				
			Logic::TMessage m;
			m._type = Logic::Message::CONTROL;
			switch(key.keyId)
			{
			case GUI::Key::A:				
			//	if (!keyPressed(teclaD)){
				m._string = "walkStop";				
				break;
			//	}
			case GUI::Key::D:
			//	if (!keyPressed(teclaA)){
				m._string = "walkStop";
				break;
				//}
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
