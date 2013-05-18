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


namespace Physics
{
	class CActor;
	class CActorTrigger;
	class CContactListener;	
}


class b2World;
class OgreB2DebugDraw;

namespace Physics 
{

	
	class CScene 
	{

	public:

		typedef std::vector<CActor*>		TActors;

		/************
			ACTORS
		*************/
		bool add(CActor *actor);
		void remove(CActor* actor);

		b2World* getPhysicWorld() {return _world;}

	protected:

		/**
		Clase amiga. Solo el servidor físico puede crear o liberar escenas físicas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**	Constructor de la clase.	*/
		CScene(const std::string& name);

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

		TActors _actors;

		OgreB2DebugDraw* _debugDraw;

		b2World* _world;
		CContactListener* _worldListener;

		/**
		Actualiza el estado de la escena cada ciclo.
		*/
		void simulate(unsigned int timeDiff);
	

	}; // class CScene

} // namespace Physics

#endif 