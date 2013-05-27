/**
@file HudController.h

Contiene la declaración de la clase CHudController. Se encarga de
recibir eventos del teclado (la R) para recargar el HUD

@author Pablo Terrado Contreras
@date Mayo, 2013
*/

#ifndef __GUI_HudController_H
#define __GUI_HudController_H

#include "InputManager.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CEntity;
}

namespace ScriptManager
{
	class CServer;
}

// Declaración de la clase
namespace GUI
{
	/**
	Esta clase sirve para gestionar el teclado, y recargar el HUD 
	
	@ingroup GUIGroup

	@author Pablo Terrado
	@date Mayo, 2013
	*/

	class CHudController : public CKeyboardListener
	{
	public:

		/**
		Constructor.
		*/
		CHudController();

		/**
		Destructor.
		*/
		~CHudController();
 

		/**
		Activa la la clase, se registra en el CInputManager y así empieza a 
		escuchar eventos.
		*/
		void activate();

		/**
		Desctiva la la clase, se deregistra en el CInputManager y deja de 
		escuchar eventos.
		*/
		void deactivate();

		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se pulse una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyPressed(TKey key);
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyReleased(TKey key);


	}; // class CHudController

} // namespace GUI

#endif // __GUI_HudController_H
