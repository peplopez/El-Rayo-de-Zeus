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

		/************
			ACTORS
		*************/
		void addActor(CActor *actor);
		void addActor(CActorTrigger* actor);
		//bool addStaticActor(Physics::CStaticActor *actor);

		void removeActor(CActor* actor);
		void removeActor(CActorTrigger* actor);
		//void removeStaticActor(CStaticActor* actor);

	protected:

		/**
		Clase amiga. Solo el servidor físico puede crear o liberar escenas físicas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**	Constructor de la clase.	*/
		CScene(const std::string& name) : _name(name) {};

		/**
		Destructor de la aplicación.
		*/
		~CScene();

		/**	Despierta la escena*/
		bool activate();

		/**	Duerme la escena*/
		bool deactivate();

		void release();
		void tick(unsigned int);

		


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
		TActorVector _actors;
		TTriggerVector _triggers;

		// Componentes de la simulacion
		void checkCollisions();
		void checkTriggers();


		/**
		Actualiza el estado de la escena cada ciclo.
		*/
		void simulate();

		/**
		Corrige la posición de 2 actores que colisionan.
		*/
		void updateLogicPosition(Physics::CActor *actor1, Physics::CActor *actor2, float x, float y);		

		

	}; // class CScene

} // namespace Physics

#endif 