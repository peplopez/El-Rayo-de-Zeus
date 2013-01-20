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
#ifndef __Logic_LogicalPosition_H
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
		const enum Ring{	
			ANILLO_INFERIOR=0,
			ANILLO_CENTRAL=1,
			ANILLO_SUPERIOR=2
		};
		const enum Sense{ //Quizá mejor sentido angunos positivos/negativos?	
			IZQUIERDA=0,
			DERECHA=1
		};
		

	}

	//Coordenadas comunes a todas las entidades del juego.
	
	/**
	Tipo copia para los anillos. Por simplicidad.
	*/
	typedef LogicalPosition::Ring Ring;
	typedef LogicalPosition::Sense Sense;

	
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
		Ring _ring;
		/**
			Sentido
		*/
		Sense _sense;
		/**
			Número de base: 1-8
		*/
		unsigned short _base;
		/**
			Grados
		*/	
		float _degrees;


		/**
			Mi intención es quitarlo más adelante, al decir el anillo ya estás diciendo el radio, este no cambia jamás durante la ejecución
		*/	
		//float _radio;

		/**
			Anchura de la entidad gráfica en grados
		*/	
		float _angularBox;
	}TLogicalPosition; 

} // namespace Logic

#endif // __Logic_LogicalPosition_H