/**
@file WaypointGraph.h

Clase para mantener grafos de puntos de ruta (waypoints).

@see WaypointGraph

@author Gonzalo Flórez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_WaypointGraph_H
#define __AI_WaypointGraph_H

#include "BaseSubsystems/Math.h"

#include "UndirectedGraph.h"

namespace AI {

/**
Esta clase mantiene un grafo con los puntos de ruta. Cada aristas 
del grafo representa que existe un camino entre los nodos que une 
y la etiqueta de la arista, la distancia.
<p>
El grafo es no dirigido, lo que significa que no se pueden representar 
aristas unidireccionales (por ejemplo, precipicios donde un personaje 
puede caer pero no puede volver a subir).
<p>
La generación del grafo es automática. En primer lugar se añaden todos 
los nodos con el método addWaypoint y, a continuación, se invoca a 
computeNavigationGraph para generar el grafo. Mientras, se genera se 
lleva a cabo una comprobación para averiguar si el grafo obtenido es 
conexo. 
*/
class CWaypointGraph
{
public:
	/**
	Constructor
	*/
	CWaypointGraph(void);
	/**
	Destructor
	*/
	~CWaypointGraph(void);
	/**
	Añade un nuevo waypoint al grafo.
	*/
	void addWaypoint(Vector3 waypoint);
	/**
	Calcula las aristas entre los waypoints. Para ello, entre cada par 
	de waypoints hace una comprobación de visibilidad. Si un waypoint es
	visible desde otro, se añade una arista entre ellos.
	*/
	void computeNavigationGraph();
	/**
	Devuelve true si se han calculado las aristas y no se ha añadido después
	ningún nodo.
	*/
	bool isClean();
	/** 
	Devuelve true si el grafo es conexo (existe un camino entre cualquier 
	par de nodos).
	*/
	bool isConnected();
	/**
	Devuelve un nodo del grafo.
	*/
	const CNode* getNode(int idNode) { return _g.getNode(idNode); } ;
	/**
	Devuelve el coste asociado a la arista entre dos nodos (la distancia).
	*/
	float getCost(int node1, int node2) { return _g.getEdgeCost(node1, node2); };
	/**
	Devuelve la lista de vecinos de un nodo.
	*/
	list<unsigned int> getNeighbours(int idNode) { return _g.getNeighbours(idNode); };
	/**
	Obtiene el waypoint más cercano a una posición dada.
	<p>
	Hace una búsqueda lineal en la lista de nodos y devuelve el más cercano.
	*/
	int getClosestWaypoint(Vector3 position);
	/**
	Devuelve el número de waypoints que contiene el grafo.
	*/
	int getWaypointCount() { return _g.getNodeCount(); };
	/**
	Devuelve la posición de un waypoint, dado su identificador.
	*/
	Vector3 getWaypoint(int numWaypoint) { return _g.getNode(numWaypoint)->position; };


private:
	/**
	Será cierto si el grafo se ha generado al menos 1 vez
	*/
	bool _generated;
	/** 
	Es cierto si el grafo generado es conexo. Es falso si no es conexo o
	aún no se ha generado.
	*/
	bool _connected;
	/** 
	Es cierto si se ha generado y no se ha añadido después ningún nodo.
	Es falso si se ha añadido algún nodo o no se ha generado.
	*/
	bool _clean;
	CUndirectedGraph _g;

	// TEST
	bool getVisible(int id1, int id2);

}; // class CWaypointGraph

} // namespace AI

#endif __AI_WaypointGraph_H
