//---------------------------------------------------------------------------
// InitState.cpp
//---------------------------------------------------------------------------

/**
@file InitState.cpp

Contiene la implementación del estado de menú inicial.

@see Application::CApplicationState
@see Application::CInitState

@author Pablo Terrado
@date Marzo, 2013
*/

#include "InitState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"

namespace Application {

	CInitState::~CInitState() 
	{
	} // ~CInitState

	//--------------------------------------------------------

	bool CInitState::init() 
	{
		CApplicationState::init();


		// Cargamos la ventana que muestra el inicio con CEGUI
		/*
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Inicio.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("Inicio");
		*/

		//TODOPT
		// Cargamos el script del menu inicio.
		// Ejecutamos el proc que muestra la ventana de Inicio con LUA (el script es Inicio.lua)
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("Inicio");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initMenuInicio");

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
		/*
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();
		*/

		//Activamos la ventana que nos muestra el menú y activamos el ratón con LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showMenuInicio");

	} // activate

	//--------------------------------------------------------

	void CInitState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		/*
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		*/

		// Desactivamos la ventana GUI con el menú y el ratón con LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideMenuInicio");
		
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
