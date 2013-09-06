//---------------------------------------------------------------------------
// PauseState.cpp
//---------------------------------------------------------------------------

/**
@file PauseState.cpp

Contiene el estado de pausa del juego.

@see Application::CApplicationState
@see Application::CPauseState

@author Pablo Terrado
@date Septiembre, 2013
*/

#include "PauseState.h"
#include "TimeConstants.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "GUI/CameraController.h" //PT
#include "GUI/HudController.h" //PT
#include "GUI/ShopController.h" //PT

#include "Physics/Server.h"

#include "Logic/GameStatus.h"
#include "Logic/BaseInfo.h"
#include "Logic/RingInfo.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"

//PT
#include <BaseSubsystems/Server.h>

namespace Application {

	// PT al iniciar el estado de pausa
	bool CPauseState::init() 
	{
		CApplicationState::init();

		/*
		// Cargamos la ventana del PAUSE
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Pause.layout");

		_pauseWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Pause");

		//Loading of Pause.lua script
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("Pause");

		*/

		return true;

	
	} // init

	//--------------------------------------------------------

	// ƒ®§ Al cerrar la app
	void CPauseState::release() 
	{

		CApplicationState::release();
	} // release

	//--------------------------------------------------------

	// ƒ®§ Al entrar en PauseState (cambio de currentState)
	void CPauseState::activate() 
	{

		CApplicationState::activate();

		//Se activa la ventana Pause
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showPause");

		//PT Hide mousecursor when PAUSE game
		CEGUI::MouseCursor::getSingletonPtr()->hide();



	} // activate

	//--------------------------------------------------------

	//  ƒ®§ Al salir de PauseState (cambio de currentState)
	void CPauseState::deactivate() 
	{

		//TODO
		//Reiniciar el reloj de Ogre

		//PT si no estamos en ventana completa, entonces utilizamos el raton de Windows y no el de CEGUI
		#if _WIN32	
				if(BaseSubsystems::CServer::getSingletonPtr()->isWindowedMode() )
					//CEGUI::MouseCursor::getSingletonPtr()->setVisible(false); //FER
					CEGUI::MouseCursor::getSingletonPtr()->hide();				//PT
		#endif
		
		// Desactivamos la ventana GUI con el menú y el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hidePause");
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CPauseState::tick(unsigned int msecs) 
	{
		

	} // tick

	
	/**************
		INPUT
	*************/

	bool CPauseState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	// PT. Para salir del estado de Pause y volver al estado de Game
	// se necesita pulsar la tecla P (ó lo que sea GUI::Key::PAUSE)
	bool CPauseState::keyReleased(GUI::TKey key)
	{

		switch(key.keyId)
		{

		case GUI::Key::PAUSE:
			_app->setState("game");
			break;

		case GUI::Key::P:
			_app->setState("game");
			break;

		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPauseState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPauseState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CPauseState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
