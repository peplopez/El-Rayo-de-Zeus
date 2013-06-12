/**
@file HudController.cpp

Contiene la implementación de la clase CHudController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CHudController

@author Pablo Terrado Contreras
@date Mayo, 2013
*/

#include "HudController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "ScriptManager\Server.h"
#include "Logic/Entity/Messages/Message.h"


namespace GUI {

	CHudController::CHudController(): _controlledHud(0)
	{
		//activate();
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
		CInputManager::getSingletonPtr()->addMouseListener(this);
	} // activate

	//--------------------------------------------------------

	void CHudController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
	} // deactivate

	//--------------------------------------------------------

	bool CHudController::keyPressed(TKey key)
	{
		return false;
	} // keyPressed

	//--------------------------------------------------------

	bool CHudController::keyReleased(TKey key)
	{

		if(_controlledHud)
		{

			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::HUD);

			switch(key.keyId)
			{

				//to reload HUD for DEBUG
				case GUI::Key::R:
					ScriptManager::CServer::getSingletonPtr()->reloadScript("Hud");
					ScriptManager::CServer::getSingletonPtr()->executeProcedure("reloadHud");
					break;

				default:
					return false;
			}

			return true;
		}
		return false;

	} // keyReleased


} // namespace GUI
