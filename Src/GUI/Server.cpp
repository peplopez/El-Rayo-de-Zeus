/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la interfaz con el usuario (entrada de periféricos, CEGui...).

@see GUI::CServer

@author David Llansó
@date Agosto, 2010
*/


#include "Server.h"

#include "PlayerController.h"
#include "CameraController.h"
#include "HudController.h" //PT
#include "ShopController.h" //PT

#include <BaseSubsystems/Server.h>
#include <ScriptingModules\LuaScriptModule\CEGUILua.h> //PT
#include <ScriptManager\Server.h> //PT

#include <cassert>
#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIProgressBar.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUICheckbox.h>


namespace GUI {

	
	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	//PT. Antes. CServer::CServer() : _playerController(0)

	
	CServer::CServer() : _playerController(0), _cameraController(0), _hudController(0), _shopController(0)
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	
	CServer::~CServer()
	{
		_instance = 0;
		_currentWindow = NULL;

	} // ~CServer
	
	//--------------------------------------------------------

	
	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de GUI::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	
	void CServer::Release()
	{
		assert(_instance && "GUI::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	
	bool CServer::open()
	{
		_playerController = new CPlayerController();
		_cameraController = new CCameraController();
		_hudController = new CHudController(); //PT
		_shopController = new CShopController(); //PT

		_GUISystem = BaseSubsystems::CServer::getSingletonPtr()->getGUISystem();
		_windowManager = CEGUI::WindowManager::getSingletonPtr();

		//PT
		//Carga de las plantillas y archivos de fuentes con LUA en lugar de con CEGUI
		//No hace falta pasarle toda la ruta del script porque se supone que todos los scripts de LUA
		//van a estar en media/scripts . Tampoco hace falta pasarle la extension .lua.
		//Todo ello se hace en la carga del script. ScriptManager::CServer::loadScript()
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("GUI");
		//_GUISystem->setDefaultMouseCursor(0)
		// HACK FRS Windowed? -> Desactivar el cursor de CEGUI y superponer el de WIN32
#if _WIN32		
		if(BaseSubsystems::CServer::getSingletonPtr()->isWindowedMode() )
			_GUISystem->setDefaultMouseCursor(0);
#endif


#ifndef NON_EXCLUSIVE_MODE_IN_WINDOW_MODE 
		// Establecemos cual será el puntero del ratón.
		_GUISystem->setDefaultMouseCursor("OgreTrayImages","MouseArrow");
#endif	

		CInputManager::getSingletonPtr()->addMouseListener(this);
		CInputManager::getSingletonPtr()->addKeyListener(this);

		return true;

	} // open

	//--------------------------------------------------------

	
	void CServer::close() 
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);

		delete _playerController;
		delete _cameraController;
		delete _hudController; //PT
		delete _shopController; //PT
	} // close

