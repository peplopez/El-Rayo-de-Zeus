//---------------------------------------------------------------------------
// 3DApplication.h
//---------------------------------------------------------------------------

/**
@file 3DApplication.h

Contiene la declaración de la clase aplicacion para aplicaciones
basadas en Ogre. Esta clase maneja la ejecución de todo el juego.

@see Application::C3DApplication
@see Application::CBaseApplication

@author David Llansó
@date Julio, 2010
 */

#ifndef __Application_3DApplication_H
#define __Application_3DApplication_H

#include "BaseApplication.h"

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
	Clase C3DApplication, que contiene el control de la aplicación
	del juego. Extiende CBaseApplication y su máquina de estados para
	realizar las tareas de pintado de la escena usando Ogre. Para 
	la interfaz de usuario utiliza OIS y CeGUI.

	@ingroup applicationGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class C3DApplication : public CBaseApplication
	{
	public:
		/**
		Constructor de la clase
		*/
		C3DApplication();

		/**
		Destructor de la aplicación
		*/
		virtual ~C3DApplication();

		/**
		Inicializa la aplicación.

		@return false si la inicialización ha fallado.
		*/
		virtual bool init();

		/**
		Finaliza la aplicación, liberando todos los recursos
		utilizados.
		*/
		virtual void release();

	protected:

		/**
		Función que ejecuta una vuelta del bucle principal
		de la aplicación.

		@param msecs Número de milisegundos transcurridos desde
		la última llamada (o desde la activación de la aplicación, 
		en caso de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);

	}; // class 3DApplication

} // namespace Application

#endif // __Application_3DApplication_H
