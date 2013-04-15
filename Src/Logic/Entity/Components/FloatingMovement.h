/**
@file FloatingMovement.h

Contiene la declaración de la cabecera del componente
que controla el movimiento de los items.

@see Logic::CFloatingMovement
@see Logic::IComponent

@author Francisco Aisa García / Cedido por Gloom
@date Febrero, 2013
*/

#ifndef __Logic_FloatingMovement_H
#define __Logic_FloatingMovement_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	Este componente se encarga de hacer que los items del escenario
	floten (rotando y desplazandose verticalmente).

	@author Francisco Aisa García / Cedido por Gloom
	@date Febrero, 2013
	*/
	
	class CFloatingMovement : public IComponent {
		DEC_FACTORY(CFloatingMovement);
	
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		
		/** Constructor por defecto; en la clase base no hace nada. */
		CFloatingMovement() : IComponent(GetAltTypeIdOf(CFloatingMovement)), _currentOrbitalPos(0) {}
			
		// =======================================================================
		//                   METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Método llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		//________________________________________________________________________

		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>orbitalSpeed:</strong> Velocidad a la que el item sube y baja. </li>
			<li><strong>orbitalOffset:</strong> Distancia que el item recorre en el desplazamiento vertical. </li>
			<li><strong>orbitalRotationSpeed:</strong> Velocidad a la que rota el objeto. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	private:


		// =======================================================================
		//                           METODOS PRIVADOS
		// =======================================================================
		

		/**
		Calcula la siguiente posicion de la vertical del movimiento en funcion
		de la velocidad y el offset dados.

		@param position Posicion actual del item.
		@param msecs Milisegundos transcurridos desde el último tick.
		@return La nueva posicion del item usando una funcion seno para suavizar el movimiento.
		*/
		float estimateItemFloatingHeight(float height, unsigned int msecs);

		//________________________________________________________________________

		/**
		Calcula la nueva orientacion a partir de la velocidad de rotacion
		dada.

		@param msecs Milisegundos transcurridos desde el último tick.
		@return La nueva orientacion del item en radianes.
		*/
		float estimateItemRotation(unsigned int msecs);
		

		// =======================================================================
		//                           CAMPOS PRIVADOS
		// =======================================================================


		/** 
		Posicion actual en radianes que sirve para determinar cual es el siguiente
		valor sinusoidal  
		*/
		float _currentOrbitalPos;

		/** Distancia vertical que el item debe recorrer */
		float _orbitalOffset;

		/** Velocidad a la que el item se desplaza verticalmente */
		float _orbitalSpeed;

		/** Velocidad a la que el item debe rotar */
		float _orbitalRotationSpeed;

	}; // class CFloatingMovement

	REG_FACTORY(CFloatingMovement);

} // namespace Logic

#endif // __Logic_Life_H
