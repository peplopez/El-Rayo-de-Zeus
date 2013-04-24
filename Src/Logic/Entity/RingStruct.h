  //---------------------------------------------------------------------------
// RingStruct.h
//---------------------------------------------------------------------------

/**
@file RingStruct.h

Contiene las constantes de posiciones cartesianas de los anillos.

@see Logic::RingStruct

@author Jose Luis López Sánchez
*/
#pragma once
#ifndef __Logic_RingStruct_H
#define __Logic_RingStruct_H

#include <string>

#include "BaseSubsystems/Math.h"

namespace Logic

{	
	//si queremos, estas tres constantes pueden ser parametros de entrada
	// pero al ser algo que no se va a cambiar frecuentemente no hace falta
	const Vector3 separationBetweenRings = Vector3(0,50,0);
	const Vector3 startingRingPosition = Vector3(0,-50,0);

	const unsigned short RADIO_MAYOR = 73;
	const unsigned short RADIO_MENOR = 58;

	
	struct TRingPositions
	{
		/**
		Y, si están apiladas comparten "x" y "z" 
		*/
		static Vector3 _up;
		static Vector3 _center;
		static Vector3 _down;
	};


	
}
// namespace Logic

#endif // __Logic_RingStruct_H