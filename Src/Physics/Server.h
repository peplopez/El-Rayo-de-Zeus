/**
@file Server.h

Contiene la declaración del servidor de física. 

@see Physics::CServer

@author 
@date
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "Logic/Entity/LogicalPosition.h" // ƒ®§ Si se usan refs no se puede predeclarar?

// Predeclaración de tipos
namespace Physics {
	class CScene;
	class CActor;
	class IObserver;
}


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
		void tick(unsigned int msecs);




		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena física. Se asume que sólo existirá una escena física, por lo que 
		sólo debe invocarse una vez.
		*/
		CScene* createScene ();

		/**
		Destruye la escena física.
		*/
		void destroyScene ();


		//------------------------------
		// Gestión de entidades simples
		//------------------------------


		Physics::CActor* createActor(const Logic::TLogicalPosition &position, const float angularWidth, const float height, 
										bool isTrigger, IObserver *component); 

		void destroyActor(Physics::CActor* actor);

		
		Logic::TLogicalPosition& getActorLogicPosition(Physics::CActor* actor);


		//¿Por qué llamar al server y no directamente al actor->move?
		void moveActor(Physics::CActor *actor, const Logic::TLogicalPosition &position);

		// UNDONE ƒ®§ En principio no haría falta sobrecarga, si desde el CPhysicCharacter tenemos el  TLogical entero y esto es una simple llamada (no mensaje)
		//void moveActor(Physics::Actor *actor, const float degress);
		//void moveActor(Physics::CActor *actor, const float degrees, const float height);

	protected:


		// Instancia única de la clase.
		static CServer *_instance;

		// Escena física
		CScene *_scene;

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();


	}; // class CServer

} // namespace Physics

#endif // __Physics_Server_H