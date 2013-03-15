//---------------------------------------------------------------------------
// InitState.cpp
//---------------------------------------------------------------------------

/**
@file InitState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CInitState

@author David Llansó
@date Agosto, 2010
*/

#include "InitState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>

namespace Application {

	CInitState::~CInitState() 
	{
	} // ~CInitState

	//--------------------------------------------------------

	bool CInitState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el inicio
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Inicio.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("Inicio");

		return true;

	} // init

	//--------------------------------------------------------

	void CInitState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CInitState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

	} // activate

	//--------------------------------------------------------

	void CInitState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CInitState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CInitState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CInitState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
			case GUI::Key::ESCAPE:
				_app->exitRequest();
				break;
			case GUI::Key::RETURN:
				_app->setState("menu");
			case GUI::Key::SPACE:
				_app->setState("menu");
			default:
				return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CInitState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CInitState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CInitState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
