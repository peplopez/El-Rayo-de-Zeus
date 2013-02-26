/**
@file PhysicCharacter.h

Contiene la declaración del componente que se utiliza para representar jugadores y enemigos en
el mundo físico.

@see Logic::PhysicCharacter

@author Jose Luis López Sánchez
@date Febrero, 2013
*/

#ifndef __Logic_PhysicCharacter_H
#define __Logic_PhysicCharacter_H


#include "Logic/Entity/Components/Physic.h"
#include "Logic/Entity/LogicalPosition.h"


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
	@date Febrero, 2013
	*/
	class CPhysicCharacter : public CPhysic
	{
		DEC_FACTORY(CPhysicCharacter);
	
	public:

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

		
	}; // class CPhysicCharacter

	REG_FACTORY(CPhysicCharacter);

} // namespace Logic

#endif // __Logic_PhysicCharacter_H
