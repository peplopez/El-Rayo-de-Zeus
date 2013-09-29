/**
@file Arrow.h

Contiene la declaración del componente que se utiliza para representar flechas en el múndo físico
@see Logic::Arrow

@author Jose Luis López Sánchez
@date September, 2013
*/

#ifndef __Logic_Arrow_H
#define __Logic_Arrow_H


#include "Logic/Entity/Components/Physics.h"
#include "Logic/Entity/LogicalPosition.h"

namespace Physics
{
	class CActor;
	class CLife;
}

// Los componentes se definen dentro del namespace Logica
// TODO corregir comentarios
namespace Logic 
{
	/**
	Componente que se utiliza para representar jugadores y enemigos en el mundo físico usando 
	character controllers.
	<p>
	Este componente recibe mensajes de tipo AVATAR_MOVE indicando el movimiento que se quiere 
	realizar. A continuación se le indica al motor de física el movimiento deseado, y en el
	siguiente tick se actualiza la posición de la entidad lógica. Debemos tener en cuenta que 
	la posición final la determina el motor de física, ya que durante el movimiento podemos 
	colisionar con otra entidades, resbalar sobre paredes, etc.
	<p>
	Este componente también simula el efecto de la gravedad sobre el jugador (los controllers
	de PhysX no se ven afectados por la gravedad).
	
    @ingroup logicGroup

	@author Jose Luis López Sánchez & ƒ®§
	@date September, 2013
	*/
	class CArrow : public CPhysics
	{
		DEC_FACTORY(CArrow);
	
	public:

		CArrow() : CPhysics(GetAltTypeIdOf(CArrow)), _diffDegrees(0), _diffHeight(0), _diffBase(0){}

			/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
				/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**Este componente sólo acepta mensajes de tipo AVATAR_WALK.*/
		virtual bool accept(const CMessage *message);
		
		/**
		Cuando se recibe un mensaje de tipo AVATAR_WALK, se almacena su vector de 
		desplazamiento para mover el character controller en el próximo tick.
		De esta forma, si en un ciclo se reciben varios mensaje de tipo AVATAR_WALK 
		sólo tendrá efecto el último.
		*/
		virtual void process(CMessage *message);

		/**
		Este método se invoca en cada ciclo de la simulación y hace lo siguiente:
		<ul>
		<li> Actualiza la posición de la entidad lógica usando la información porporcionada por
		     el motor de física. </li>
		<li> Mueve el character controller de acuerdo al último mensaje AVATAR_WALK recibido. </li>
		</ul>
		<p>
		Los character controllers no tienen orientación, sólo posición
		*/
		virtual void tick(unsigned int msecs);

		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		virtual void onTrigger(IObserver*, bool);
		virtual void onCollision(IObserver* other);

		void DestroyArrow();
	protected:

		float _diffDegrees;
		float _diffHeight;
		char _diffBase;

	}; // class CArrow

	REG_FACTORY(CArrow);

} // namespace Logic

#endif // __Logic_Arrow_H
