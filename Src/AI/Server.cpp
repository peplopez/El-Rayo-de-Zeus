/**
@file Server.cpp

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "Server.h"

#include <assert.h>


namespace AI {
	/////////////////////////////////////////
	/**
	Instancia única de la clase.
	*/
	CServer* CServer::_instance = 0;
	/////////////////////////////////////////
	/**
	Constructor de la clase.
	*/
	CServer::CServer(void)
	{
		assert(!_instance && "Segunda inicialización de AI::CServer no permitida!");
		_f = new CAStarFunctionsGaleon();
		_aStar = new micropather::MicroPather(_f);
	}
	/////////////////////////////////////////
	/**
	Destructor de la clase.
	*/
	CServer::~CServer(void)
	{
		assert(_instance);
		delete _aStar;
		delete _f;
	}
	/////////////////////////////////////////
	/**
	Inicializa el servidor de IA. Esta operación sólo es necesario realizarla
	una vez durante la inicialización de la aplicación. 

	@return Devuelve false si no se ha podido inicializar.
	*/
	bool CServer::Init() 
	{
		_instance = new CServer();
		return true;
	}
	/////////////////////////////////////////
	/**
	Libera el servidor de IA. Debe llamarse al finalizar la aplicación.
	*/
	void CServer::Release()
	{
		if (_instance)
			delete _instance;
		_instance = 0;
	}
	/////////////////////////////////////////
	/**
	Añade un nuevo nodo al grafo de navegación.
	*/
	void CServer::addWaypoint(Vector3 waypoint)
	{
		_wpg.addWaypoint(waypoint);
	}
	/////////////////////////////////////////
	/**
	Recalcula el grafo de navegación a partir de los nodos que han sido
	añadidos con addGraphNode.
	*/
	void CServer::computeNavigationGraph() 
	{
		_wpg.computeNavigationGraph();
	}
	/////////////////////////////////////////
	/**
	Calcula una ruta usando A*.
	*/
	std::vector<Vector3> *CServer::getAStarRoute(Vector3 from, Vector3 to)
	{
		// TODO PRÁCTICA IA
		// Dadas dos posiciones devuelve una ruta para llegar de una a otra.
		// En primer lugar utilizamos el grafo de waypoints para obtener los nodos
		// más cercanos al origen y al destino. 
		// A continuación usamos A* para calcular la ruta.
		// Por último tenemos que devolver la ruta en un vector de posiciones. Para 
		// ello tendremos que convertir los ids de los nodos en sus posiciones y añadir
		// las posiciones de origen y destino.
		int idFrom = _wpg.getClosestWaypoint(from);
		int idTo = _wpg.getClosestWaypoint(to);
		vector<void*>* path = new vector<void*>();
		float cost = 0.0f;
		int solved = _aStar->Solve((void*) idFrom, (void*) idTo, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return 0;
		}

		vector<Vector3>* out = new vector<Vector3>();
		// Añadimos el punto inicial si no coincide con el primer nodo
		if (!from.positionEquals(_wpg.getNode((int)((*path)[0]))->position, 5.0))
			out->push_back(from);
		for (vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			out->push_back(_wpg.getNode((int)(*it))->position);
		}
		// Añadimos el punto final si no coincide con el último nodo
		if (!to.positionEquals(_wpg.getNode((int) ((*path)[path->size() - 1]))->position, 5.0))
			out->push_back(to);
		delete path;
		return out;
	}
	/////////////////////////////////////////
	/**
	Dado un ángulo en radianes lo lleva al intervalo [-PI, PI]
	*/
	double CServer::correctAngle(double angle)
	{
		while (angle > Math::PI)
			angle -= 2 * Math::PI;
		while (angle < -Math::PI)
			angle += 2 * Math::PI;
		return angle;
	}

} // namespace AI