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

#include "ScriptManager\Server.h"

#include <cassert>

#define TURN_FACTOR 0.001f

namespace GUI {

	CCameraController::CCameraController() : _controlledCameras(0)
	{
		//activate();
		/*PeP: ¿Alquien es capaz de explicarme por qué me he visto forzado a hacer esta ñapa para que se ejecute el Activate()?*/
		//PT. 
		//Desde GameState.cpp en el activate (CGameState::activate) al igual que se llama a 
		//GUI::CServer::getSingletonPtr()->getPlayerController()->activate();
		//llamamos a
		//GUI::CServer::getSingletonPtr()->getCameraController()->activate();
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
		if(!_controlledCameras.empty())
		{
			bool keySent = false;
			TEntities::const_iterator it = _controlledCameras.begin();
			TEntities::const_iterator end = _controlledCameras.end();
			for (; it != end; ++it)
			{
				Logic::CMessageBoolFloat *m = new Logic::CMessageBoolFloat();
				m->setType(Logic::Message::CAMERA);
				m->setFloat(20.0f);
				switch(key.keyId)
				{			
					case GUI::Key::UPARROW:
						m->setBool(true);		
						break;
					case GUI::Key::DOWNARROW:
						m->setBool(false);
					break;	
					//PT. Caso en el que se recargan todos los scripts
					case GUI::Key::R:
						ScriptManager::CServer::getSingletonPtr()->reloadScript("Hud");
						ScriptManager::CServer::getSingletonPtr()->executeProcedure("reloadHud");
						break;
					default:
						return false;
				}
				(*it)->emitMessage(m);
				keySent = true;
			}
			return keySent;
		}
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CCameraController::keyReleased(TKey key)
	{
		if(!_controlledCameras.empty())
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
		
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CCameraController::mouseReleased(const CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace GUI
