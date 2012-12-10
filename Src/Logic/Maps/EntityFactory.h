/**
@file EntityFactory.h

Contiene la declaración de la clase factoría de entidades
del juego.

@see Logic::CEntityFactory

@author David Llansó García, Marco Antonio Gómez Martín
@date Agosto, 2010
*/

#ifndef __Logic_EntityFactory_H
#define __Logic_EntityFactory_H

#include <map>
#include <string>
#include <list>

#include "EntityID.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class CEntity;
	class CBluePrint;
}

// Definición de la clase
namespace Logic 
{
	/**
	Factoría de CEntity que centraliza tanto la construcción
	como la destrucción de entidades del juego. Es un singleton
	de inicialización explícita. Hace uso de la factoría de 
	componentes para ensamblar las entidades a partir de las descripciones
	de las entidades (blueprints).

	La factoría carga la descripción de las entidades (componentes
	que las forman) del fichero blueprints.

	@ingroup logicGroup
	@ingroup mapGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CEntityFactory 
	{
	public:

		/**
		Inicializa la base de datos de la factoría.

		@return false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la base de datos. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Devuelve un puntero al único objeto de la clase.

		@return Factoría de entidades.
		*/
		static CEntityFactory *getSingletonPtr() {return _instance;}

		/**
		Carga un nuevo listado de entidades que se pueden crear mediante 
		componentes. El fichero que contiene la definición es muy simple.
		Cada línea corresponde a una entidad que viene definida por una
		serie de palabras separadas por espacios donde la primera
		palabra es el tipo de la entidad y el resto son los componentes 
		que conforman dicha entidad.
		<p>
		Se puede cargar más de un fichero con definiciones de entidades.
		Si los tipo de entidad se encuentran repetidos siempre prevalece
		el último añadido.

		@param filename Fichero con la descripción de las entidades.
		@return true si la carga se hizo correctamente.
		*/
		bool loadBluePrints(const std::string &filename);

		/**
		Descarga el listado de entidades creadas
		*/
		void unloadBluePrints();

		/**
		Crea una nueva entidad de juego en un mapa determinado a partir de
		su descripción en base a los componentes que necesita debido
		a su naturaleza según lo leído en el/los archivo/s blueprint.

		El método se encarga de asignar un nuevo identificador único a la
		entidad y añadirlo en el mapa del parámetro.

		@param entityInfo Descripción de la entidad; puede ser leída
		de un fichero de mapa o montada "al vuelo".
		@param map Mapa donde se crea la entidad.
		@return Entidad creada o NULL si no se pudo crear.

		@note Las entidades aquí creadas pueden eliminarse al final del 
		juego o bien utilizando deferredDeleteEntity.
		*/
		CEntity *createEntity(const Map::CEntity *entityInfo,
							  CMap *map);

		/**
		Destruye el CEntity pasado como parámetro. La destrucción
		es inmediata, por lo que el <em>invocante debe garantizar</em>
		que ese objeto no está actualmente en uso (no se está destruyendo
		a si mismo) o no afecta a otras entidades en acciones que estén 
		aun por realizar en el tick() actual de la lógica (si es que se
		está en medio de uno). De no ser así, el resultado será 
		indeterminado.

		@remarks Si no se está seguro es recomendable usar el método
		deferredDeleteEntity() que eliminará la entidad cuando se 
		termine el tick() de todo el mapa.

		@param entidad Entidad de juego que se borrará.
		*/
		void deleteEntity(CEntity *entity);

		/**
		Solicita la destrucción retrasada de la entidad que se pasa como 
		parámetro.
		<p>
		Este método se utiliza cuando se está aún con el juego activo y
		el objeto no puede eliminarse en este momento bien porque se 
		intenta autodestruir en medio de su propio tick() o porque
		acciones que aun estar por procesar en otras entidades puedan
		requerir de la presencia de esta entidad.
		<p>
		La factoría retrasa la destrucción del objeto hasta que el
		bucle de juego invoca a deleteDefferedObjects(). Será
		la propia factoría la que se preocupe en ese momento de:
		<ul>
			<li>Desactivar el objeto si pertenece a un mapa activo.</li>
			<li>Eliminarlo del mapa al que pertenece.</li>
			<li>Borrarlo definitivamente.</li>
		</ul>

		@param entity Entidad del juego que se quiere borrar.
		@note Si se elimina la factoría (se llama al destructor) _antes_
		de la invocación a deleteDefferedObjects() esos objetos
		_no_ son borrados por esta factoría como elementos diferidos,
		sino que se confía en que el mapa que aún contiene el objeto
		eliminará esa entidad.
		*/
		void deferredDeleteEntity(CEntity *entity);

		/**
		Método invocado por el bucle del juego para que la factoría
		elimine todos los objetos pendientes de ser borrados.
		*/
		void deleteDefferedEntities();

		/**
		Estructura que define una entidad blueprint.
		*/
		typedef struct
		{
			/**
			Nombre del tipo de entidad
			*/
			std::string type;

			/**
			Lista de nombres de componentes que componen la entidad.
			*/
			std::list<std::string> components;

		}TBluePrint;

	protected:

		/**
		Única instancia de la clase.
		*/
		static CEntityFactory *_instance;

		/** 
		Constructor de la clase, protegido, pues es un singleton.
		Registra al objeto como observer del cargador de mapas.
		*/
		CEntityFactory();

		/**
		Destructor protegido, por ser singleton.
		*/
		~CEntityFactory();

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

		/** 
		Ensambla una nueva entidad de juego a partir de su tipo de entidad.
		En base al tipo de entidad se crearán y añadirán a la entidad las 
		instancias de los componentes que necesita según lo leído en el/los 
		archivo/s blueprint.
		Es un procedimiento auxiliar utilizado por createEntity.

		@param type Tipo de la entidad que se quiere crear.
		@return La entidad creada o NULL si no se pudo crear.
		*/
		CEntity *assembleEntity(const std::string &type);

		/**
		Tipo lista de CEntity donde guardaremos los pendientes de borrar.
		*/
		typedef std::list<Logic::CEntity*> TEntityList;

		/**
		Lista de objetos pendientes de borrar.
		*/
		TEntityList _pendingEntities;

		/**
		Tipo tabla para almacenar entidades blueprint por nombre.
		*/
		typedef std::map<std::string,TBluePrint> TBluePrintMap;

		/**
		Tabla donde se almacenan las entidades blueprint por nombre.
		*/
		TBluePrintMap _bluePrints;

	}; // CEntityFactory

} // namespace Logic

#endif // __Logic_EntityFactory_H
