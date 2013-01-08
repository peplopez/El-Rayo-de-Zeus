/**
@file Server.h

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __AI_Server_H
#define __AI_Server_H

#include "WaypointGraph.h"
#include "AStarFunctionsGaleon.h"
#include "micropather.h"

namespace AI {


/**
Servidor de IA. De momento sólo se encarga de mantener el
grafo con los waypoints y de centralizar las llamadas a A*.

La idea es que en un futuro también se ocupe de gestionar la 
percepción.
*/
class CServer
{
	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor de IA.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor de IA. Esta operación sólo es necesario realizarla
		una vez durante la inicialización de la aplicación. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor de IA. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Función invocada en cada vuelta para actualización.

		@param secs Segundos desde que se reenderizó el último frame.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(float secs);

		/**
		Añade un nuevo nodo al grafo de navegación.
		*/
		void addWaypoint(Vector3 waypoint);
		/**
		Recalcula el grafo de navegación a partir de los nodos que han sido
		añadidos con addGraphNode.
		*/
		void computeNavigationGraph();
		/**
		Devuelve el grafo de navegación.
		*/
		CWaypointGraph* getNavigationGraph() {return &_wpg; };
		/**
		Calcula una ruta usando A*.
		*/
		vector<Vector3> *getAStarRoute(Vector3 from, Vector3 to);
		/**
		Dado un ángulo en radianes lo lleva al intervalo [-PI, PI]
		*/
		static double correctAngle(double angle);

	private:
		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();
		/**
		Instancia única de la clase.
		*/
		static CServer *_instance;

		CWaypointGraph _wpg;
		/**
		Clase que se encarga de calcular la mejor ruta con A*
		*/
		// CAStar* _aStar;
		micropather::MicroPather* _aStar;
		/** 
		Funciones de distancia para calcular A*
		*/
		CAStarFunctionsGaleon* _f;

}; // class CServer

} // namespace AI
#endif