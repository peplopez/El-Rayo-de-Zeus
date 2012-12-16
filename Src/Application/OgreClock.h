//---------------------------------------------------------------------------
// OgreClock.h
//---------------------------------------------------------------------------

/**
@file OgreClock.h

Contiene la declaración de un temporizador basado en Ogre.

@see Application::COgreClock

@author David Llansó
@date Julio, 2010
*/

#ifndef __Application_OgreClock_H
#define __Application_OgreClock_H

#include "Clock.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Timer;
}

namespace Application 
{
	/**
	Este temporizador  getTime() que deberá ser implementado en 
	las clases hijas y que devuelve el tiempo desde que se inició la
	aplicación, y getTimeSinceLastRequest que devuelve el tiempo transcurrido
	desde la última llamada al método. 

	@remarks Esta clase pide en su constructor una instancia del reloj de
	Ogre por lo que BaseSubsystems::CServer deverá haber sido inicializado
	antes de la construcción de esta clase.

	@ingroup applicationGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class COgreClock : public IClock
	{
	public:
		/** 
		Constructor de la clase 
		*/
		COgreClock();

		/** 
		Destructor 
		*/
		virtual ~COgreClock() {}

	protected:

		/**
		 Método que devuelve la "hora física" del sistema
		 en milisegundos.

		 Utiliza el reloj de Ogre.

		@return Hora en milisegundos.
		*/
		virtual unsigned long getPhysicalTime();

		/**
		Temporizador de Ogre.
		*/
		Ogre::Timer* _timer;

	}; // COgreClock

} // namespace Application

#endif //  __Application_OgreClock_H
