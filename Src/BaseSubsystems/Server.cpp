//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

/**
@file Server.cpp

Contiene la implementación de la clase principal que engloba los subsistemas
usados. La mayoría de ellos son parte de Ogre.

@see BaseSubsystems::CServer

@author David Llansó
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

// CEGUI
#include <CEGUISystem.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIFont.h>
#include <CEGUIImageset.h>
#include <CEGUIWindowManager.h>
#include <falagard/CEGUIFalWidgetLookManager.h>
#include <CEGUIScheme.h>

// Para cerrar la aplicación si se cierra la ventana
#include "Application/BaseApplication.h"

/**
Si se define la siguiente directiva, en modo ventana se reenderiza aunque
la ventana esté en background (siempre que esté visible)
*/
#define RENDER_IN_BACKGROUND

namespace BaseSubsystems 
{

	/**
	Clase auxiliar que hereda de Ogre y que sirve para modificar el 
	comportamiento habitual de cierre de ventana. No queremos cerrarla
	desde aquí, queremos que sea la aplicación.
	*/
	class WindowEventListener : public Ogre::WindowEventListener
	{
	private:

	public:
		WindowEventListener()
		{
		}
		
		/**
		En vez de dejar que el método de salida habitual funcione 
		lo reescribimos para que se le comunique a la aplicación.
		Al devolver false la ventana no se cierra, lo haremos desde
		la aplicación.
		<p>
		Este método es llamado al solicitar el cierre de la ventana
		tanto con Alt-F4 como al pulsar la 'x' de la ventana.

		@param renderWindow Ventana sobre la que se produce el evento.
		@return Siempre falso, así no se realiza ninguna acción.
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
		_mouse(0),
		_keyboard(0),
		_inputSystem(0),
		_windowEventListener(0)
	{
		assert(!_instance && "Segunda inicialización de BaseSubsystems::CServer no permitida!");

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
		assert(!_instance && "Segunda inicialización de BaseSubsystems::CServer no permitida!");

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
		assert(_instance && "BaseSubsystems::CServer no está inicializado!");
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // release

	//--------------------------------------------------------

	bool CServer::open()
	{
		if( !initOgre() ||
			!initOIS() ||
			!initCEGUI() )
		{
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
		// Inicializamos el punto de acceso a Ogre con una definición
		// diferente de los plugins si estamos en release o en debug.
#ifdef _DEBUG
		_root = new Ogre::Root("plugins_d.cfg");
#else
		_root = new Ogre::Root("plugins.cfg");
#endif
		// Cargamos las rutas de los recursos desde el archivo de configuración
		setupOgreResources();

		// Cargamos los ajustes de configuración.
		if(!configureOgre())
			return false;
		
		// Inicializamos los recursos y el sistema de renderizado.
		if(!initOgreResources())
		{
			// La unicialización puede haber fallado por cargar una
			// configuración incorrecta, por haber actualizado hardware o
			// cambiado de máquina. Mostramos la ventana de configuración.
			if(!configureOgre(true)) {
				// Si el juego no se puede configurar se finaliza
				return false;
			}
			// Si tras actualizar la configuración sigue sin poderse desistimos.
			if(!initOgreResources())
				return false;
		}
		return true;

	} // initOgre

	//--------------------------------------------------------

	bool CServer::initOIS()
	{
		// Cogemos el identificador y las dimensiones de la ventana
		// de renderizado para el gestor de periféricos de entrada.
		// Para saber el espacio de actuación del ratón.
		unsigned int width, height, windowHnd;
		getWindowExtents(width,height);
		if((width == -1) || (height == -1))
			return false;

		if((windowHnd = getWindowHnd()) == -1)
			return false;

		// Preparamos variables para la inicialización del sistema
		// de entrada.
		OIS::ParamList paramList;
		std::ostringstream windowHndStr;

		windowHndStr << windowHnd;
		paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		
#if defined NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
		// Si estamos en modo ventana no queremos ratón y teclado en modo exclusivo,
		// queremos que las demás aplicaciones puedan usarlo.
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

		// Si es posible creamos el buffer del ratón.
		if (_inputSystem->getNumberOfDevices(OIS::OISMouse) > 0) 
		{
			_mouse = static_cast<OIS::Mouse*>(_inputSystem->createInputObject(OIS::OISMouse, true));

			// Establecemos las dimensiones de la ventana de
			// renderizado para saber el espacio de actuación 
			// del ratón.
			setWindowExtents(width, height);
		}

		return true;

	} // initOIS

	//--------------------------------------------------------

	bool CServer::initCEGUI()
	{
		CEGUI::OgreRenderer& CEGUIRenderer =
				 CEGUI::OgreRenderer::create(*_renderWindow);

		CEGUI::System::create(CEGUIRenderer);

		_GUISystem = CEGUI::System::getSingletonPtr();

		// Inicializamos los directorios necesarios para el Resource Provider,
		// así cuando creemos un recurso no tenemos que dar una ruta completa.
		CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(
			CEGUI::System::getSingleton().getResourceProvider());

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

	void CServer::releaseOgre()
	{
		if(_renderWindow)
		{
			_renderWindow->removeAllViewports();
			_root->detachRenderTarget(_renderWindow);
			delete _renderWindow;
			_renderWindow = 0;
		}

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

		// Carga las rutas de los recursos desde el archivo de configuración
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

		// Carga los ajustes de configudación desde ogre.cfg
		if(force || !_root->restoreConfig()) {
			// Si no hay archivo de configuración se lanza la ventana de configuración
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
			// con los parámetros actuales del sistema de reenderizado.
			_renderWindow = _root->initialise(true,"Galeon");

			// Añadimos un listener que gestiona el evento de cierre de la ventana.
			_windowEventListener = new WindowEventListener();
			Ogre::WindowEventUtilities::addWindowEventListener(_renderWindow,_windowEventListener);

#if defined RENDER_IN_BACKGROUND
			// Si la ventana pasa a background no queremos que se pierda el foco
			// para que la ventana se siga renderizando.
			_renderWindow->setDeactivateOnFocusChange(false);
#endif

			// Inicializa los recursos que deben haber sido cargados en
			// setupResources()
			Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup("General");
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
		assert(_renderWindow && "¡No hay ventana de renderizado!");
		if(_renderWindow)
		{
			width = _renderWindow->getWidth();
			height = _renderWindow->getHeight();
		}
		else
			width = height = -1;

	} // getWindowExtents

	//--------------------------------------------------------

	const unsigned int CServer::getWindowHnd() 
	{
		assert(_renderWindow && "¡No hay ventana de renderizado!");
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

		// Redimensionamos y cambiamos la resolución de la 
		// pantalla.
		bool fullScreen = _renderWindow->isFullScreen();
		_renderWindow->setFullscreen(fullScreen,width,height);

		/////////////
		// OIS     //
		/////////////

		// Actualizamos el estado del ratón de OIS
		const OIS::MouseState &mouseStateOIS = _mouse->getMouseState();
		mouseStateOIS.width  = width;
		mouseStateOIS.height = height;

	} // setWindowExtents

} // namespace BaseSubsystems
