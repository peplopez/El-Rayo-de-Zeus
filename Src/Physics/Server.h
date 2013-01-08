/**
@file Server.h

Contiene la declaración del servidor de física. 

@see Physics::CServer

@author Antonio Sánchez Ruiz-Granados
@date Octubre, 2010
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include <string>

#include "BaseSubsystems/Math.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Physics {
	class CCollisionMng;
};

namespace Logic {
	class CPhysicEntity;
	class CEntity;
};

class CPhysicScene;
class IPhysicModel;
class CPhysicModelSimple;
class CPhysicModelGeneric;
class CPhysicModelCharacter;
class IPhysicObj;
class CPhysicObj;
class CPhysicObjCharacter;

/**
Namespace que contiene las clases nuevas relacionadas con la parte física. 
<p>
En este proyecto también existe código heredado que está fuera de este namespace.
El nombre de esas clases comienza por IPhysic o CPhysic.

@author Antonio Sánchez Ruiz-Granados
@date Octubre, 2010
*/
namespace Physics {

    enum TPhysicMode
    {
        PM_STATIC = 0,
        PM_KINEMATIC,
        PM_DYNAMIC
    };
	
	/**
	Clase central del subistema físico. Se encarga de gestionar las entidades físicas
	y simular su comportamiento. Esta clase sirve de wrapper sobre las clases de física
	y cumple varias finalidades:
	
	<ul>
	<li> Proporciona una interfaz común con el resto de servidores de la aplicación (gráficos, GUI, etc). </li> 
	<li> Simplifica la gestión de la escena. PhysX permite trabajar con varias escenas simulatáneamente, 
	pero nuestro juego sólo va a utilizar una.
	<li> Realiza las conversiones entre los tipos lógicos y los de PhysX (vectores, matrices, etc).
	</ul>
	
	Esta clase está implementada como un Singleton de inicialización explícita; se debe 
	llamar a su método Init(), para que la instancia esté creada. Al final de 
	la aplicación, habrá que llamar a Release().
	<p>

	@ingroup physicsGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Octubre, 2010
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
		Función invocada en cada vuelta para actualización.

		@param secs Segundos desde que se reenderizó el último frame.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(float secs);


		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena física. Se asume que sólo existirá una escena física.
		 */
		void createScene ();

		/**
		Destruye la escena física.
		*/
		void destroyScene ();

		/**
		Devuelve la escena física.

		@return Escena física.
		*/
		CPhysicScene* getScene () const;


		//----------------------
		// Creación de modelos
		//----------------------

		/**
		Crea un modelo físico para objetos simples.

		@return modelo con valores por defecto.
		*/
		CPhysicModelSimple* createModelSimple();

		/**
		Crea un modelo físico para objetos genéricos (objetos compuestos de 
		otros objetos) a partir de un fichero que contiene su descripción.

		@param file Ruta del fichero que describe el modelo.
		@param modelo con los valores leidos del fichero.
		*/
		CPhysicModelGeneric* createModelGeneric(const std::string &file);

		/**
		Crea el modelo físico de un character controller con forma de cápsula.

		@param radius Radio de la cápsula.
		@param height Altura de la cápsula.
		@return modelo fisico creado.
		*/
		CPhysicModelCharacter* createModelCapsuleCharacter(float radius, float height);

		
		//----------------------------
		// Gestión de modelos simples
		//----------------------------

		/**
		Añade un volumen de colisión (shape) con forma de cápsula a un modelo simple.

		@param model Modelo simple al que vamos a añadir la shape.
		@param radius Radio de la cápsula.
		@param height Altura de la capsula.
		@param group Grupo de colisión al que pertenecerá la shape.
		*/
		void createCapsuleShape(CPhysicModelSimple *model, float radius, float height, int group);

		/**
		Añade un volumen de colisión (shape) con forma de caja a un modelo simple.

		@param model Modelo simple al que vamos a añadir la shape.
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de la caja
				resultante medirán dimensions*2.
		@param group Grupo de colisión al que pertenecerá la shape.
		*/
		void createBoxShape(CPhysicModelSimple *model, const Vector3 &dimensions, int group);

		/**
		Añade un volumen de colisión (shape) con forma de plano a un modelo simple. El plano
		pasa por el origen de coordenadas.

		@param model Modelo simple al que vamos a añadir la shape.
		@param normal Vector normal al plano.
		@param group Grupo de colisión al que pertenecerá la shape.
		*/
		void createPlaneShape(CPhysicModelSimple *model, const Vector3 &normal, int group);

		/**
		Marca / desmarca un volumen de colisión (shape) como trigger.

		@param model Modelo de colisión simple al que pertenece la shape.
		@param idxShape Índice de la shape que queremos marcar / desmarcar como trigger.
		@param isTrigger Indica si la shape es o no un trigger.
		*/
		void setTrigger(CPhysicModelSimple *model, int idxShape, bool isTrigger);

