//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llansó
@date Agosto, 2010
*/

#include "MenuState.h"
#include "BaseApplication.h"
#include "Clock.h"
#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"

//PT
#include <BaseSubsystems/Server.h>

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CMenuState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		/*
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Menu.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("Menu");
		*/

		// Cargamos la ventana que muestra el menú con LUA
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("Menu");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initMenu");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Single")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::startReleased, this));

		CEGUI::WindowManager::getSingleton().getWindow("Menu/Multi")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::multiplayerReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Exit")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::exitReleased, this));

		// Asociamos los botones del menú con las funciones que se deben ejecutar desde LUA
		//ScriptManager::CServer::getSingletonPtr()->executeProcedure("asociarEventosMenu");

		return true;

	} // init

	//--------------------------------------------------------

	void CMenuState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CMenuState::activate() 
	{
		CApplicationState::activate();

		CBaseApplication::getSingletonPtr()->getClock()->removeAllTimeObservers();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		/*
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();
		*/

		// Activamos la ventana que nos muestra el menú y activamos el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showMenu");

		// In case we come back from game to MenuState
		// HACK FRS Windowed? -> Desactivar el cursor de CEGUI y superponer el de WIN32
		#if _WIN32	
				if(BaseSubsystems::CServer::getSingletonPtr()->isWindowedMode() )
					CEGUI::MouseCursor::getSingletonPtr()->hide();
		#endif

	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		/*
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		*/

		// Desactivamos la ventana GUI con el menú y el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideMenu");

		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CMenuState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(GUI::TKey key)
	{
		Logic::TMapNames _mapsToLoad;

		switch(key.keyId)
		{

		case GUI::Key::ESCAPE:
			_app->exitRequest();
			break;

		case GUI::Key::RETURN:
			_app->setState("menusingle");
			
			break;

		case GUI::Key::M:
			_app->setState("netmenu");
			break;
			
		default:
			return false;
			
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
// TODO Por qué se devuelve  true o false?
// TODO Toda la carga y creación del jugador deberia encapsularse en un startGame() reusable
	bool CMenuState::startReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("menusingle");

		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->exitRequest();
		return true;

	} // exitReleased

	//--------------------------------------------------------

	bool CMenuState::multiplayerReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("netmenu");
		return true;

	} // multiplayerReleased


} // namespace Application
