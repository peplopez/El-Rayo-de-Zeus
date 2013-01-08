/**
@file AStarFunctionsGaleon.h

En este fichero se implementan las funciones
necesarias para calcular rutas usando A*.


@author Gonzalo Flórez
@date Diciembre, 2010
*/

#pragma once

#ifndef __AI_AStarFunctionsGaleon_H
#define __AI_AStarFunctionsGaleon_H

#include "micropather.h"

namespace AI 
{
	/**
	Clase que hereda de micropather::Graph y que contiene las funciones
	de A* específicas de Galeon.
	*/
	class CAStarFunctionsGaleon : public micropather::Graph
	{
	public:
		/** 
		Constructor
		*/
		CAStarFunctionsGaleon(void);
		/** 
		Destructor
		*/
		~CAStarFunctionsGaleon(void);
		/**
		Devuelve el coste según la heurística para llegar desde el estado stateStart hasta stateEnd.
		Para que el camino devuelto por A* sea óptimo la heurística sea aceptable y no sobreestimar 
		la distancia.
		Para la búsqueda de caminos de Galeon utilizaremos como heurística la distancia euclídea
		entre los puntos.
		*/
		virtual float LeastCostEstimate( void* stateStart, void* stateEnd );

		/** 
		Devuelve la lista de vecinos de un nodo junto con el coste de llegar desde el nodo actual
		hasta cada uno de ellos.
		En Galeon usaremos el grafo de waypoints para obtenerla.
		*/	
		virtual void AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent );

		/**
			This function is only used in DEBUG mode - it dumps output to stdout. Since void* 
			aren't really human readable, normally you print out some concise info (like "(1,2)") 
			without an ending newline.
		*/
		virtual void  PrintStateInfo( void* state );

	};

} // namespace AI 

#endif //__AI_AStarFunctionsGaleon_H