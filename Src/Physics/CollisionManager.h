/**
@file CollisionManager.h

Contiene la declaración del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __PHYSICS_COLLISION_MANAGER_H_
#define __PHYSICS_COLLISION_MANAGER_H_

#include <PxSimulationEventCallback.h> 
#include <characterkinematic/PxController.h>

// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	Clase que gestiona las colisiones entre entidades físicas. Cuando se produce una
	colisión notifica a los componentes lógicos correspondientes. 

	 @ingroup physicGroup

	 @author Antonio Sánchez Ruiz-Granados
	 @date Noviembre, 2012
	*/
	class CCollisionManager : public physx::PxSimulationEventCallback,
							  public physx::PxUserControllerHitReport
						  
	{
	public:
		/**
		Constructor por defecto.
		*/
		CCollisionManager();

		/**
		Destructor.
		*/
		virtual ~CCollisionManager();

		/**
		Método invocado cuando se rompe un constraint (ver documentación PhysX).
		*/
		void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count); 
 
		/**
		Método invocado cuando se despiertan actores físicos (ver documentación PhysX).
		*/
		void onWake(physx::PxActor **actors, physx::PxU32 count);
  
		/**
		Método invocado cuando se duermen actores físicos (ver documentación PhysX).
		*/
		void onSleep(physx::PxActor **actors, physx::PxU32 count); 
   
		/**
		Método invocado cuando se produce un contacto entre dos shapes físicos 
		(ver documentación PhysX).
		*/
		void onContact(const physx::PxContactPairHeader &pairHeader, 
			           const physx::PxContactPair *pairs, physx::PxU32 nbPairs); 
  
		/**
		Método invocado cuando una entidad física entra o sale de un trigger 
		(ver documentación PhysX).
		*/
		void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

		/**
		Método invocado cuando un character controller golpea una shape 
		(ver documentación PhysX).
		*/ 
		void onShapeHit(const physx::PxControllerShapeHit &hit);
  
		/**
		Método invocado cuando un character controller golpea otro character controller 
		(ver documentación PhysX).
		*/ 
		void onControllerHit(const physx::PxControllersHit &hit); 
   
		/**
		Método invocado cuando un character controller golpea un obstáculo definido por 
		el usuario (ver documentación PhysX).
		*/ 
		void onObstacleHit(const physx::PxControllerObstacleHit &hit);
   
	}; // CCollisionManager

}; // namespace Physics

#endif // __PHYSICS_COLLISION_MANAGER_H_