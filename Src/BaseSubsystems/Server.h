//---------------------------------------------------------------------------
// Server.h
//---------------------------------------------------------------------------

/**
@file Server.h

Contiene la declaración de la clase principal que engloba los subsistemas
usados. La mayoría de ellos son parte de Ogre.

@see BaseSubsystems::CServer

@author David Llansó
@date Julio, 2010
*/

#ifndef __BaseSubsystems_Server_H
#define __BaseSubsystems_Server_H

/**
Si se define la siguiente directiva, en modo ventana el juego no toma ratón
y teclado como como de uso exclusivo sino que se sigue viendo el del OS y se
pueden usar en otras ventanas
*/
//#define NON_EXCLUSIVE_MODE_IN_WINDOW_MODE

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Root;
	class Timer;
	class RenderWindow;
}

namespace OIS
{
	class InputManager;
	class Mouse;
	class Keyboard;
}

namespace CEGUI
{
	class System;
}

namespace BaseSubsystems
{
	class WindowEventListener;
}

/**
Namespace con todo lo que rodea la aplicación. Engloba los sistemas 
externos en los que se apoya la aplicación, o clases que los inicializan.
(para más información ver @ref baseSubsystemsGroup).

@author David Llansó
@date Julio, 2010
*/
namespace BaseSubsystems 
{
	/**
	Clase central que se encarga de inicializar y gestionar los diferentes
	subsistemas necesarios para correr la aplicación. Su misión principal es 
	la de inicializar los subsistemas de Ogre, OIS, etc. y hacerlos accesibles 
	al resto de proyectos de la solución.
	<p>
	En esta clase es donde se ocultan los detalles engorrosos de la creación
	y destrucción de los recursos necesarios para que luego el resto de 
	proyectos de la solución puedan acceder a ellos de forma fácil sin
	preocuparse.
	<p>
	De la parte de Ogre se encarga de la carga de los diferentes plugins, de
	la carga de los resursos, de la inicialización del sistema de render y de
	la creación de la ventana de renderizado. Ofrece métodos accesores a las
	diferentes instancias y modificadores como el del tamaño de la pantalla.
	También da acceso al temporizador de Ogre, usado por la aplicación para 
	gestionar los tiempos del juego.
	<p>
	En la parte de OIS crea el sistema de gestión de periféricos de entrada,
	un buffer para el teclado y otro para el ratón. Ofrece métodos accesores a 
	las diferentes instancias y modificadores como el del tamaño de la pantalla,
	que pone el rango de actuación del ratón.
	<p>
	Está implementado como un Singleton; se debe llamar a su método Init(), para
	que la instancia esté creada. Al final de la aplicación, habrá que llamar
	a Release().

	@author David Llansó
	@date Julio, 2010
	@ingroup baseSubsystemsGroup
	*/
	class CServer 
	{
	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al gestor de subsistemas.
		*/
		static BaseSubsystems::CServer *getSingletonPtr() { return _instance; }
		/**
		Inicializa los diferentes subsistemas.

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el gestor de subsistemas. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Devuelve el temporizador de la aplicación.

		@return Temporizados de la aplicación.
		*/
		Ogre::Timer* getAppTimer();

		/**
		Devuelve el punto de entrada a Ogre.

		@return Puntero al punto de entrada a Ogre.
		*/
		Ogre::Root *getOgreRoot() {return _root;}

		/**
		Devuelve la ventana de reenderizado.

		@return Puntero a la actual ventana de reenderizado.
		*/
		Ogre::RenderWindow *getRenderWindow() {return _renderWindow;}

		/**
		Devuelve propiedades de la ventana de reenderizado.

		@param width Ancho de la ventana de reenderizado en pixels.
		Si no hay ventana devuelve -1.
		@param height Alto de la ventana de reenderizado en pixels.
		Si no hay ventana devuelve -1.
		*/
		void getWindowExtents(unsigned int &width, unsigned int &height);
		
		/**
		Devuelve el identificador de la ventana de reenderizado sobre
		la que estamos trabajando.

		@return Identificador de la ventana de reenderizado sobre la
		que estamos trabajando. Si no hay ventana devuelve -1.
		*/
		const unsigned int getWindowHnd();

