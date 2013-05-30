//---------------------------------------------------------------------------
// MenuSingleState.h
//---------------------------------------------------------------------------

/**
@file MenuSingleState.h

Contiene la declaración del estado de menú de Single Player (con las opciones para el Jugador).

@see Application::CApplicationState
@see Application::CMenuSingleState

@author Pablo Terrado
@date Mayo, 2013
*/

#ifndef __Application_MenuSingleState_H
#define __Application_MenuSingleState_H

#include "ApplicationState.h"


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

namespace ScriptManager
{
	class Server;
}

//PT
namespace Ogre
{
	class Root;
}

namespace Application 
{
	/**
	Como su nombre indica, esta clase es la clase del menú Single Player del juego. 
	Es muy sencilla ysirve para que el jugador decida su nickname, modelo de jugador, y color.

	@ingroup applicationGroup

	@author Pablo Terrado
	@date Mayo, 2013
	*/
	class CMenuSingleState : public CApplicationState 
	{

	protected:

		typedef std::list<std::string> TMapNameList;
	
	public:
		/** 
		Constructor de la clase 
		*/
		CMenuSingleState(CBaseApplication *app) : CApplicationState(app)
				{}

		/** 
		Destructor 
		*/
		virtual ~CMenuSingleState();

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

	private:

		/**
		Ventana CEGUI que muestra el menú.
		*/
		CEGUI::Window* _menuWindow;

		//PT combobox
		CEGUI::Combobox* _cbModel;
		CEGUI::Combobox* _cbColor;

		//PT
		//CEGUI::ProgressBar* _hbar;

		/**
		*/
		TMapNameList _mapsToLoad;

	
		/**
		Función que se quiere realizar cuando se pulse el botón start.
		Simplemente cambia al estado de juego.
		*/
		bool startReleased(const CEGUI::EventArgs& e);

		/**
		Función que se quiere realizar cuando se pulse el botón exit.
		Simplemente termina la aplicación.
		*/
		bool backReleased(const CEGUI::EventArgs& e);

		/**
		Función que se quiere realizar cuando el progreso avance/actualice
		para cambiar la barra de progreso
		*/
		bool CMenuSingleState::onProgressChanged(const CEGUI::EventArgs &e);

	}; // CMenuSingleState

} // namespace Application

#endif //  __Application_MenuSingleState_H
