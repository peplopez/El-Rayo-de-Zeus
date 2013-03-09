/**
@file CameraController.cpp

Contiene la implementación de la clase CCameraController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CCameraController

@author Jose Luis López Sánchez
@date Febrero, 2013
*/

#include "CameraController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageFloat.h"

#include <cassert>

#define TURN_FACTOR 0.001f

namespace GUI {

	CCameraController::CCameraController() : _controlledCamera(0)
	{
		activate();
		/*PeP: ¿Alquien es capaz de explicarme por qué me he visto forzado a hacer esta ñapa para que se ejecute el Activate()?*/
	} // CCameraController

	//--------------------------------------------------------

	CCameraController::~CCameraController()
	{
		deactivate();

	} // ~CCameraController

	//--------------------------------------------------------

	void CCameraController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
	//	CInputManager::getSingletonPtr()->addMouseListener(this);

	} // activate

	//--------------------------------------------------------

	void CCameraController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
	//	CInputManager::getSingletonPtr()->removeMouseListener(this);

	} // deactivate

	//--------------------------------------------------------

	bool CCameraController::keyPressed(TKey key)
	{
		if(_controlledCamera)
		{
			Logic::CMessageBoolFloat *m = new Logic::CMessageBoolFloat();
			m->setType(Logic::Message::CAMERA);
			m->setFloat(1.0f);
			switch(key.keyId)
			{			
			case GUI::Key::UPARROW:
				m->setBool(false);		
				break;
			case GUI::Key::DOWNARROW:
				m->setBool(true);
				break;			
			default:
				return false;
			}
			_controlledCamera->emitMessage(m);
			return true;
		}
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CCameraController::keyReleased(TKey key)
	{
		if(_controlledCamera)
		{
			Logic::CMessageBoolFloat *m = new Logic::CMessageBoolFloat();
			m->setType(Logic::Message::CONTROL);
			switch(key.keyId)
			{
			case GUI::Key::A:
			case GUI::Key::D:
				//m._string = "stopStrafe";
				m->setAction(Logic::Message::WALK_STOP);
				break;

			default:
				return false;
			}
		//	_controlledCamera->emitMessage(m);
			return true;
		}
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CCameraController::mouseMoved(const CMouseState &mouseState)
	{
		
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CCameraController::mousePressed(const CMouseState &mouseState)
	{
		if(!_controlledCamera)
			return false;
		else
		{
			Logic::CMessageBoolFloat *m = new Logic::CMessageBoolFloat();
			m->setType(Logic::Message::CAMERA);
			m->setFloat(1.0f);
			switch(mouseState.button)
			{			
			case GUI::Button::LEFT:
				m->setBool(false);		
				break;
			case GUI::Button::RIGHT:
				m->setBool(true);
				break;			
			default:
				return false;
			}
			_controlledCamera->emitMessage(m);
			return true;
		}
	} // mousePressed

	//--------------------------------------------------------

	bool CCameraController::mouseReleased(const CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace GUI
