/**
@file Server.h

Contiene la declaración del servidor de física. 

@see Physics::CServer

@author 
@date
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "BaseSubsystems/Math.h"
#include "Logic/Entity/LogicalPosition.h"

// Predeclaración de tipos
namespace Logic {
	class CEntity;
	class CPhysicCharacter;
	class CPhysicEntity;
	class IPhysics;
};

namespace Physics {
	class Scene;
	class Actor;
};


// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	Servidor de física. Se encarga de gestionar las entidades físicas y simular su 
	comportamiento.
	*/
	class CServer 
	{
	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor físico.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor físico. Esta operación sólo es necesario realizarla
		una vez durante la inicialización de la aplicación. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor físico. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Realiza la simulación física y actualiza la posición de todas las 
		entidades físicas. 
		*/
		bool tick(unsigned int msecs);


		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena física. Se asume que sólo existirá una escena física, por lo que 
		sólo debe invocarse una vez.
		*/
		void createScene ();

		/**
		Destruye la escena física.
		*/
		void destroyScene ();


		//------------------------------
		// Gestión de entidades simples
		//------------------------------



		Physics::Actor* createActor(const Logic::TLogicalPosition &position, const float angularBox, 
			                                    const float height, bool trigger, const Logic::IPhysics *component); 


		void destroyActor(Physics::Actor* *actor);

		Logic::TLogicalPosition getActorLogicPosition(const Physics::Actor* actor);


		void moveActor(Physics::Actor *actor, const Logic::TLogicalPosition &position);

		void moveActor(Physics::Actor *actor, const float degress);

		void moveActor(Actor *actor, const float degrees, const float height);

	private:

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		// Instancia única de la clase.
		static CServer *_instance;

		// Escena física
		Scene *_scene;


	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H