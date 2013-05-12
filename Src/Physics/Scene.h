/**
@file Scene.h

Contiene la declaración de la clase contenedora de los elementos
de una escena.

@see Physics::CScene

@author Emilio Santalla
@date Febrero 2013
*/

#ifndef __Physics_Scene_H
#define __Physics_Scene_H


#include <vector>
#include "BaseSubsystems\Math.h"

namespace Physics
{
	class CActor;
	class CActorTrigger;
	class CRigidBody;
}

namespace Physics 
{
	struct Manifold 
	{
	  CRigidBody* A;
	  CRigidBody* B;
	  float penetration;
	  Vector2 normal;

	  Manifold() : A(0), B(0), penetration(0), normal(Vector2::ZERO) {}
	};
	
	class CScene 
	{

	public:

		typedef std::vector<CActor*>		TCircleColliders, TAABBColliders;
		typedef std::vector<CActorTrigger*> TCircleTriggers, TAABBTriggers;

		/************
			ACTORS
		*************/
		bool addCircleActor(CActor *actor);
		bool addCircleActor(CActorTrigger* actor);

		bool addAABBActor(CActor *actor);
		bool addAABBActor(CActorTrigger* actor);

		void removeCircleActor(CActor* actor);
		void removeCircleActor(CActorTrigger* actor);

		void removeAABBActor(CActor* actor);
		void removeAABBActor(CActorTrigger* actor);

	protected:

		/**
		Clase amiga. Solo el servidor físico puede crear o liberar escenas físicas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**	Constructor de la clase.	*/
		CScene(const std::string& name) : _name(name), _gravityForce(Vector2(0, 0)) {};

		/**
		Destructor de la aplicación.
		*/
		~CScene();

		/**	Despierta la escena*/
		bool activate();

		/**	Duerme la escena*/
		bool deactivate();
		void tick(unsigned int msecs);


		/******************
			GET's & SET's
		********************/
		/**
		Devuelve el nombre de la escena.
		@return Nombre de la escena.
		*/
		const std::string& getName() {return _name;}


	private:

		/**	Nombre de la escena.*/
		std::string _name;

		TCircleColliders _circleColliders;
		TAABBColliders _AABBColliders;
		TCircleTriggers	_circleTriggers;
		TAABBTriggers _AABBTriggers;

		Vector2 _gravityForce;

		// Componentes de la simulacion
		void checkCollisions();
		void checkTriggers();


		/**
		Actualiza el estado de la escena cada ciclo.
		*/
		void simulate(unsigned int timeDiff);

		/**
		*/
		void updateCirclePos(CActor* circleCollider, unsigned int timeDiff);

		/**
		*/
		bool CheckCircleCircleCollision(Manifold &m);

		/**
		*/
		void ResolveCollision(Manifold &m);

		void PositionalCorrection( Manifold &m );
		

	}; // class CScene

} // namespace Physics

#endif 