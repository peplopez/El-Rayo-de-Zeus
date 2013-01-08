/**
@file WaypointGraph.cpp

Clase para mantener grafos de puntos de ruta (waypoints).

@see WaypointGraph

@author Gonzalo Flórez
@date Diciembre 2010

*/
#include "WaypointGraph.h"
#include "DisjointSets.h"
#include "Physics\Server.h"

#include "Logic\Entity\Entity.h"

namespace AI {

	/**
	Constructor
	*/
	CWaypointGraph::CWaypointGraph(void) : _generated(false) , _connected(false), _clean(false)
	{
	}
	/////////////////////////////////////////
	/**
	Destructor
	*/
	CWaypointGraph::~CWaypointGraph(void)
	{
	}
	/////////////////////////////////////////
	/**
	Añade un nuevo waypoint al grafo.
	*/
	void CWaypointGraph::addWaypoint(Vector3 waypoint) 
	{
		_g.addNode(new CNode(waypoint));
		_clean = false;
	}
	/////////////////////////////////////////
	/**
	Calcula las aristas entre los waypoints. Para ello, entre cada par 
	de waypoints hace una comprobación de visibilidad. Si un waypoint es
	visible desde otro, se añade una arista entre ellos.
	*/
	void CWaypointGraph::computeNavigationGraph()
	{
		// TODO PRÁCTICA IA
		// Generar las aristas para el grafo de navegación
		// Para cada par de nodos del grafo tenemos que comprobar si se puede llegar en línea recta
		// El grafo subyacente en el que se basa esta clase se encuentra en la variable _g, y es un 
		// grafo no dirigido implementado en la clase CUndirectedGraph. En el fichero de cabecera de 
		// esta clase se pueden encontrar los métodos para acceder a los nodos, añadir aristas, etc.
		// Como el grafo es no dirigido, sólo necesitamos hacer comparaciones en 1 sentido (si B es 
		// visible desde A sabemos que A es visible desde B). Al añadir la arista de A a B se añade 
		// automáticamente la arista de B a A.
		// Para comprobar la visibilidad usamos el método raycastClosest que nos proporciona el 
		// servidor de física. Este método recibe un rayo y la distancia máxima y devuelve la entidad 
		// con la que ha colisionado o NULL. Sólo nos interesa añadir una arista si el rayo no ha
		// colisionado con el mundo. Podemos saber si es el mundo usando la propiedad getType de
		// la entidad que devuelve raycastClosest, comparándola con la cadena "World".
		int numNodes = _g.getNodeCount();
		DisjointSets sets(numNodes);
		for (int i = 0; i < numNodes - 1; i++) {
			const CNode* node1 = _g.getNode(i);
			Vector3 pos1 = node1->position;
			for (int j = i + 1; j < numNodes; j++) {
				const CNode* node2 = _g.getNode(j);
				Vector3 pos2 = node2->position;
				// Sacamos la dirección (en pos2) y la distancia
				pos2 -= pos1;
				float dist = pos2.length();
				// El rayo necesita la dirección normalizada
				pos2 /= dist;
				// Si el rayo no choca con ninguna entidad ==> devuelve NULL ==> hay una recta entre los waypoints
				Logic::CEntity *entCol = 
					Physics::CServer::getSingletonPtr()->raycastClosest(Ray(pos1, pos2), dist);
				if (entCol == NULL || entCol->getType() != "World") {
					_g.addEdge(i, j, dist);
					int idI = sets.FindSet(i);
					int idJ = sets.FindSet(j);
					sets.Union(idI, idJ);
				}
			}
		}
		_connected = sets.NumSets() == 1;
		_clean = true;
		_generated = true;

		// Reseteamos las alturas de los nodos
		for (int i = 0; i < _g.getNodeCount(); i++) {
			_g.getNode(i)->position.y = 0;
		}
	}
	/////////////////////////////////////////
	/**
	Obtiene el waypoint más cercano a una posición dada.
	<p>
	Hace una búsqueda lineal en la lista de nodos y devuelve el más cercano.
	*/
	int CWaypointGraph::getClosestWaypoint(Vector3 position)
	{
		// TODO PRÁCTICA IA
		// Recorremos la lista de waypoints y devolvemos el más cercano a la posición dada.
		// En lugar de calcular la distancia podemos calcular la distancia al cuadrado 
		// (squaredDistance). De esta manera nos ahorramos una raíz cuadrada.
		// En algunos casos, el resultado de este método no es válido para calcular una ruta, pues
		// no existe un camino en línea recta desde la posición de entrada hasta el waypoint.
		// Para evitar este problema, podemos comprobar que el nodo es visible desde la posición 
		// indicada. Esto puede aumentar mucho el coste de esta operación.

		int numNodes = _g.getNodeCount();
		if (numNodes == 0) return -1;

		int closestNode = 0;
		Vector3 node = _g.getNode(0)->position;
		double minDist = node.squaredDistance(position);
		for (int i = 1; i < numNodes; i++) {
			Vector3 node = _g.getNode(i)->position;
			double dist = node.squaredDistance(position);
			if (dist <= minDist) {
				minDist = dist;
				closestNode = i;
			}
		}
		return closestNode;
	}

}