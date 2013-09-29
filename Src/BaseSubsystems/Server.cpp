//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

/**
@file Server.cpp

Contiene la implementaci�n de la clase principal que engloba los subsistemas
usados. La mayor�a de ellos son parte de Ogre.

@see BaseSubsystems::CServer

@author David Llans�
@date Julio, 2010
*/

#include "Server.h"

#include <assert.h>

// Ogre
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreTimer.h>
#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
#include <OgreException.h>
#include <OgreWindowEventUtilities.h>

// OIS
#include <OISInputManager.h>
#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>

//PT. LUA (Indicacion en http://mastervj.fdi.ucm.es/dokuwiki/doku.php?id=ayuda_proyectos:cegui)
#include <ScriptingModules/LuaScriptModule/CEGUILua.h>
#include "ScriptManager\Server.h"

// CEGUI
#include <CEGUISystem.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIFont.h>
#include <CEGUIImageset.h>
#include <CEGUIWindowManager.h>
#include <falagard/CEGUIFalWidgetLookManager.h>
#include <CEGUIScheme.h>

// Para cerrar la aplicaci�n si se cierra la ventana
#include <Application/BaseApplication.h>

#if _WIN32	
	#include <resource.h>
#endif


/**
Si se define la siguiente directiva, en modo ventana se reenderiza aunque
la ventana est� en background (siempre que est� visible)
*/
#define RENDER_IN_BACKGROUND

namespace BaseSubsystems 
{

	const std::string CServer::_WINDOW_TITLE = "Zeus Thunderbolt";

	/**
	Clase auxiliar que hereda de Ogre y que sirve para modificar el 
	comportamiento habitual de cierre de ventana. No queremos cerrarla
	desde aqu�, queremos que sea la aplicaci�n.
	*/
	class WindowEventListener : public Ogre::WindowEventListener
	{

	public:
		WindowEventListener()
		{
		}
		
		/**
		En vez de dejar que el m�todo de salida habitual funcione 
		lo reescribimos para que se le comunique a la aplicaci�n.
		Al devolver false la ventana no se cierra, lo haremos desde
		la aplicaci�n.
		<p>
		Este m�todo es llamado al solicitar el cierre de la ventana
		tanto con Alt-F4 como al pulsar la 'x' de la ventana.

		@param renderWindow Ventana sobre la que se produce el evento.
		@return Siempre falso, as� no se realiza ninguna acci�n.
		*/
		bool windowClosing(Ogre::RenderWindow* renderWindow)
		{
			Application::CBaseApplication::getSingletonPtr()->exitRequest();
			return false;
		}
	};

	CServer *CServer::_instance = 0;

