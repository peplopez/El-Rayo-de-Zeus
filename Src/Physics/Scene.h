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

//HACK TOCHO ?

#include <vector>

#include "Physics/Actor.h"
#include "Physics/ActorTrigger.h"


namespace Physics 
{

	class CScene 
	{
	public:

		void addActor(CActorTrigger* actor);
		void addActor(CActor *actor);

		//bool addStaticActor(Physics::CStaticActor *actor);

		void removeActor(CActor* actor);
		void removeActor(CActorTrigger* actor);

		//void removeStaticActor(CStaticActor* actor);

		void release();

	protected:

		/**
		Clase amiga. Solo el servidor físico puede crear o liberar escenas físicas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**
		Constructor de la clase.
		*/
		CScene();

		/**
		Destructor de la aplicación.
		*/
		~CScene();

		/**
		Despierta la escena
		*/
		bool activate();

		/**
		Duerme la escena
		*/
		bool deactivate();
		
		/**
		Actualiza el estado de la escena cada ciclo.
		*/
		void simulate();
		
	private:

		TActorVector _actors;
		TTriggerVector _triggers;

		// Componentes de la simulacion
		void checkCollisions();
		void checkTriggers();

		/**
		Corrige la posición de 2 actores que colisionan.
		*/
		void updateLogicPosition(Physics::CActor *actor1, Physics::CActor *actor2, float x, float y);		

		

	}; // class CScene

} // namespace Physics

#endif 