//---------------------------------------------------------------------------
// Cegui.cpp
//---------------------------------------------------------------------------

/**
@file Cegui.cpp

Contiene la implementación de la clase que representa un Cegui.

@see Graphics::CCegui

@author Pablo Terrado
@date Enero, 2013
*/

//#include "Cegui.h"
//
//#include "BaseSubsystems/Math.h"
//
//#include <assert.h>
//
//#include <cegui\CEGUISystem.h>
//#include <cegui\CEGUIWindowManager.h>
//#include <cegui\CEGUIWindow.h>
//
//
//namespace Graphics 
//{	
//
//	/**
//	Constructor de la clase CCegui
//	Se le pasa como parametro el name, que es el nombre del fichero .layout a cargar
//	*/
//	CCegui::CCegui(const std::string &name) :
//		_name(name), _cegui(0)
//	{
//
//		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout(name);
//		assert( !CEGUI::WindowManager::getSingletonPtr()->isWindowPresent(name) && "Ya existe una ventana CEGUI con el mismo nombre!" );
//		//_cegui = CEGUI::WindowManager::getSingleton().getWindow(name);	//esto se hace en cargarventana	
//
//	} // CCegui
//
//	//------------------------------------------------------------
//
//	/**
//	Destructor de la clase CCegui
//	*/
//	CCegui::~CCegui(){
//		if(_cegui) 
//			_cegui->destroy();
//
//	} // ~CCegui		
//	
//
//	void CCegui::cargarFicheroLayout(const std::string &nameLayout){
//		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout(nameLayout);
//	}
//
//	void CCegui::setText(const std::string &text){
//	}
//
//	/**
//	Devuelve una ventana definida por codigo o en el XML del fichero .layout
//	*/
//	void CCegui::cargarVentana(const std::string &nameWindow){
//		_cegui = CEGUI::WindowManager::getSingleton().getWindow(nameWindow);
//	}
//		
//	CEGUI::Window* CCegui::conseguirHijo(const std::string &nameChild){
//		return _cegui->getChild(nameChild);
//	}
//
//	void CCegui::establecerPropiedad(CEGUI::Window* _window, const std::string &nameProperty, const std::string &valueProperty){
//		_window->setProperty(nameProperty,valueProperty);
//	}
//
//	
//
//
//} // namespace Graphics
