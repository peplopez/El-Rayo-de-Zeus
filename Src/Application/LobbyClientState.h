//---------------------------------------------------------------------------
// LobbyClientState.h
//---------------------------------------------------------------------------

/**
@file LobbyClientState.h

Contiene la declaración del estado de lobby del cliente.

@see Application::CApplicationState
@see Application::CLobbyClientState

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Application_LobbyClientState_H
#define __Application_LobbyClientState_H

#include "ApplicationState.h"
#include "NET/IObserver.h"

//PT
#include <cegui.h> //para que pueda crear combobox

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
}

//PT
namespace ScriptManager
{
	class Server;
}

namespace Application 
{
	/**
	Como su nombre indica, esta clase es la clase del menú
	principal del juego. Es muy sencilla y lo único que hace es cargar
	un layout de CEGUI al inicio y activarlo y desactivarlo cuando
	se activa o desactiva el estado (haciéndo visible/invisible también
	el puntero del ratón). También asocia los eventos de los botones 
	del menú a las funciones C++ que se deben invocar cuando los botones
	son pulsados.
	<p>
	Este estado es CEGUI dependiente, lo cual no es deseable, la aplicación
	debería ser independiente de las tecnologías usadas.

	@ingroup applicationGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CLobbyClientState : public CApplicationState, public Net::IObserver
	{

	

	public:
		/** 
		Constructor de la clase 
		*/
		CLobbyClientState(CBaseApplication *app) : CApplicationState(app) {}

		/** 
		Destructor 
		*/
		virtual ~CLobbyClientState() {}

		/**
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		/**
		Función llamada cuando se elimina ("desengancha") el
		estado de la aplicación.
		*/
		virtual void release();

		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		virtual void activate();

		/**
		Función llamada por la aplicación cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		/**
		Función llamada por la aplicación para que se ejecute
		la funcionalidad del estado.

		@param msecs Número de milisegundos transcurridos desde
		la última llamada (o desde la áctivación del estado, en caso
		de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);

		// Métodos de CKeyboardListener
		
		/**
		Método que será invocado siempre que se pulse una tecla. 
		Será la aplicación quién llame a este método cuando el 
		estado esté activo. Esta clase NO se registra en el 
		InputManager sino que es la aplicación quien lo hace y 
		delega en los estados.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyPressed(GUI::TKey key);
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla. Será la aplicación quién llame a este método 
		cuando el estado esté activo. Esta clase NO se registra en
		el InputManager sino que es la aplicación quien lo hace y 
		delega en los estados.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyReleased(GUI::TKey key);

		// Métodos de CMouseListener
		
		/**
		Método que será invocado siempre que se mueva el ratón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mouseMoved(const GUI::CMouseState &mouseState);
		
		/**
		Método que será invocado siempre que se pulse un botón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mousePressed(const GUI::CMouseState &mouseState);

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón. La aplicación avisa de este evento al estado 
		actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. 
		*/
		virtual bool mouseReleased(const GUI::CMouseState &mouseState);


		/******************
			NET::IOBSERVER
		******************/
		virtual void dataPacketReceived(Net::CPacket* packet);	
		virtual void connectPacketReceived(Net::CPacket* packet) {};
		virtual void disconnectPacketReceived(Net::CPacket* packet){};

	private:

		CEGUI::Window * _windowStatus;
		CEGUI::Window * _windowConnect;
		CEGUI::Window * _windowBack;

		//PT combobox
		CEGUI::Combobox* _cbModel;
		CEGUI::Combobox* _cbColor;

		typedef std::list<std::string> TMapNameList;
		TMapNameList _mapsToLoad;
		
		/**
		Función que se quiere realizar cuando se pulse el botón start.
		Simplemente cambia al estado de juego.
		*/
		bool _connectReleased(const CEGUI::EventArgs& e);

		/**
		Función que se quiere realizar cuando se pulse el botón back.
		Simplemente cambia al estado de menu.
		*/
		bool _backReleased(const CEGUI::EventArgs& e);


		/**
		* Función que ejecuta la acción start. 
		Centraliza el código y será invocada cuando se pulse la tecla correspondiente o se
		genere el evento de ratón
		*/
		void _connect();

	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H
