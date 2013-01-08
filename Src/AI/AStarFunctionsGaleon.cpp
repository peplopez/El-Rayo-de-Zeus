/**
@file AStarFunctionsGaleon.cpp

En este fichero se implementan las funciones
necesarias para calcular rutas usando A*.


@author Gonzalo Flórez
@date Diciembre, 2010
*/

#include "AStarFunctionsGaleon.h"

#include "Server.h"

namespace AI 
{

	//---------------------------------------------------------
	/** 
	Constructor
	*/
	CAStarFunctionsGaleon::CAStarFunctionsGaleon(void)
	{
	}

	//---------------------------------------------------------
	/** 
	Destructor
	*/
	CAStarFunctionsGaleon::~CAStarFunctionsGaleon(void)
	{
	}

	//---------------------------------------------------------
	/**
	Devuelve el coste según la heurística para llegar desde el estado stateStart hasta stateEnd.
	Para que el camino devuelto por A* sea óptimo la heurística sea aceptable y no sobreestimar 
	la distancia.
	Para la búsqueda de caminos de Galeon utilizaremos como heurística la distancia euclídea
	entre los puntos.
	*/
	float CAStarFunctionsGaleon::LeastCostEstimate( void* stateStart, void* stateEnd )
	{
		// TODO PRÁCTICA IA
		// Función heurística para A*.
		// En el caso de Galeón, una heurística admisible es la distancia entre los nodos 
		// indicados por stateStart y stateEnd.
		// Para calcularla tenemos que obtener los nodos del grafo de navegación, al que 
		// podemos acceder mediante el servidor de IA
		int idOrigen = (int) stateStart;
		int idDestino = (int) stateEnd;
		// Utilizamos el grafo de navegación:
		CWaypointGraph* wpg = CServer::getSingletonPtr()->getNavigationGraph();
		const CNode* orig = wpg->getNode(idOrigen);
		const CNode* dest = wpg->getNode(idDestino);
		return orig->position.distance(dest->position);
	}

	//---------------------------------------------------------
	/** 
	Devuelve la lista de vecinos de un nodo junto con el coste de llegar desde el nodo actual
	hasta cada uno de ellos.
	En Galeon usaremos el grafo de waypoints para obtenerla.
	*/	
	void CAStarFunctionsGaleon::AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent )
	{
		// TODO PRÁCTICA IA
		// Esta función nos da la lista de vecinos de un nodo y el coste real (no heurístico) para llegar a
		// cada uno de ellos.
		// Para acceder a la lista de vecinos usamos el grafo de navegación.
		// Para el coste de cada vecino consultamos la arista lo une con el nodo.
		// Estos datos se devuelven en un vector de pares [idNodo, coste] (tipo StateCost).
		int idNodo = (int) state;
		// Utilizamos el grafo de navegación:
		CWaypointGraph* wpg = CServer::getSingletonPtr()->getNavigationGraph();
		list<unsigned int> neighbours = wpg->getNeighbours(idNodo);
		// Llenamos la lista de adyacentes con pares de (nodo vecino, coste en llegar)
		for (list<unsigned int>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
			micropather::StateCost nodeCost = {(void*)(*it), wpg->getCost((int) state, (*it))};
			adjacent->push_back(nodeCost);
		}
	}

	//---------------------------------------------------------
	/**
		This function is only used in DEBUG mode - it dumps output to stdout. Since void* 
		aren't really human readable, normally you print out some concise info (like "(1,2)") 
		without an ending newline.
	*/
	void  CAStarFunctionsGaleon::PrintStateInfo( void* state )
	{
		CWaypointGraph* wpg = CServer::getSingletonPtr()->getNavigationGraph();
		Vector3 position = wpg->getNode((int) state)->position;
		printf("(%f, %f, %f)", position.x, position.y, position.z);
	}

	//---------------------------------------------------------

} // namespace AI