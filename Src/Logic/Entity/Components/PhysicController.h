/**
@file PhysicController.h

Contiene la declaración del componente que se utiliza para representar jugadores y enemigos en
el mundo físico.

@see Logic::PhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Jose Luis López Sánchez
@date Febrero, 2013
*/

#ifndef __Logic_PhysicController_H
#define __Logic_PhysicController_H

#include "Physics.h"
#include "Logic/Entity/LogicalPosition.h"

// Predeclaración de tipos
/*namespace physx {
	class PxCapsuleController;
	struct PxControllersHit;
	struct PxControllerShapeHit;
};
*/
namespace Physics {
	class CServer;
};

// Los componentes se definen dentro del namespace Logica
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

	@author Antonio Sánchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CPhysicController : public IPhysics
	{
		DEC_FACTORY(CPhysicController);
	public:

		/**
		Constructor por defecto.
		*/
		CPhysicController();

		/**
		Destructor.
		*/
		virtual ~CPhysicController();
		
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
		*/
		virtual void tick(unsigned int msecs);

		/**
		Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		*/
		void  onTrigger (IPhysics *otherComponent, bool enter);

		/**
		Se invoca cuando se produce una colisión entre un character controller y una entidad física.
		*/
		//void onShapeHit (const physx::PxControllerShapeHit &hit);

		/**
		Se invoca cuando se produce una colisión entre dos character controllers.
		*/
	//	void onControllerHit (const physx::PxControllersHit &hit);

	private:

		/**
		Crea el character controller de PhysX que representa la entidad física a partir de la
		información del mapa.
		*/
	//	physx::PxCapsuleController* createController(const Map::CEntity *entityInfo);

		// Servidor de física
	//	Physics::CServer *_server;

		// Character controller que representa la entidad física en PhysX
	//	physx::PxCapsuleController *_controller;

		// Vector de desplazamiento recibido en el último mensaje de tipo AVATAR_WALK. 
		Vector3 _movement;

				/**
		Estructura de datos que contiene la posición lógica
		*/
		Logic::TLogicalPosition _logicalPosReceived;
		
		// Indica si el character controller esta apoyado sobre una superficie o cayendo.
		bool _falling;  //_entity->getHeight() también nos proporciona la misma info, si es 0 está en el suelo.

	}; // class CPhysicController

	REG_FACTORY(CPhysicController);

} // namespace Logic

#endif // __Logic_PhysicController_H
