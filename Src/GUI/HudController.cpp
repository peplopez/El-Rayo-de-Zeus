/**
@file HudController.cpp

Contiene la implementación de la clase CHudController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CHudController

@author Jose Luis López Sánchez
@date Febrero, 2013
*/

#include "HudController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageFloat.h"

#include "ScriptManager\Server.h"

#include <cassert>

#define TURN_FACTOR 0.001f

namespace GUI {

	CHudController::CHudController()
	{
		activate();
	} // CHudController

	//--------------------------------------------------------

	CHudController::~CHudController()
	{
		deactivate();
	} // ~CHudController

	//--------------------------------------------------------

	void CHudController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
	} // activate

	//--------------------------------------------------------

	void CHudController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
	} // deactivate

	//--------------------------------------------------------

	bool CHudController::keyPressed(TKey key)
	{
		/*
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
			return keySent;*/
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CHudController::keyReleased(TKey key)
	{
		switch(key.keyId)
		{
			case GUI::Key::R:
				ScriptManager::CServer::getSingletonPtr()->reloadScript("Hud");
				ScriptManager::CServer::getSingletonPtr()->executeProcedure("reloadHud");
				break;
			default:
				return false;
		}
		return true;

	} // keyReleased


} // namespace GUI
