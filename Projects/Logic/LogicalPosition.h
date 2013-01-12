//---------------------------------------------------------------------------
// LogicalPosition.h
//---------------------------------------------------------------------------


/**
@file LogicalPosition.h

Contiene el tipo de datos de un mensaje.

@see Logic::LogicalPosition

@author Jose Luis López Sánchez
*/
#pragma once

#define __Logic_LogicalPosition_H

#include <string>

#include "BaseSubsystems/Math.h"

namespace Logic
{
	/**
	Namespace para los tipos de mensajes posibles.
	*/
	namespace LogicalPosition
	{					
		/**
			Ambito actual hasta que se cambie de base y/o anillo
		*/
		enum Ring{	
			ANILLO_INFERIOR,
			ANILLO_CENTRAL,
			ANILLO_SUPERIOR
		};
	}
		//Coordenadas comunes a todas las entidades del juego.
	
	/**
	Tipo copia para los anillos. Por simplicidad.
	*/
	typedef LogicalPosition::Ring Ring;
	
	/**
	Contiene el tipo de datos de una posición lógica de representación espacial dentro de nuestro juego
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author José Luis López Sánchez
	@date Diciembre, 2012
    @ingroup grupoEntidad
	*/
	typedef struct
	{
		/**
			Anillo
		*/
		Ring _type;
		/**
			Número de base: 1-8
		*/
		short base;
		/**
			Grados en radianes: 1-8
		*/	
		float _degrees;

		/**
			Mi intención es quitarlo más adelante, al decir el anillo ya estás diciendo el radio, este no cambia jamás durante la ejecución
		*/	
		float _radio;

	} LogicalPosition; 

} // namespace Logic

