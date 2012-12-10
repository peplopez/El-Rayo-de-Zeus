//---------------------------------------------------------------------------
// ApplicationState.h
//---------------------------------------------------------------------------

/**
@file ApplicationState.h

Contiene la declaración de la clase que representa un estado de la
aplicacion.

@see Application::CApplicationState
@see Application::CBaseApplication

@author Marco Antonio Gómez Martín & David Llansó
@date Julio, 2010
*/

#ifndef __Application_ApplicationState_H
#define __Application_ApplicationState_H

#include "BaseApplication.h"
#include "GUI/InputManager.h"

namespace Application 
{
	/**
	Clase CApplicationState; representa un estado de la 
	aplicación, como el estado del menú, o el estado jugando,
	etc.
	Distintos estados estarán implementados como clases hijas
	de esta.
	
	@ingroup applicationGroup

	@author Marco Antonio Gómez Martín & David Llansó
	@date Julio, 2010
	*/
	class CApplicationState : public GUI::CKeyboardListener, 
							  public GUI::CMouseListener
	{
	public:
		/** 
		Constructor de la clase 

		@param app Aplicacion a la que pertenece el estado.
		*/
		CApplicationState(CBaseApplication *app) : _app(app) {};

		/** 
		Destructor 
		*/
		virtual ~CApplicationState() {}

		/**
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init() {return true;}

		/**
		Función llamada cuando se elimina ("desengancha") el
		estado de la aplicación.
		*/
		virtual void release() {}

		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		virtual void activate() {}

		/**
		Función llamada por la aplicación cuando se desactiva
		el estado.
		*/
		virtual void deactivate() {}

		/**
		Función llamada por la aplicación para que se ejecute
		la funcionalidad del estado.

		@param msecs Número de milisegundos transcurridos desde
		la última llamada (o desde la áctivación del estado, en caso
		de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs) {}

		// Métodos de CKeyboardListener
		
		/**
		Método que será invocado siempre que se pulse una tecla. 
		Será la aplicación quién llame a este método cuando el 
		estado esté activo. Esta clase NO se registra en el 
		InputManager sino que es la aplicación quien lo hace y 
		delega en los estados. Por defecto no se capturan 
		pulsaciones.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyPressed(GUI::TKey key) {return false;}
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla. Será la aplicación quién llame a este método 
		cuando el estado esté activo. Esta clase NO se registra en
		el InputManager sino que es la aplicación quien lo hace y 
		delega en los estados. Por defecto no se capturan fin de
		pulsaciones.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyReleased(GUI::TKey key) {return false;}

		// Métodos de CMouseListener
		
		/**
		Método que será invocado siempre que se mueva el ratón. La
		aplicación avisa de este evento al estado actual. Por defecto 
		no se capturan movimiento del ratón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mouseMoved(const GUI::CMouseState &mouseState)
														{return false;}
		
		/**
		Método que será invocado siempre que se pulse un botón. La
		aplicación avisa de este evento al estado actual.Por defecto 
		no se capturan pulsaciones.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mousePressed(const GUI::CMouseState &mouseState)
														{return false;}

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón. La aplicación avisa de este evento al estado 
		actual.Por defecto no se capturan fin de pulsaciones.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. 
		*/
		virtual bool mouseReleased(const GUI::CMouseState &mouseState)
														{return false;}

	protected:
		/**
		Puntero a la aplicación a la que pertenece el estado
		*/
		CBaseApplication *_app;

	}; // CApplicationState

} // namespace Application

#endif //  __Application_ApplicationState_H
