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

		void deferredGhostBody(CActor* actor);
		void deferredUnghostBody(CActor* actor);

		void createGhostBodies();
		void deleteGhostBodies();

	protected:

		/**
		Clase amiga. Solo el servidor físico puede crear o liberar escenas físicas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**	
		Constructor de la clase.	
		*/
		CScene(const std::string& name);

		/**
		Destructor de la aplicación.
		*/
		~CScene();

		/**
		Inicializa la escena
		*/
		bool activate();

		/**
		Deinicializa la escena
		*/
		bool deactivate();

		/**
		Tick en float para b2D
		*/
		void tick(float timeStep);

		/**
		Activa/Desactiva pintado de debug
		*/
		void switchDebugDraw();


		/******************
			GET's & SET's
		********************/
		/**
		Devuelve el nombre de la escena.
		@return Nombre de la escena.
		*/
		const std::string& getName() {return _name;}


	private:

		/**	
		Nombre de la escena.
		*/
		std::string _name;

		TActors _actors;

		TActors _actorsToGhost;
		TActors _actorsToUnghost;

		OgreB2DebugDraw* _debugDraw;
		bool _debugDrawEnabled;

		b2World* _world;
		CContactListener* _worldListener;


		void CreateWorldEdges();

		bool _isInit;
		void _init();
		void _deinit();	


	

	}; // class CScene

} // namespace Physics

#endif 