	//--------------------------------------------------------

	
	bool CServer::keyPressed(TKey key)
	{
		_GUISystem->injectKeyDown(key.keyId);    
		_GUISystem->injectChar(key.text);    

		
		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // keyPressed

	//--------------------------------------------------------

	
	bool CServer::keyReleased(TKey key)
	{
		_GUISystem->injectKeyUp(key.keyId);

		
		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	
	bool CServer::mouseMoved(const CMouseState &mouseState)
	{
#if defined NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
		_GUISystem->injectMousePosition((float)mouseState.posAbsX,(float)mouseState.posAbsY);
#else 
		_GUISystem->injectMouseMove((float)mouseState.movX,(float)mouseState.movY);
#endif	
		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	
	bool CServer::mousePressed(const CMouseState &mouseState)
	{
		switch (mouseState.button)
		{
		case Button::LEFT:
			_GUISystem->injectMouseButtonDown(CEGUI::LeftButton);
		case Button::RIGHT:
			_GUISystem->injectMouseButtonDown(CEGUI::RightButton);
		case Button::MIDDLE:
			_GUISystem->injectMouseButtonDown(CEGUI::MiddleButton);
		}

		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // mousePressed

	//--------------------------------------------------------

	
	bool CServer::mouseReleased(const CMouseState &mouseState)
	{
		switch (mouseState.button)
		{
		case Button::LEFT:
			_GUISystem->injectMouseButtonUp(CEGUI::LeftButton);
		case Button::RIGHT:
			_GUISystem->injectMouseButtonUp(CEGUI::RightButton);
		case Button::MIDDLE:
			_GUISystem->injectMouseButtonUp(CEGUI::MiddleButton);
		}

		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // mouseReleased




	
	/***************
		WINDOW
	****************/
	
	
	void CServer::setText(const std::string& msg)
	{
		_currentWindow->setText(msg.c_str());
	} // setText

	//--------------------------------------------------------

	
	void CServer::setWindowEnabled(const std::string& windowName, bool isEnable)
	{
		getWindow(windowName)->setEnabled(isEnable);
	} // setWindowEnable

	//--------------------------------------------------------


	
	void CServer::setWindowVisible(const std::string& windowName, bool isVisible)
	{
		getWindow(windowName)->setVisible(isVisible);
	} // setWindowVisible

	//--------------------------------------------------------

	
	std::string CServer::getWindowText(const std::string& textWindow)
	{
		std::string& text = std::string( _windowManager->getWindow( textWindow )->getText().c_str() );		
			text.erase(text.find_last_not_of(" \n\r\t")+1);//Se eliminan los espacios (trim)
		return text;
	}


	
	

	//-------- PROGRESS BAR --------------------

	
	void CServer::updateProgress(
		const std::string& barWindow, const std::string& statusWindow,
		float progressAmount, const std::string& statusMsg) 
	{
		CEGUI::ProgressBar* bar = static_cast<CEGUI::ProgressBar*> (
			getWindow( barWindow ));
		
		//PT I dont want to show progresBar when form has not been filled
		// Only when progressBar is up to 0 is when I want to show it
		if(progressAmount > 0.0f)
		{
			if(	!bar->isVisible() )
				bar->setVisible(true);
		}

		bar->setProgress(progressAmount);
		if(statusMsg.length() ){
			CEGUI::Window* statusW = getWindow(statusWindow);
			statusW->setText(statusMsg);
			statusW->setVisible(true);
			//getWindow(statusWindow)->setText(statusMsg);
		}
		
		
	} // addProgress


	//--------- COMBOBOX ----------------------

	
	CEGUI::Combobox* CServer::createCombobox(const std::string& comboWindow, 
		const std::string* itemTexts, const unsigned int nItems) 
	{		
		CEGUI::Combobox* combo = static_cast<CEGUI::Combobox*>(
			_windowManager->getWindow(comboWindow) );			
			
		for(int i=0; i < nItems; ++i) // add items to the combobox list
			combo->addItem(	new CEGUI::ListboxTextItem( itemTexts[i], i));
			// FRS AutoDelete = true por defecto (no hace falta delete)	

		combo->setReadOnly(true);		
		return combo;
	}
	
	//---------------------------------------------------------

	
	int CServer::getComboSelectedID(const std::string& comboWindow) 
	{		
		CEGUI::Combobox* combo = static_cast<CEGUI::Combobox*>(
			_windowManager->getWindow(comboWindow) );		
				
		if( combo->getSelectedItem() !=  0)
			return combo->getSelectedItem()->getID();
		else
			return -1;
	}
	
	//---------------------------------------------------------

	bool CServer::isCheckboxSelected(const std::string& checkBox)
	{
		CEGUI::Checkbox* check = static_cast<CEGUI::Checkbox*>(
			_windowManager->getWindow(checkBox));

		return check->isSelected();
	}
	

	//---------------------------------------------------------
	std::string CServer::getComboSelectedText(const std::string& comboWindow) 
	{		
		CEGUI::Combobox* combo = static_cast<CEGUI::Combobox*>(
			CEGUI::WindowManager::getSingletonPtr()->getWindow(comboWindow) );			
			
		if( combo->getSelectedItem() !=  0)
			return std::string(combo->getSelectedItem()->getText().c_str() );
		else
			return "";
	}

} // namespace GUI
