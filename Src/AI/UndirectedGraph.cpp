/**
@file UndirectedGraph.cpp

Clase que permite crear grafos no dirigidos.

@see UndirectedGraph

@author Gonzalo Flórez
@date Diciembre 2010

*/
#include "UndirectedGraph.h"

/////////////////////////////////////////
/**
Constructor
*/
CUndirectedGraph::CUndirectedGraph(void) : _edgeCount(0)
{
}
/////////////////////////////////////////
/**
Destructor
*/
CUndirectedGraph::~CUndirectedGraph(void)
{
	map<unsigned int, list<unsigned int>*>::iterator it;
	for (it = _adjacency.begin(); it != _adjacency.end(); it++) {
		delete (*it).second;
	}
	vector<CNode*>::iterator it2;
	for (it2 = _nodes.begin(); it2 != _nodes.end(); it2++) {
		delete (*it2);
	}
}
/////////////////////////////////////////
/**
Añade un nodo al grafo. Es el propio grafo en su destructor quien
se encarga de eliminar los nodos. NO elimina su contenido, solamente
el CNode.

@param node puntero al nodo que se añade.
*/
void CUndirectedGraph::addNode(CNode* node)
{
	_nodes.push_back(node);
}
/////////////////////////////////////////
/**
Crea una arista entre dos nodos existentes y le asigna un coste.

@param node1 Identificador del primer nodo.
@param node2 Identificador del segundo nodo.
@param cost Coste asociado a la arista.
*/
void CUndirectedGraph::addEdge(unsigned int node1, unsigned int node2, float cost)
{
	assert(node1 < _nodes.size() && node2 < _nodes.size());
	setAdjacent(node1, node2);
	setAdjacent(node2, node1);
	_cost.setElement(node1, node2, cost);
	_edgeCount++;
}
/////////////////////////////////////////
/**
Crea una arista que va desde el nodo de origen (orig) al de destino (dest)
añadiendo el segundo a la lista del primero. Crea la lista en el caso de que
no exista.
<p>
@remarks Este método no crea la arista en sentido contrario (desde dest a orig). 
Hay que hacerlo manualmente.

@param orig Id del nodo de origen.
@param dest Id del nodo de destino.
*/
void CUndirectedGraph::setAdjacent(unsigned int orig, unsigned int dest) {
	list<unsigned int>* neighbours = _adjacency[orig];
	// Si es null es porque nuevo
	if (neighbours == NULL) {
		neighbours = new list<unsigned int>();
		_adjacency[orig] = neighbours;
	}
	neighbours->push_back(dest);
}
/////////////////////////////////////////
/**
Devuelve una lista con todos los nodos conectados al especificado.

@param idNode Identificador del nodo para el que se va a obtener la 
lista de vecinos.
@return Lista de vecinos.
*/
const list<unsigned int> &CUndirectedGraph::getNeighbours(unsigned int idNode) 
{
	assert(idNode < _nodes.size());
	list<unsigned int>* neighbours = _adjacency[idNode];
	if (neighbours == NULL) {
		return *(new list<unsigned int>());
	} else {
		return (*neighbours);
	}
}
/////////////////////////////////////////
/**
Devuelve la etiqueta de un nodo.

@param id Identificador del nodo.
@return CNode con el contenido del nodo.
*/
CNode* CUndirectedGraph::getNode(unsigned int id)
{
	assert(id < _nodes.size());
	return (_nodes[id]);
}
