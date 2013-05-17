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
#include "ScriptManager\Server.h"

namespace GUI {

	CHudController::CHudController()
	{
		//activate();
	} // CHudController

	//--------------------------------------------------------

	CHudController::~CHudController()
	{
		//deactivate();
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