		/**
		Establece la masa de un modelo.

		@param model Modelo físico.
		@param mass Masa asociada al modelo físico.
		*/
		void setMass(CPhysicModelSimple *model, float mass);

		
		//----------------------------
		// Gestión de objetos físicos
		//----------------------------

		/**
		Crea un nuevo objeto físico y lo activa.

		@param component Componente lógico asociado a la nueva entidad física.
		@param mode Indica si el objeto es estático, dinámico o kinemático.
		@param position Posicion inicial del objeto.
		@param orientation Orientación inicial del objeto.
		@param model Modelo que describe el tipo de objeto físico a crear.
		@return Nuevo objeto físico creado o NULL si hubo algún problema.
		*/
		IPhysicObj* createObject(Logic::CPhysicEntity *component, TPhysicMode mode, 
			                     const Vector3 &position, const Matrix3 &orientation, 
								 const IPhysicModel *model); 

		/**
		Destruye un objeto físico.

		@param obj Objeto físico a eliminar.
		*/
		void destroyObject(IPhysicObj *obj);

		/**
		Devuelve la posición de un objeto físico.

		@return Posición en coordenadas del mundo.
		*/
		Vector3 getPosition(const IPhysicObj *obj);

		/**
		Devuelve la orientación de un objeto físico. Puesto que los objetos físicos genéricos
		pueden estar compuesto spor varias partes es necesario indicar la parte de la que 
		queremos obtener la orientación. Nótese que los objetos simples sólo tienen una parte 
		por lo que idxPart siempre deberá ser 0 cuando trabajemos con ellos.

		@param obj Objeto físico del que se desea conocer la orientación.
		@param idxPart Parte del objeto físico que se usa para recuperar su orientación.
		*/
		Matrix3 getOrientation(const IPhysicObj *obj, int idxPart);
		
		
		
		//------------------------------------
		// Movimiento de entidades kinemáticas
		//------------------------------------

		/**
		Mueve un objeto físico kinemático.

		@param obj Objeto físico kinemático.
		@param pos Nueva posición del objeto físico.
		*/
		void move(IPhysicObj *obj, const Vector3 &pos);

		/**
		Mueve un objeto físico kinemático.

		@param obj Objeto físico kinemático.
		@param rot Nueva rotación del objeto físico.
		*/
		void move(IPhysicObj *obj, const Matrix3 &rot);
		
		/**
		Mueve un objeto físico kinemático.

		@param obj Objeto físico kinemático.
		@param transf Nueva posición y rotación del objeto físico.
		*/
		void move(IPhysicObj *obj, const Matrix4 &transf);

		

		//---------------------------------
		// Gestión de character controllers
		//---------------------------------

		/**
		Mueve un character controller. El controller puede no terminar en la posición
		deseada si colisiona con otras entidades durante el movimiento.

		@param obj Objeto físico que representa al character controller.
		@param displ Vector de desplazamiento que representa el movimiento a realizar.
		@return Posible combinación de los flags NxControllerFlag de PhysX indicando
				colisiones.
		*/
		unsigned moveCharacter(CPhysicObjCharacter *obj, const Vector3 &displ);

		/**
		Teletransporta un character controller a una posición. Este método no comprueba
		si el controller "cabe" en la posición de destino o si terminará atravesando
		otra entidad ya existente, por lo que debe usarse con cuidado.
		
		@param obj Objeto físico que representa al character controller.
		@param position Nueva posición del character controller en coordenadas del mundo.
		*/
		void setPosition(CPhysicObjCharacter *obj, const Vector3 &position);


		//--------------------------
		// Lanzamiento de rayos
		//--------------------------

		/**
		 Lanza un rayo y devuelve la primera entidad lógica contra la que interseca. Si el rayo
		 no choca contra ninguna entidad devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia máxima de la primera intersección
		 @return Primera entidad lógica alcanzada o NULL.
		 */
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist) const; 


	private:

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		/**
		Instancia única de la clase.
		*/
		static CServer *_instance;

		/**
		Gestor de colisiones.
		*/
		CCollisionMng *_collisionMng;

		/**
		Modelos físicos. En realidad debería haber un gestor de recursos en el que registrar
		los distintos modelos y poder reutilizarlos. Cada "tipo" de entidad tendría un modelo 
		físico asociado, y para crear entidades concretas de ese tipo sólo habría que instanciar 
		su tipo físico.	Como no tenemos fichero de tipos de entidades (o arquetipos), por ahora 
		no tiene mucho sentido tener un gestor de modelos físicos. Simplemente vamos a almacenar 
		los modelos en una lista para liberarlos al final.
		*/
		std::list<IPhysicModel*> models;

	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H
