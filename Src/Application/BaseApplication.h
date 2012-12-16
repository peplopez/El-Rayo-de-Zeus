//---------------------------------------------------------------------------
// BaseApplication.h
//---------------------------------------------------------------------------

/**
@file BaseApplication.h

Contiene la declaración de la clase aplicacion, que maneja la ejecución
de todo el juego.

@see Application::CBaseApplication
@see Application::CApplicationState

@author Marco Antonio Gómez Martín & David Llansó
@date Julio, 2010
*/

#ifndef __Application_BaseApplication_H
#define __Application_BaseApplication_H

#include "GUI/InputManager.h"

#include <map>
#include <string>
#include <cassert>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Application 
{
  class CApplicationState;
  class IClock;
}

/**
Namespace con todo lo relacionado con la aplicación: clase abstracta, 
estados, etc. Es el corazón del juego, el encargado de inicializar
el resto de sistemas necesarios para la ejecución del juego.
(para más información ver @ref applicationGroup).

@author Marco Antonio Gómez Martín & David Llansó
@date Julio, 2010
*/
namespace Application 
{
	/**
	Clase CBaseApplication, que contiene el control de la aplicación
	del juego, implementando una máquina de estados de aplicación.

	Las clases que heredan de esta se preocuparán de inicializar
	los subsistemas necesarios para esa aplicación concreta
	(subsistema gráfico, etc.), así como de la creación de los estados
	que componen la aplicación.

	@remarks Las clases que implementen está clase <em>deben</em> inicializar el
	reloj (_clock).
	
	@ingroup applicationGroup

	@author Marco Antonio Gómez Martín & David Llansó
	@date Julio, 2010
	*/
	class CBaseApplication : public GUI::CKeyboardListener,
							 public GUI::CMouseListener
	{
	public:
		/**
		Constructor de la clase
		*/
		CBaseApplication();

		/**
		Destructor de la aplicación
		*/
		virtual ~CBaseApplication();

		/**
		Devuelve la aplicación instanciada. La clase
		implementa el patrón singleton, pero a través del
		constructor, es decir, se deberá hacer un <code>new</code>
		explícitamente (o declarar una variable global o local; en definitiva
		que se llame al constructor de la clase). Ese objeto creado
		será el que devuelva este método.

		@return Puntero a la aplicación instanciada.
		*/
		static CBaseApplication *getSingletonPtr() {return _instance;}

		/**
		Inicializa la aplicación. En depuración se comprueba que la aplicación
		se inicialice solo una vez.

		@return false si la inicialización ha fallado.
		*/
		virtual bool init();

		/**
		Finaliza la aplicación, liberando todos los recursos utilizados. En 
		depuración se comprueba que la aplicación esté inicializada.
		*/
		virtual void release();

		/**
		Libera y elimina todos los estados de la aplicación.
		*/
		virtual void releaseAllStates();

		/**
		Función de ejecución de la aplicación. Aquí se encuentra la ejecución 
		del bucle principal, que se repite hasta que alguien solicite su 
		terminación. En el bucle simplemente se comprueba si hay que realizar
		una transición de estado, si es así se hace efectivo el cambio. Tras 
		estó se hace el tick() para la actualización de la vuelta.
		*/
		virtual void run();

		/**
		Esta función es llamada cuando alguien externo a la
		aplicación (normalmente un estado), desea que la aplicación
		termine. Esta función está pensada para ser llamada mientras
		el método run() está en ejecución. Al terminar la vuelta actual
		se finalizará la aplicación.
		*/
		void exitRequest() {_exit = true;}

		/**
		Función para averiguar si alguien ha solicitado a la aplicación
		su terminación.

		@return Devuelve si la aplicación debe terminar con la vuelta
		del bucle actual.
		*/
		bool exitRequested() {return _exit;}

		// FUNCIONES ASOCIADAS CON LOS ESTADOS

		/**
		Añade un estado a la aplicación. La aplicación acepta la
		responsabilidad de borrar el estado al finalizar la aplicación.

		@param name Nombre del estado
		@param newState Estado a añadir. La aplicación se encargará
		de eliminarlo al finalizar.
		@return true si todo fue bien.
		*/
		bool addState(const std::string &name,
						CApplicationState *newState);

		/**
		Establece el estado de la aplicación, a partir de su nombre. La 
		acción <em>no</em> es inmediata, sino que se realizará en la siguiente
		vuelta del bucle principal de la aplicación.

		@param name Nombre del estado.
		@return Devuelve cierto si el estado solicitado existe. Si el
		estado no existe, <em>no</em> hay un cambio efectivo del estado.
		*/
		bool setState(const std::string &name);

		/**
		Devuelve el estado actual de la aplicación.

		@return Estado actual de la aplicación.
		*/
		CApplicationState *getState() {return _currentState;}

		/**
		Devuelve el tiempo de la aplicación.

		@return Tiempo que la aplicación lleva activa.
		*/
		unsigned int getAppTime();

		// Métodos de CKeyboardListener
		
		/**
		Método que será invocado siempre que se pulse una tecla. La
		aplicación avisa de este evento al estado actual.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyPressed(GUI::TKey key);
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla. La aplicación avisa de este evento al estado 
		actual.

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

	protected:

		/**
		Realiza un cambio de estado, avisando al estado saliente
		y al estado entrante del hecho.
		*/
		void changeState();

		/**
		Función que ejecuta una vuelta del bucle principal
		de la aplicación.

		@param msecs Número de milisegundos transcurridos desde
		la última llamada (o desde la áctivación de la aplicación, 
		en caso de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);

		/**
		Instancia única de la aplicación.
		*/
		static CBaseApplication *_instance;

		/**
		Indica si la aplicación ha sido inicializada
		*/
		bool _initialized;

		/**
		Tipo de datos tabla de estados. Es una tabla hash
		de las STL.
		*/
		typedef std::map<std::string, CApplicationState*> TStateTable;

		/**
		Tabla hash con todos los estados de la aplicación
		*/
		TStateTable _states;

		/**
		Puntero al estado actual
		*/
		CApplicationState *_currentState;

		/**
		Puntero al estado objetivo. Alguien externo a la apliación
		ha solicitado un cambio de estado que aún no se ha hecho
		efectivo.
		*/
		CApplicationState *_nextState;

		/**
		Indica si la aplicación terminará al final de la vuelta
		del bucel actual.
		*/
		bool _exit;

		/**
		Temporizador que lleva la cuenta del tiempo desde que se 
		inició la aplicación.
		*/
		IClock *_clock;

	}; // class BaseApplication

} // namespace Application

#endif // __Application_BaseApplication_H
