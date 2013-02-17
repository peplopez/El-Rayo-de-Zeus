/**
@file ErrorManager.h

Contiene la declaración del gestor de errores para PhysX.

@see Physics::CErrorManager

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Physics_ErrorManager_H
#define __Physics_ErrorManager_H

#include <foundation/PxErrorCallback.h>


// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	 Gestor de errores para PhysX. Cuando PhysX detecta que se intenta hacer algo incorrecto 
	 (por ejemplo, llamadas a funciones con parámetros no válidos), invoca a este gestor 
	 que a su vez imprime el mensaje en la consola.

	 @ingroup physicGroup

	 @author Antonio Sánchez Ruiz-Granados
	 @date Noviembre, 2012
	 */
	class CErrorManager : public physx::PxErrorCallback
	{
	public:
		/**
		 * Constructor por defecto.
		 */
		CErrorManager();

		/**
		 * Destructor.
		 */
		~CErrorManager();

		/**
		 * Método invocado por PhysX cada vez que se produce un error. El mensaje de error se 
		 * imprime por pantalla.
		 */
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);

	}; // classCErrorManager

}; // namespace Physics

#endif // __Physics_ErrorManager_H