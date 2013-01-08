/**
@file RouteTo.h

Contiene la declaración de la clase CSteringMovement. Se encarga de
mover un avatar utilizando movimientos cinéticos o dinámicos.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __Logic_RouteTo_H
#define __Logic_RouteTo_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	/**
	Componente que se encarga de gestionar el cálculo y el seguimiento
	de rutas. El componente acepta mensajes de tipo ROUTE_TO con un parámetro
	_vector que indica la posición de destino.
	<p>
	Cuando se recibe un mensaje de este tipo, se calcula una nueva ruta
	hasta el destino indicado y se comienza su seguimiento. Para cada 
	segmento de la ruta, el componente envía un mensaje de tipo MOVE_TO, con el 
	destino de ese segmento y el tipo de movimiento necesario como parámetros 
	(estos mensajes pueden ser aceptados por el componente CSteeringMovement).
	<p>
	Cuando se llega al último punto de la ruta la entidad se detiene y envía 
	un mensaje del tipo FINISHED_ROUTE.
	<p>
	Si es imposible obtener una ruta hasta el destino se envía un mensaje FAILED_ROUTE.
	*/
	class CRouteTo : public IComponent
	{
	
		DEC_FACTORY(CRouteTo);
	
	public:
		/**
		Constructor
		*/
		CRouteTo(void) : IComponent (), _recalculateRoute(false), _currentRoute(0), _arrived(true),
			_seekTolerance(7.0), _arriveTolerance(0.5){};
		/**
		Destructor
		*/
		~CRouteTo(void) { if (_currentRoute != 0) delete _currentRoute; };
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad máxima a la que se moverá la entidad.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es necesario.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);
		/**
		Este componente sólo acepta mensajes de tipo ROUTE_TO.
		*/
		virtual bool accept(const TMessage &message);
		/**
		Si se recibe un mensaje de tipo ROUTE_TO, almacena el vector como 
		destino del movimiento.
		*/
		virtual void process(const TMessage &message);

	private:
		/**
		Destino de la ruta.
		*/
		Vector3 _target;
		/**
		Indica si hay que volver a calcular la ruta
		*/
		bool _recalculateRoute;
		/**
		Indica si se ha llegado al destino.
		*/
		bool _arrived;
		/**
		Almacena el punto de la ruta al que se está intentando llegar.
		*/
		unsigned int _currentNode;
		/**
		Almacena la ruta actual
		*/
		std::vector<Vector3> *_currentRoute;
		/**
		Guarda las distancias a las que se considera que se ha llegado a un 
		punto de ruta al usar un movimiento de tipo seek o arrive
		*/
		float _seekTolerance, _arriveTolerance;
		/**
		Guarda la tolerancia para el movimiento actual.
		*/
		float _distanceTolerance;
		/** 
		Envía un mensaje MOVE_TO.

		@param target Destino.
		@movementType Tipo de movimiento.
		*/
		void sendMoveMessage(Vector3 target, int movementType);
	};

	REG_FACTORY(CRouteTo);

} // namespace Logic

#endif //#ifndef __Logic_RouteTo_H
