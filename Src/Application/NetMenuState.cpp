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

#include "NetMenuState.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CNetMenuState::~CNetMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CNetMenuState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("NetMenu.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("NetMenu");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		
		CEGUI::WindowManager::getSingleton().getWindow("NetMenu/Server")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CNetMenuState::serverReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("NetMenu/Client")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CNetMenuState::clientReleased, this));

		CEGUI::WindowManager::getSingleton().getWindow("NetMenu/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CNetMenuState::backReleased, this));

		return true;

	} // init

	//--------------------------------------------------------

	void CNetMenuState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CNetMenuState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

	} // activate

	//--------------------------------------------------------

	void CNetMenuState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CNetMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CNetMenuState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CNetMenuState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::S:
			_app->setState("lobbyserver");
			break;
		case GUI::Key::C:
			_app->setState("lobbyclient");
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CNetMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CNetMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CNetMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CNetMenuState::serverReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("lobbyserver");
		return true;

	} // serverReleased
			
	//--------------------------------------------------------

	bool CNetMenuState::clientReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("lobbyclient");
		return true;

	} // clientReleased

		//--------------------------------------------------------

	bool CNetMenuState::backReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("menu");
		return true;

	} // backReleased

} // namespace Application
