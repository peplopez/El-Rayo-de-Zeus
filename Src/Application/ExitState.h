//---------------------------------------------------------------------------
// ExitState.h
//---------------------------------------------------------------------------

/**
@file ExitState.h

Contiene la declaración de un estado muy sencillo e independiente
de la plataforma, que provoca la terminación de la aplicación.

@see Application::CApplicationState
@see Application::CExitState

@author Marco Antonio Gómez Martín & David Llansó
@date Julio, 2010
*/

#ifndef __Application_ExitState_H
#define __Application_ExitState_H

#include "ApplicationState.h"

namespace Application 
{
	class CBaseApplication;
}

namespace Application 
{
	/**
	Clase CExitState; representa el estado de la aplicación
	que provoca su terminación inmediata.
	
	@ingroup applicationGroup

	@author Marco Antonio Gómez Martín & David Llansó
	@date Julio, 2010
	*/
	class CExitState : public CApplicationState 
	{
	public:
		/** 
		Constructor de la clase 
		*/
		CExitState(CBaseApplication *app) : CApplicationState(app) {}

		/** 
		Destructor 
		*/
		virtual ~CExitState() {}

		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		virtual void activate() 
		{
			CApplicationState::activate();
			this->_app->exitRequest();
		}

	}; // CExitState

} // namespace Application

#endif // __Application_ExitState_H