	CServer::CServer() : 
		_root(0),
		_renderWindow(0),
		_debugRenderWindow(0),
		_mouse(0),
		_keyboard(0),
		_joystick(0),
		_inputSystem(0),
		_windowEventListener(0)
	{
		assert(!_instance && "Segunda inicializaci�n de BaseSubsystems::CServer no permitida!");

		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CServer

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "Segunda inicializaci�n de BaseSubsystems::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "BaseSubsystems::CServer no est� inicializado!");
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // release

	//--------------------------------------------------------

	bool CServer::open()
	{
		//PT: En el initCEGUI, antes de inicializar CEGUI, inicializar LUA (initLUA)
		if( !initOgre() ||
			!initOIS() ||
			!initLUA() ||
			!initCEGUI() )
		{			 
			srand (time(NULL));// FRS initialize random seed
			close();
			return false;
		}
		return true;	

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		releaseCEGUI();

		releaseOIS();
		
		releaseOgre();
		
	} // close

	//--------------------------------------------------------

	bool CServer::initOgre()
	{
		// Inicializamos el punto de acceso a Ogre con una definici�n
		// diferente de los plugins si estamos en release o en debug.
#ifdef _DEBUG
		_root = new Ogre::Root("plugins_d.cfg");
#else
		_root = new Ogre::Root("plugins.cfg");
#endif
		// Cargamos las rutas de los recursos desde el archivo de configuraci�n
		setupOgreResources();

		// Cargamos los ajustes de configuraci�n.
		if(!configureOgre())
			return false;
		
		// Inicializamos los recursos y el sistema de renderizado.
		if(!initOgreResources())
		{
			// La unicializaci�n puede haber fallado por cargar una
			// configuraci�n incorrecta, por haber actualizado hardware o
			// cambiado de m�quina. Mostramos la ventana de configuraci�n.
			if(!configureOgre(true)) {
				// Si el juego no se puede configurar se finaliza
				return false;
			}
			// Si tras actualizar la configuraci�n sigue sin poderse desistimos.
			if(!initOgreResources())
				return false;
		}
		return true;

	} // initOgre

	//--------------------------------------------------------

	bool CServer::initOIS()
	{
		// Cogemos el identificador y las dimensiones de la ventana
		// de renderizado para el gestor de perif�ricos de entrada.
		// Para saber el espacio de actuaci�n del rat�n.
		unsigned int width, height, windowHnd;
		getWindowExtents(width,height);
		if((width == -1) || (height == -1))
			return false;

		if((windowHnd = getWindowHnd()) == -1)
			return false;

		// Preparamos variables para la inicializaci�n del sistema
		// de entrada.
		OIS::ParamList paramList;
		std::ostringstream windowHndStr;

		windowHndStr << windowHnd;
		paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		
#if defined NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
		// Si estamos en modo ventana no queremos rat�n y teclado en modo exclusivo,
		// queremos que las dem�s aplicaciones puedan usarlo.
		if(!_renderWindow->isFullScreen())
		{
#if defined OIS_WIN32_PLATFORM 
			paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" ))); 
			paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE"))); 
			paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND"))); 
			paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE"))); 
#elif defined OIS_LINUX_PLATFORM 
			paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false"))); 
			paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false"))); 
			paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false"))); 
			paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true"))); 
#endif 
		}
#endif 

		// Creamos el sistema de entrada
		try
		{
			_inputSystem = OIS::InputManager::createInputSystem(paramList);
		}
		catch(std::exception e)
		{
			return false;
		}
		
		// Si es posible creamos el buffer del teclado.
		if (_inputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) 
		{
			_keyboard = static_cast<OIS::Keyboard*>(_inputSystem->createInputObject(OIS::OISKeyboard, true));
		}

		// Si es posible creamos el buffer del rat�n.
		if (_inputSystem->getNumberOfDevices(OIS::OISMouse) > 0) 
		{
			_mouse = static_cast<OIS::Mouse*>(_inputSystem->createInputObject(OIS::OISMouse, true));

			// Establecemos las dimensiones de la ventana de
			// renderizado para saber el espacio de actuaci�n 
			// del rat�n.
			setWindowExtents(width, height);
		}

		// Si es posible creamos el buffer del joystick.
		if (_inputSystem->getNumberOfDevices(OIS::OISJoyStick) > 0) 
		{
			_joystick = static_cast<OIS::JoyStick*>(_inputSystem->createInputObject(OIS::OISJoyStick, true));
		}


		return true;

	} // initOIS

	//--------------------------------------------------------

	bool CServer::initCEGUI()
	{
		CEGUI::OgreRenderer& CEGUIRenderer = CEGUI::OgreRenderer::create(*_renderWindow);

		//PT (Se cambia la inicializacion de CEGUI)
		//Indicaciones de http://mastervj.fdi.ucm.es/dokuwiki/doku.php?id=ayuda_proyectos:cegui)

		//Como en la web de ayuda a proyectos
		//Funciona
		//CEGUI::LuaScriptModule& luaModule = CEGUI::LuaScriptModule::create(ScriptManager::CServer::getSingletonPtr()->getLuaState());
		//CEGUI::System::create(CEGUIRenderer, NULL, NULL, NULL, &luaModule); 

		//Con LUA
		//No funciona teniendo un fichero de configuracion porque no estan cargados los recursos
		//_luaModule = &CEGUI::LuaScriptModule::create(ScriptManager::CServer::getSingletonPtr()->getLuaState());
	    //CEGUI::System::create(CEGUIRenderer, NULL, 0, NULL, _luaModule,"media/gui/configs/CEGUIConfig.xml");

		_luaModule = &CEGUI::LuaScriptModule::create(ScriptManager::CServer::getSingletonPtr()->getLuaState());
	    CEGUI::System::create(CEGUIRenderer, NULL, NULL, NULL, _luaModule);

	    //Con CEGUI
		//CEGUI::System::create(CEGUIRenderer);

		_GUISystem = CEGUI::System::getSingletonPtr();

		// Inicializamos los directorios necesarios para el Resource Provider,
		// as� cuando creemos un recurso no tenemos que dar una ruta completa.
		CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(
			CEGUI::System::getSingleton().getResourceProvider());

		//CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(_GUISystem->getResourceProvider());

		rp->setResourceGroupDirectory("fonts", "media/gui/fonts/");
		rp->setResourceGroupDirectory("imagesets", "media/gui/imagesets/");
		rp->setResourceGroupDirectory("layouts", "media/gui/layouts");
		rp->setResourceGroupDirectory("looknfeels", "media/gui/looknfeel/");
		rp->setResourceGroupDirectory("schemes", "media/gui/schemes/");

		// Definimos los grupos de recursos que usaremos
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");



		return true;

	} // initCEGUI

	//--------------------------------------------------------



	//--------------------------------------------------------
	//PT
	bool CServer::initLUA()
	{

		ScriptManager::CServer::getSingletonPtr()->Init();

		return true;

	} // initLUA


	void CServer::releaseOgre()
	{
		if(_renderWindow)
		{
			_renderWindow->removeAllViewports();
			_root->destroyRenderTarget(_renderWindow->getName());
			_renderWindow = 0;
		}

		destroyDebugRenderWindow();

		if(_windowEventListener)
		{
			delete _windowEventListener;
			_windowEventListener = 0;
		}

		if(_root)
		{
			delete _root;
			_root = 0;
		}

	} // releaseOgre

	//--------------------------------------------------------

	void CServer::releaseOIS()
	{
		if(_mouse) 
		{
			_inputSystem->destroyInputObject(_mouse);
			_mouse = 0;
		}

		if(_keyboard) 
		{
			_inputSystem->destroyInputObject(_keyboard);
			_keyboard = 0;
		}

		if(_joystick) 
		{
			_inputSystem->destroyInputObject(_joystick);
			_joystick = 0;
		}

		if(_inputSystem)
		{
			_inputSystem->destroyInputSystem(_inputSystem);
			_inputSystem = 0;
		}

	} // releaseOIS

	//--------------------------------------------------------

	void CServer::releaseCEGUI()
	{
		if(_GUISystem)
		{
			CEGUI::System::destroy();
			_GUISystem = 0;
		}

	} // releaseCEGUI

	//--------------------------------------------------------
	
	Ogre::Timer* CServer::getAppTimer() 
	{
		if(_root)
			return _root->getTimer();

		return 0;

	} // getAppTimer

	//--------------------------------------------------------

	void CServer::setupOgreResources()
	{
		//////////////////
		// OGRE         //
		//////////////////

		// Carga las rutas de los recursos desde el archivo de configuraci�n
		Ogre::ConfigFile cf;
		cf.load("resources.cfg");

		// Examina todos los ajustes en el archivo 
		Ogre::ConfigFile::SectionIterator itSection = cf.getSectionIterator();

		std::string sSection, sType, sArch;
		while(itSection.hasMoreElements()) 
		{
			sSection = itSection.peekNextKey();

			Ogre::ConfigFile::SettingsMultiMap *mapSettings = itSection.getNext();
			Ogre::ConfigFile::SettingsMultiMap::const_iterator itSetting = mapSettings->begin();
			while(itSetting != mapSettings->end()) 
			{
				sType = itSetting->first;
				sArch = itSetting->second;

				Ogre::ResourceGroupManager::getSingleton().
						addResourceLocation(sArch, sType, sSection);

				++itSetting;
			}
		}

	} // setupResources
	
	//--------------------------------------------------------

	bool CServer::configureOgre(bool force)
	{
		//////////////////
		// OGRE         //
		//////////////////

		// Carga los ajustes de configudaci�n desde ogre.cfg
		if(force || !_root->restoreConfig()) {
			// Si no hay archivo de configuraci�n se lanza la ventana de configuraci�n
			if(!_root->showConfigDialog()) {
				// Si el juego no se puede configurar se finaliza
				return false;
			}
		}
		return true;

	} // configure
	
	//--------------------------------------------------------

	bool CServer::initOgreResources()
	{
		//////////////////
		// OGRE         //
		//////////////////

		try
		{
			// Inicializa el sistema de renderizado y crea la ventanapor defecto. 
			// El true nos evita llamar a Root::createRenderWindow y se invoca 
			// con los par�metros actuales del sistema de reenderizado.
			_renderWindow = _root->initialise(true, _WINDOW_TITLE);
			
			
			// FRS Establecemos el *.ico de ventana y el cursor para modo windowed
#if _WIN32		

			HWND hwnd;
				_renderWindow->getCustomAttribute("WINDOW", &hwnd);
			HINSTANCE hInst = (HINSTANCE)GetModuleHandle(0);	
			
			SetClassLong( hwnd, GCL_HICON, 
				(LONG) LoadIcon(hInst, MAKEINTRESOURCE(IDI_BIG) ) );
			SetClassLong( hwnd, GCL_HICONSM, 
				(LONG) LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL) ) );
			SetClassLong( hwnd, GCL_HCURSOR, 
				(LONG) LoadCursor(hInst, MAKEINTRESOURCE (IDC_CURSOR)) );	
			
			// UNDONE FRS Mejor un menu interno con CEGUI que un menu de WIN
			// SetMenu(hwnd, LoadMenu(hInst, MAKEINTRESOURCE(IDC_ZTB) ));
			
			// UNDONE FRS Tambien se podr�a haber usado WM_SETICON
			// La diferencia reside en que SetClassLong establece el ID para todas las instancias de la clase Hwnd
			// Y SendMessage tan s�lo cambia en un momento dado la instancia concreta hwnd.
			//	SendMessage(hwnd, WM_SETICON, ICON_BIG,   LPARAM(iconBig));
			//	SendMessage(hwnd, WM_SETICON, ICON_SMALL, LPARAM(iconSmall));
#endif

			// A�adimos un listener que gestiona el evento de cierre de la ventana.
			_windowEventListener = new WindowEventListener();
			Ogre::WindowEventUtilities::addWindowEventListener(_renderWindow,_windowEventListener);

#if defined RENDER_IN_BACKGROUND
			// Si la ventana pasa a background no queremos que se pierda el foco
			// para que la ventana se siga renderizando.
			_renderWindow->setDeactivateOnFocusChange(false);
#endif
			// Inicializa los recursos que deben haber sido cargados en setupResources()
			Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup("General"); // FRS Solo se inicializa el grupo [General] ?
		}
		catch(Ogre::Exception e)
		{
			return false;
		}

