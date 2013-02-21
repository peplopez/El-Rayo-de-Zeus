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


#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"
#include "Physics/IObserver.h"

namespace Physics{
	class CServer;
	class CActor;
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
	@date Febrero, 2013
	*/
	class CPhysicCharacter : public IComponent, public Physics::IObserver
	{
		DEC_FACTORY(CPhysicCharacter);
	public:

		/**
		Constructor por defecto.
		*/
		CPhysicCharacter();

		/**
		Destructor.
		*/
		virtual ~CPhysicCharacter();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo AVATAR_WALK.
		*/
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

		// TODO que metodos va a necesitar un character?
		///**
		//Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		//*/
		//void  onTrigger (TEntityID otherEntity, bool enter);
		///**
		//Se invoca cuando se produce una colisión entre un character controller y una entidad física.
		//*/
		//void onShapeHit (const physx::PxControllerShapeHit &hit);

		///**
		//Se invoca cuando se produce una colisión entre dos character controllers.
		//*/
		//void onControllerHit (const physx::PxControllersHit &hit);

	private:

		// Servidor de física
		Physics::CServer *_server;

		// Character controller que representa la entidad física en PhysX
		//	physx::PxCapsuleController *_controller;
		Physics::CActor* _physicActor;

		///**
		//Crea el character controller de PhysX que representa la entidad física a partir de la
		//información del mapa.
		//*/
		//physx::PxCapsuleController* createController(const Map::CEntity *entityInfo);
		Physics::CActor* createActor(const Map::CEntity* entityInfo);

		// Estructura de desplazamiento acumulado durante los últimos mensajes de tipo AVATAR_WALK. 
		Logic::TLogicalPosition _movement;

		// Indica si el character controller esta apoyado sobre una superficie o cayendo.
		bool _falling;  // PeP: _entity->getHeight() también nos proporciona la misma info, si es 0 está en el suelo.

	}; // class CPhysicCharacter

	REG_FACTORY(CPhysicCharacter);

} // namespace Logic

#endif // __Logic_PhysicCharacter_H