		/**
		Devuelve el sistema de gestión de periféricos de entrada de 
		OIS.

		@return Puntero al sistema de gestión de periféricos de 
		entrada de OIS.
		*/
		OIS::InputManager *getInputSystem() {return _inputSystem;}

		/**
		Devuelve la instancia OIS del buffer del ratón.

		@return Instancia del buffer del ratón.
		*/
		OIS::Mouse* getBufferedMouse(){return _mouse;}
		
		/**
		Devuelve la instancia OIS del buffer del teclado.

		@return Instancia del buffer del teclado.
		*/
		OIS::Keyboard* getBufferedKeyboard(){return _keyboard;}

		/**
		Devuelve el sistema de la interfaz gráfica de usuario CEGUI.

		@return Puntero al sistema de la interfaz gráfica de usuario 
		CEGUI.
		*/
		CEGUI::System *getGUISystem() {return _GUISystem;}

	protected:

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la aplicación.
		*/
		virtual ~CServer();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();

		/**
		Inicialización de Ogre.

		@return true si todo fue correctamente.
		*/
		bool initOgre();

		/**
		Inicialización de OIS.

		@return true si todo fue correctamente.
		*/
		bool initOIS();

		/**
		Inicialización de CEGUI.

		@return true si todo fue correctamente.
		*/
		bool initCEGUI();

		/**
		Liberación de Ogre.
		*/
		void releaseOgre();

		/**
		Liberación de Ogre.
		*/
		void releaseOIS();

		/**
		Liberación de CEGUI.
		*/
		void releaseCEGUI();

		/**
		Método que carga los recursos Ogre especificados en el fichero de 
		configuración.
		*/
		void setupOgreResources();

		/**
		Carga los ajustes de configudación de Ogre. Si estos no existen, o 
		se fuerza a ello, se lanza una ventana de configuración

		@param force Indica si se quiere mostrar la pantalla de configuración sin
		intentar cargar los valores de disco.
		@return true si de una u otra forma se pudieron cargar los ajustes.
		*/
		bool configureOgre(bool force = false);

		/**
		Una vez cargados los recursos del archivo de configuración de Ogre se inicializa 
		un grupo de recursos "general". Posteriormente podrían inicializarse nuevos 
		grupos de recursos para otros propósitos, como por ejemplo uno por nivel.
		Además se encarga de inicializar el sistema de render y crear una
		ventana de renderizado. Antes de llamar a este método debe haberse 
		seleccionado un sistema de render mediante Root::setRenderSystem o mediante
		el método configureOgre().

		@return true si todo fue bien.
		*/
		bool initOgreResources();

		/** 
		Pone las dimensiones máximas de la pantalla en las propiedades del ratón
		OIS. Si se redimendiona la ventana de reenderizado deberemos llamar a este 
		método.

		@param width Anchura de la ventana en pixels.
		@param height Altura de la ventana en pixels.
		*/
		void setWindowExtents(unsigned int width, unsigned int height);

		/**
		Instancia única de la aplicación.
		*/
		static CServer *_instance;
		
		/**
		Punto de entrada al sistema Ogre.
		*/
		Ogre::Root *_root;

		/** 
		Ventana de renderizado 
		*/
		Ogre::RenderWindow *_renderWindow;

		/**
		Clase que escucha los eventos de ventana. Capturamos el evento de
		cierre para forzar que la aplicación se cierre.
		*/
		WindowEventListener* _windowEventListener;

		/**
		Sistema de gestión de periféricos de entrada de OIS.
		*/
		OIS::InputManager *_inputSystem;

		/** 
		Buffer de la entrada del ratón OIS.
		*/
		OIS::Mouse *_mouse;

		/** 
		Buffer de la entrada del teclado OIS.
		*/
		OIS::Keyboard *_keyboard;

		/**
		Sistema de la interfaz gráfica de usuario CEGUI.
		*/
		CEGUI::System *_GUISystem;

	}; // class CServer

} // namespace BaseSubsystems

#endif // __BaseSubsystems_Server_H
