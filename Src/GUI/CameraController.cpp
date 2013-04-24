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
			m->setFloat(0.6f);
			switch(key.keyId)
			{			
			case GUI::Key::UPARROW:
				m->setBool(true);		
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
			switch(key.keyId)
			{
			case GUI::Key::A:
			case GUI::Key::D:

			default:
				return false;
			}
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
	} // mousePressed

	//--------------------------------------------------------

	bool CCameraController::mouseReleased(const CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace GUI