		return true;

	} // initialise

	//--------------------------------------------------------

	void CServer::getWindowExtents(unsigned int &width, unsigned int &height) 
	{
		assert(_renderWindow && "�No hay ventana de renderizado!");
		if(_renderWindow)
		{
			width = _renderWindow->getWidth();
			height = _renderWindow->getHeight(); 
		}
		else
			width = height = -1;

	} // getWindowExtents


	//--------------------------------------------------------


	bool CServer::isWindowedMode() 
	{ 
		assert(_renderWindow && "�No hay ventana de renderizado!"); 
		return _renderWindow? !_renderWindow->isFullScreen() : true;
	} 


	//--------------------------------------------------------

	const unsigned int CServer::getWindowHnd() 
	{
		assert(_renderWindow && "�No hay ventana de renderizado!");
		if(_renderWindow)
		{
			unsigned int windowHnd;
			_renderWindow->getCustomAttribute("WINDOW", &windowHnd);
			return windowHnd;
		}
		return -1;

	} // getWindowHnd

	//--------------------------------------------------------

	void CServer::setWindowExtents(unsigned int width, unsigned int height) 
	{
		/////////////
		// Ogre    //
		/////////////

		// Redimensionamos y cambiamos la resoluci�n de la 
		// pantalla.
		bool fullScreen = _renderWindow->isFullScreen();
		_renderWindow->setFullscreen(fullScreen,width,height);

		/////////////
		// OIS     //
		/////////////

		// Actualizamos el estado del rat�n de OIS
		const OIS::MouseState &mouseStateOIS = _mouse->getMouseState();
		mouseStateOIS.width  = width;
		mouseStateOIS.height = height;

	} // setWindowExtents

	//--------------------------------------------------------

	void CServer::createDebugRenderWindow()
	{
		//Si no existe Crear un debug renderWindow de tama�o 1/2 el original
		if (!_debugRenderWindow)
		{
			try {
			_debugRenderWindow = _root->createRenderWindow(_WINDOW_TITLE + "- DEBUG", 640,
									480, false);
			_debugRenderWindow->setDeactivateOnFocusChange(false);
			}
			catch(Ogre::Exception e)
			{
			}
		}
	}

	//--------------------------------------------------------

	void CServer::destroyDebugRenderWindow()
	{
		if (_debugRenderWindow)
		{
			//Bug en Ogre-> si no se hace as� a trav�s del RenderSistem espec�fico peta:
			//http://www.ogre3d.org/forums/viewtopic.php?f=22&t=69648
			_root->getRenderSystemByName("Direct3D9 Rendering Subsystem")->destroyRenderTarget(_debugRenderWindow->getName());
			_debugRenderWindow = 0;
		}
	}

} // namespace BaseSubsystems
