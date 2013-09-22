//---------------------------------------------------------------------------
// LobbyServerState.h
//---------------------------------------------------------------------------

/**
@file LobbyServerState.h

Contiene la declaración del estado de lobby del servidor.

@see Application::CApplicationState
@see Application::CLobbyServerState

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Application_LobbyServerState_H
#define __Application_LobbyServerState_H

#include "ApplicationState.h"

#include <list>
#include <map>

#include "Net/Manager.h"
#include "NET/IObserver.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
	class WindowManager;
}

namespace Net
{
	class CManager;
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
	class CLobbyServerState : public CApplicationState , public Net::IObserver
	{
	public:
		/** 
		Constructor de la clase 
		*/
		CLobbyServerState(CBaseApplication *app) : CApplicationState(app),
			_waiting(true), _nClients(0), _maskClientIds(0), _mapLoadedByClients(0) {} 	

		/**
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		bool init();

		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		void activate();

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
		virtual bool keyReleased(GUI::TKey key);




		/******************
			NET::IOBSERVER
		******************/
		void dataPacketReceived(Net::CPacket* packet);
		void connectPacketReceived(Net::CPacket* packet);
		void disconnectPacketReceived(Net::CPacket* packet);


	private:

		/**
		Indica si estamos en fase de espera de jugadores
		*/
		bool _waiting;
			
		/**
		* Función que ejecuta la acción start. 
		Centraliza el código y será invocada cuando se pulse la tecla correspondiente o se
		genere el evento de ratón
		*/
		void _start();

				
		//-------- NET ----------------------------

		// PLAYER INFO
		unsigned int _nClients;
		std::string _playerNicks[8]; // HACK fijamos num max de players a 8
		std::string _playerModels[8]; // TODO deberían ser enum para menos info por red en un futuro

		Net::CManager* _netManager;

		//typedef std::list<Net::NetID> TNetIDList; UNDONE no es necesario una lista
		typedef unsigned char TMask;

		/**
		lista donde almacenamos los clientes conectados
		*/
		//TNetIDList _maskClientIds; UNDONE
		TMask _maskClientIds; // char máscara

		/**
		lista donde almacenamos los clientes que han cargado el mapa
		*/
		//TNetIDList _mapLoadedByClients; UNDONE
		TMask _mapLoadedByClients;

		// UNDONE
		//typedef std::map<Net::NetID,unsigned int> TNetIDCounterMap;
		//typedef std::pair<Net::NetID,unsigned int> TNetIDCounterPair;

		/**
		Tabla donde almacenamos por cada cliente cuantos jugadores han creado
		*/
		//TNetIDCounterMap _playersLoadedByClients; UNDONE
		unsigned int _playersLoadedByClients[8];


		
		//-------- CEGUI ----------------------

		CEGUI::WindowManager* _windowManager;
		CEGUI::Window* _windowMenu;	
		CEGUI::Window* _windowStatus;
		CEGUI::Window* _windowStart;
		CEGUI::Window* _windowBack;

		void _logStatus(const std::string& statusMsg);
		/**
		Función que se quiere realizar cuando se pulse el botón start.
		Simplemente cambia al estado de juego.
		*/
		bool _startReleased(const CEGUI::EventArgs& e);

		/**
		Función que se quiere realizar cuando se pulse el botón back.
		Simplemente cambia al estado de menu.
		*/
		bool _backReleased(const CEGUI::EventArgs& e);




	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H
