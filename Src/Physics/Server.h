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
		
	protected:

		/**
		Tipo para la estructura que guarda las diferentes escenas
		*/
		typedef std::map<std::string, CScene*> TScenes;


	public:


		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor físico.
		*/
		static CServer *getSingletonPtr() { assert(_instance && "Servidor fisico no inicializado"); return _instance; }
		
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
		void tick(float secs);

		/**
		*/
		void SwitchDebugDraw();


		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena física. Se asume que sólo existirá una escena física, por lo que 
		sólo debe invocarse una vez.*/
		CScene* createScene(const std::string &name);

		/**
		Elimina la escena pasada por parámetro de la tabla de escenas
		y la libera.

		@param scene Escena que se desea liberar
		*/
		void removeScene(CScene* scene);

		/**
		Elimina la escena de nombre especificado por parámetro de la 
		tabla de escenas y la libera.

		@param name Nombre de la escena que se quiere liberar.
		*/
		void removeScene(const std::string& name);


		/**
		Establece una escena como escena activa. En caso de que 
		hubiese otra escena activa este método la desactiva y establece
		la nueva.

		@param scene Escena que se desea poner como escena activa.
		*/
		void activate(CScene* scene);

		/**
		Establece una escena como escena activa. En caso de que 
		hubiese otra escena activa este método la desactiva y establece
		la nueva.

		@param name Nombre de la escena que se quiere poner como
		escena activa.
		*/
		void activate(const std::string& name);

		/*
		*/
		void deactivate(CScene* scene);



		//------------------------------
		// Gestión de entidades simples
		//------------------------------
		// UNDONE FRS Cableado directamente el CPhysic con los actors
		//Physics::CActor* createActor(const Logic::TLogicalPosition &position, const float angularWidth, const float height, 
		//								bool isTrigger, IObserver *component); 

		//void destroyActor(Physics::CActor* actor);

		//void moveActor(Physics::CActor *actor, float diffDegrees, float diffHeight, char diffRing, char diffBase);
		//
		//Logic::TLogicalPosition& getActorLogicPosition(Physics::CActor* actor);

	protected:

		// Instancia única de la clase.
		static CServer *_instance;

		/**
		Mapa de escenas. Se asocia una escena con su nombre.
		*/
		TScenes _scenes;


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