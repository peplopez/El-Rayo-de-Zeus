/**
@file Server.h

Contiene la declaración de la clase CServer, Singleton que se encarga de
la gestión de la interfaz con el usuario (entrada de periféricos, CEGui...).

@see GUI::CServer

@author David Llansó
@date Agosto, 2010
*/
#ifndef __GUI_Server_H
#define __GUI_Server_H

#include "InputManager.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CAvatarController;
}

namespace GUI 
{
	class CPlayerController;
	class CCameraController;
	class CHudController; //PT
}

namespace CEGUI
{
	class System;
	class Window; //PT
}

// Declaración de la clase
namespace GUI
{
	/**
	Servidor del módulo GUI que hace de interfaz con el usuario. Está 
	implementado como un singlenton de inicialización explícita. Sirve 
	para comunicar a CEGUI los eventos producidos ya así hacer funcionar
	la interfaz gráfica de usuario y permite que se establezca que entidad 
	es controlada por el GUI::CPlayerController. Este control se hace a 
	través del componente Logic::CAvatarController.

	@ingroup GUIGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CServer : public CKeyboardListener, public CMouseListener, public CJoystickListener
	{
	public:

		/**
		Devuelve la única instancia de la clase CServer.
		
		@return Única instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Devuelve la instancia de la clase GUI que se encarga de procesar los
		eventos de entrada para controlar al jugador por si se desea configurar
		externemante.

		@return Instancia de la clase GUI que controla al jugador.
		*/
		CPlayerController *getPlayerController() {return _playerController;}

		/** PEP
		Devuelve la instancia de la clase GUI que se encarga de procesar los
		eventos de entrada para controlar al jugador por si se desea configurar
		externemante.

		@return Instancia de la clase GUI que controla al jugador.
		*/
		CCameraController *getCameraController() {return _cameraController;}

		/** PT
		Devuelve la instancia de la clase GUI que se encarga de procesar los
		eventos de entrada para recargar el HUD

		@return Instancia de la clase GUI que recarga el HUD.
		*/
		CHudController *getHudController() {return _hudController;}


		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se pulse una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyPressed(TKey key);
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyReleased(TKey key);
		
		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se mueva el ratón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mouseMoved(const CMouseState &mouseState);
		
		/**
		Método que será invocado siempre que se pulse un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState);

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState);

	/**
	 * Show a text on the GUI screen.
	 *
	 * @param msg Message that is going to be shown on screen.
	 */
		void setText(const std::string& msg);

	protected:

		/**
		Constructor.
		*/
		CServer ();

		/**
		Destructor.
		*/
		~CServer();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();

		/**
		Clase GUI que se encarga de controlar al jugador.
		*/
		CPlayerController *_playerController;

		/**
		Clase GUI que se encarga de controlar la cámara.
		*/
		CCameraController *_cameraController;

		//PT
		/**
		Clase GUI que se encarga de controlar el HUD.
		*/
		CHudController *_hudController;


		/**
		Sistema de la interfaz gráfica de usuario CEGUI.
		*/
		CEGUI::System *_GUISystem;

	/**
	* Puntero a la ventana CEGUI que esta actualmente siendo renderizada.
	* Si no se esta renderizando ninguna interfaz de usuario es NULL
	*/
		CEGUI::Window *_currentWindow;

	private:
		/**
		Única instancia de la clase.
		*/
		static CServer* _instance;

	}; // class CServer

} // namespace GUI

#endif // __GUI_Server_H
