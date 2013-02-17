/**
@file Server.h

Contiene la declaración del servidor de física. 

@see Physics::CServer

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "BaseSubsystems/Math.h"

// Predeclaración de tipos
namespace Logic {
	class CEntity;
	class CPhysicController;
	class CPhysicEntity;
	class IPhysics;
};

namespace Physics {
	class CCollisionManager;
	class CErrorManager;
};

namespace physx {
	class PxActor;
	class PxAllocatorCallback;
	class PxCapsuleController;
	class PxController;
	class PxControllerManager;
	class PxCooking;
	class PxDefaultAllocator;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	class PxMaterial;
	class PxPhysics;
	class PxProfileZoneManager;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxRigidStatic;
	class PxScene;

	namespace pxtask {
		class CudaContextManager;
	};

	namespace debugger {
		namespace comm {
			class PvdConnection;
		};
	};
};



// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	Servidor de física. Se encarga de gestionar las entidades físicas y simular su 
	comportamiento. Cumple varias finalidades:
	
	<ul>
	<li> Proporciona una interfaz común con el resto de servidores de la aplicación (gráficos, GUI, etc). </li> 
	<li> Aísla el resto de la aplicación del motor de física concreto que usamos (hasta cierto punto).
	<li> Realiza las conversiones entre los tipos lógicos y los de PhysX (vectores, matrices, etc).
	</ul>
	<p>
	Las entidades físicas de PhysX y los componentes lógicos que proporcionan la representación física de la entidad
	están relacionados de dos formas:
	<ul>
	<li> Los componentes lógicos almacenan internamente sus actores de PhysX asociados. </li>
	<li> Usamos el atributo userData de los actores de PhysX para almacenar el componente lógico asociado. </li>
	</ul>
	De esta forma, la capa lógica pueden mover los objetos físicos (actores cinemáticos, character controllers, 
	aplicar fuerzas a actores dinámicos, etc) y la capa de física puede comunicar las colisiones a los componentes
	lógicos.
	<p>
	Es aconsejable utilizar los métodos del servidor para cambiar / recuperar las posiciones de las
	entidades físicas, ya que el servidor realiza las conversiones necesarias entre sistemas de coordenadas.
	Aunque la lógica y PhysX usan el mismo tipo de sistema de coordenadas, PhysX asume que el origen está 
	en medio del objeto mientras que la lógica asume que el origen está a los pies del objeto. Usando los 
	métodos que	proporciona el servidor, la lógica no tiene que preocuparse de estas conversiones.
	<p>
	Esta clase está implementada como un Singleton de inicialización explícita: es necesario 
	invocar al método Init() al principio de la aplicación y al método Release() al final.

	@ingroup physicGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Noviembre, 2012
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

		@param secs Millisegundos transcurridos desde la última actualización.
		@return Valor booleano indicando si todo fue bien.
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

		/**
		Establece si debe haber colisiones entre dos grupos de colisión. Al crear los objetos
		físicos se establece el grupo de colisión al que pertenecen. Usando este método es
		posible activar / desactivar las colisiones entre grupos.

		@param group1 Primer grupo de colisión.
		@param group2 Segundo grupo de colisión
		@param enable Activar o desactivar las colisiones entre los dos grupos anteriores.
		*/
		void setGroupCollisions(int group1, int group2, bool enable);


		//------------------------------
		// Gestión de entidades simples
		//------------------------------

		/**
		Crea un plano estático en la escena.

		@param point Punto del plano.
		@param normal Vector normal al plano.
		@param group Grupo de colisión.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidStatic* createPlane(const Vector3 &point, const Vector3 &normal, 
			                              int group, const Logic::IPhysics *component);

		/**
		Crea una caja estática en la escena.

		@param position Posición de la caja en coordenadas lógicas (el origen de coordenadas 
		       está en el centro de la cara inferior del cubo).
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de 
		       la caja medirán dimensions*2.
		@param trigger Indica si la entidad física representa un trigger.
		@param group Grupo de colisión.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidStatic* createStaticBox(const Vector3 &position, const Vector3 &dimensions, 
			                                  bool trigger, int group, const Logic::IPhysics *component); 

		/**
		Crea una caja dinámica en la escena.

		@param position Posición de la caja en coordenadas lógicas (el origen de coordenadas 
		       está en el centro de la cara inferior del cubo).
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de 
		       la caja medirán dimensions*2.
		@param mass Masa distribuida uniformemente en el volumen de la entidad.
		@param kinematic Indica si la entidad es cinemática.
		@param trigger Indica si la entidad física representa un trigger.
		@param group Grupo de colisión.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidDynamic* createDynamicBox(const Vector3 &position, const Vector3 &dimensions, 
			                                    float mass, bool kinematic, bool trigger, int group, 
												const Logic::IPhysics *component); 

		/**
		Crea una entidad física en la escena a partir de un fichero RepX exportado con el 
		plug-in de PhysX para 3ds Max. Asume que el fichero contiene únicamente la 
		descripción de un actor.

		@param file Fichero generado con el plug-in de PhysX.
		@param group Grupo de colisión en el que debe ser incluida la entidad.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidActor* createFromFile(const std::string &file, int group, 
			                                const Logic::IPhysics *component);

		/**
		Elimina una entidad física de la escena y libera los recursos que tenga asociados.
		NO DEBE USARSE CON CHARACTER CONTROLLERS.

		@param actor Actor físico asociado a la entidad.
		 */
		void destroyActor(physx::PxActor *actor);

		/**
		Devuelve la posición y rotación de una entidad física.

		@param actor Actor físico del que se desea conocer la posición y orientación.
		@return Matriz 4x4 con la posición y orientación de la entidad.
		 */
		Matrix4 getActorTransform(const physx::PxRigidActor *actor);

		/**
		Mueve un actor cinemático.

		@param actor Actor cinemático que se desea mover.
		@param transform Nueva posición y orientación. 
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform);

		/**
		Aplica una traslación a un actor cinemático.

		@param actor Actor cinemático que se desea mover.
		@param displ Desplazamiento a realizar
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, const Vector3 &displ);

		/**
		Indica si un actor dinámico es cinemático.
		 */
		bool isKinematic(const physx::PxRigidDynamic *actor);
		

		//----------------------------------
		// Gestión de character controllers
		//----------------------------------

		/**
		Crea un controller de tipo capsula en la escena.

		@param position Posición del controller en coordenadas lógicas (el origen de coordenadas 
		       está en los pies del controller).
		@param radius Radio de la cápsula.
		@param height Altura de la cápsula.
		@param component Componente lógico asociado a la entidad física.
		@return character controller creado
		*/
		physx::PxCapsuleController* createCapsuleController(const Vector3 &position, float radius, 
															float height, 
															const Logic::CPhysicController *component);


		/**
		Mueve el controller por la escena.

		@param controller Controller que se desea mover.
		@param movement Movimiento que se quiere realizar.
		@param msecs Millisegundos transcurridos desde la última actualización.
		@return Flags de colisión, un conjunto de physx::PxControllerFlag.
		*/
		unsigned CServer::moveController(physx::PxController *controller, const Vector3 &movement, 
			                             unsigned int msecs);

		/**
		Devuelve la posición del controller.

		@param controller Controller del que se desea conocer su posición.
		@return posición del controller en coordenadas lógicas.
		*/
		Vector3 CServer::getControllerPosition(const physx::PxCapsuleController *controller);

		
		//----------------------------------
		// Consultas 
		//----------------------------------

		/**
		 Lanza un rayo y devuelve la primera entidad lógica contra la que interseca. Si el rayo
		 no choca contra ninguna entidad devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia máxima de la primera intersección.
		 @return Primera entidad lógica alcanzada o NULL.
		 */
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist) const; 

		/**
		 Lanza un rayo y devuelve la primera entidad lógica contra la que interseca que pertenezca 
		 al grupo de colisión indicado. Si el rayo no choca contra ninguna entidad de ese grupo
		 devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia máxima de la primera intersección.
		 @param group Grupo de colisión de la entidad buscada.
		 @return Primera entidad lógica alcanzada de ese grupo o NULL.
		 */
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist, int group) const; 

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

		// Gestor de errores
		physx::PxErrorCallback *_errorManager;

		// Gestor de memoria
		physx::PxAllocatorCallback *_allocator;

		// Distintos atributos relacionados con PhysX
		physx::PxFoundation *_foundation;
		physx::PxProfileZoneManager *_profileZoneManager;
		physx::PxDefaultCpuDispatcher *_cpuDispatcher;
		physx::pxtask::CudaContextManager *_cudaContextManager;
		physx::debugger::comm::PvdConnection *_pvdConnection;
		physx::PxCooking *_cooking;

		// SDK de PhysX
		physx::PxPhysics *_physics;

		// Escena física
		physx::PxScene *_scene;

		// Gestor de controller managers
		physx::PxControllerManager* _controllerManager;

		// Material que se aplica por defecto a las nuevas entidades físicas
		physx::PxMaterial *_defaultMaterial;

		// Gestion de colisiones
		CCollisionManager *_collisionManager;

	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H
