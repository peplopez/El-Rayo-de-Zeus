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

namespace Net
{
	class CManager;
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
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

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
		

		// Métodos de CKeyboardListener

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
		bool keyReleased(GUI::TKey key);


		/******************
			NET::IOBSERVER
		******************/
		void dataPacketReceived(Net::CPacket* packet);	
		void connectPacketReceived(Net::CPacket* packet){}
		void disconnectPacketReceived(Net::CPacket* packet){}


	private:
		
		/**
		* Función que ejecuta la acción start. 
		Centraliza el código y será invocada cuando se pulse la tecla correspondiente o se
		genere el evento de ratón
		*/
		void _connect();
		
		Net::CManager* _netManager;

		


		//-------- CEGUI ------------------

		CEGUI::WindowManager* _windowManager;
		CEGUI::Window * _windowStatus;
		CEGUI::Window * _windowConnect;	
		
		//PT combobox
		CEGUI::Combobox* _cbModel;
		CEGUI::Combobox* _cbColor;

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

		void _logStatus(const std::string& statusMsg);

	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H
