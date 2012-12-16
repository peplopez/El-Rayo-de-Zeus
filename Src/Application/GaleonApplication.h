//---------------------------------------------------------------------------
// GaleonApplication.h
//---------------------------------------------------------------------------

/**
@file GaleonApplication.h

Contiene la declaración de la clase aplicacion para aplicaciones
basadas en Ogre. Esta clase maneja la ejecución de todo el juego.

@see Application::CGaleonApplication
@see Application::C3DApplication

@author David Llansó
@date Septiembre, 2010
 */

#ifndef __Application_GaleonApplication_H
#define __Application_GaleonApplication_H

#include "3DApplication.h"
#include "GUI/InputManager.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Application 
{
  class CApplicationState;
}
namespace Graphics 
{
  class CServer;
}
namespace Ogre 
{
  class Timer;
}

namespace Application 
{
	/**
	Clase CGaleonApplication, que extiende C3DApplication para añadir
	los estados concretos que se desean crear en la partida.

	@ingroup applicationGroup

	@author David Llansó
	@date Septiembre, 2010
	*/
	class CGaleonApplication : public C3DApplication
	{
	public:
		/**
		Constructor de la clase
		*/
		CGaleonApplication();

		/**
		Destructor de la aplicación
		*/
		virtual ~CGaleonApplication();

		/**
		Inicializa la aplicación añadiendo los estados concretos.

		@return false si la inicialización ha fallado.
		*/
		virtual bool init();

		/**
		Finaliza la aplicación, liberando todos los recursos
		utilizados y destruyendo los estados creados.
		*/
		virtual void release();

	}; // class GaleonApplication

} // namespace Application

#endif // __Application_GaleonApplication_H
