/**
@file Entity.h

Contiene la declaración de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Logic_Entity_H
#define __Logic_Entity_H

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/Rtti.h"

#include "Logic/Maps/EntityID.h"
#include "LogicalPosition.h"

#include <list>
#include <string>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class IComponent;
	class CEntityFactory;
	class CMessage;
}

// Declaración de la clase
namespace Logic 
{
	/**
	Clase que representa una entidad en el entorno virtual. Las entidades
	son meros contenedores de componentes, y su funcionamiento depende
	de cuáles tenga. Además, la clase contiene una serie de atributos que
	pueden ser accedidos desde los componentes (Id, nombre, etc.).
	<p>
	Tras la creación de la entidad, donde se inicializan los valores de la
	entidad a su valor por defecto, se invocará a su método spawn() en el
	que se inicializarán los atributos de la entidad con los valores leidos 
	del fichero del mapa. Tras esto, en algún momento, cuando se active
	el mapa se llamará al método activate() de la entidad. En este momento
	los componentes harán visibles los componentes gráficos y demás ya que 
	apartir de ahí es cuando la entidad empieza su función siendo actualizada 
	por el tick() en cada vuelta de bucle.

    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CEntity 
	{
	protected:

		/**
		Clase amiga que crea y destruye objetos de la clase.
		*/
		friend class CEntityFactory;

		/**
		Constructor protegido de la clase (para crear CEntity debe
		utilizarse la factoría CEntityFactory). El constructor
		no hace más que poner el identificador único de la entidad
		, pues la inicialización efectiva se hace en el método spawn().
		
		@param entityID Identificador único de la entidad.
		*/
		CEntity(TEntityID entityID);

		/**
		Destructor de la clase. Es un método protegido pues para
		eliminar CEntity debe utilizarse la factoría
		CEntityFactory. El destructor invoca al destructor de
		cada componente.
		Cuando el destructor es invocado, éste _ya ha sido desenganchado
		del mapa al que perteneció_, por lo que los destructores de los
		componentes no pueden utilizar el mapa.
		*/
		~CEntity();

		/**
		Inicialización del objeto Logic, utilizando la información extraída de
		la entidad leída del mapa (Map::CEntity). Avisará a los componentes
		de la entidad para que se inicialicen.

		@param map Mapa Logic en el que se registrará la entidad.
		@param entity Información de construcción de la entidad leída del
		fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		bool spawn(CMap *map, const Map::CEntity *entity);

	public:
			
		/**
		Activa la entidad. Esto significa que el mapa ha sido activado.
		<p>
		El método llama al activate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true si todo fue bien.
		*/
		bool activate();

		/**
		Desactiva la entidad. Esto significa que el mapa ha sido desactivado.
		<p>
		El método llama al deactivate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Llamará a los métodos tick() de todos sus componentes.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Método que añade un nuevo componente a la lista de la entidad.

		@param component Componente a añadir.
		*/
		void addComponent(IComponent* component);

		/**
		Método que quita un componente de la lista.

		El componente es eliminado de la lista de componentes de la
		entidad, pero <b>no</b> es desactivado ni eliminado (con delete);
		la responsabilidad de esa tarea se deja al invocante.

		@param component Componente a borrar.
		@return true si se borró el componente (false si el componente
		no estaba en el objeto).
		*/
		bool removeComponent(IComponent* component);
		
		/**
		Método que destruye todos los componentes de una entidad.
		*/
		void destroyAllComponents();

		/**
		Recibe un mensaje que envia a todos los componentes de la lista menos 
		al componente que lo envia, si éste se especifica en el segundo campo.

		@param message Mensaje a enviar.
		@param emitter Componente emisor, si lo hay. No se le enviará el mensaje.
		@return true si al menos un componente aceptó el mensaje
		*/
		bool emitMessage (CMessage *message, IComponent* emitter = 0);

		/**
		Devuelve el identificador único de la entidad.

		@return Identificador.
		*/
		Logic::TEntityID getEntityID() const { return _entityID; }

		/**
		Método que indica si la entidad es o no el jugador.
		Seguro que hay formas mejores desde el punto de vista de
		diseño de hacerlo, pero esta es la más rápida: la entidad 
		con la descripción de la entidad tiene esta descripción que
		establece en el spawn().
		
		@return true si la entidad es el jugador.
		*/
		bool isPlayer() { return _isPlayer; }

		/**
		Método que indica si la entidad es o no el jugador.
		Seguro que hay formas mejores desde el punto de vista de
		diseño de hacerlo, pero esta es la más rápida: la entidad 
		con la descripción de la entidad tiene esta descripción que
		establece en el spawn().
		
		@return true si la entidad es el jugador.
		*/
		void setIsPlayer(bool isPlayer);

		/**
		Devuelve el mapa donde está la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		CMap *getMap() { return _map; }

		/**
		Devuelve el mapa donde está la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		const CMap *getMap() const { return _map; }

		/**
		Devuelve el nombre de la entidad.

		@return Nombre de la entidad.
		*/
		const std::string &getName() const { return _name; }

		/**
		Devuelve el nombre gráfico de la entidad (name + ID)

		@return Nombre de la entidad gráfica asociada
		*/
		const std::string &getGraphicalName() const { return _graphicalName; };

		/**
		Devuelve la escala inicial de la entidad.

		@return Escala inicial de la entidad.
		*/
		const float &getInitialScale() const { return _initialScale; }		

		/**
		Devuelve el material de la entidad.

		@return Material de la entidad.
		*/
		const std::string &getInitialMaterial() const { return _initialMaterial; }

		/**
		Devuelve el tipo de la entidad. Este atributo es leido de
		la entidad del mapa en spawn().

		@return Tipo de la entidad.
		*/
		const std::string &getType() const { return _type; }

		/**
		Add PEP
		Establece la estructura de posición lógica asociada a las entidades que hagan uso de ella.
		
		@param pos Nueva posición lógica de la entidad.
		*/		
		void setLogicalPosition(const Logic::CLogicalPosition *pos);

		/**
		Add PEP
		Recupera la estructura de posición lógica de una entidad
		
		@return posición lógica
		*/		
		Logic::CLogicalPosition* getLogicalPosition()  {return _pos;}

		/**
		Establece la matriz de transformación de la entidad. Avisa a los 
		componentes del cambio.

		@param transform Nueva matriz de transformación de la entidad.
		*/
		void setTransform(const Matrix4& transform);

		/**
		Devuelve la metriz de transformación de la entidad.
		<p>
		La posición es inicialmente leída del mapa (si no aparece,
		se colocará a (0, 0, 0)) y la orientación es también inicialmente 
		leída del mapa, como un simple viraje (si no aparece, se colocará 
		a 0). Obviamente, pueden cambiar con el tiempo.

		@return Matriz de transformación de la entidad en el entorno.
		*/
		Matrix4 getTransform() const { return _transform; }

		/**
		Establece la posición de la entidad. Avisa a los componentes
		del cambio.

		@param position Nueva posición.
		*/
		void setPosition(const Vector3 &position);

		/**
		Devuelve la posición de la entidad.
		<p>
		La posición es inicialmente leída del mapa (si no aparece,
		se colocará a (0, 0, 0)), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Posición de la entidad en el entorno.
		*/
		Vector3 getPosition() const { return _transform.getTrans(); }

		
		/**
		Devuelve la Y del anillo de la entidad.
		
		@return Ring de la entidad en el entorno.
		*/		
		const float getY(const unsigned short base, const Logic::LogicalPosition::Ring ring);
		// Pablo
		const float getYJump(const unsigned short base, const Logic::LogicalPosition::Ring ring);

		
		/**
		Establece la orientación de la entidad. Avisa a los componentes
		del cambio.

		@param pos Nueva orientación.
		*/
		void setOrientation(const Matrix3& orientation);

			/**
		Cambia la orientación de la entidad.

		@param orientation Referencia a la matriz de rotación con la que debe 
		orientarse la entidad.
		*/
		void setOrientation(const Quaternion &quat);

		/**
		Devuelve la matriz de rotación de la entidad.
		<p>
		La orientación es inicialmente leída del mapa como un simple 
		viraje (si no aparece, se colocará a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Orientación en el entorno.
		*/
		 Matrix3 getOrientation() const;

		/**
		Establece el viraje de la entidad. Avisa a los componentes
		del cambio.

		@param yaw Nuevo viraje.
		*/
		void setYaw(float yaw);

		void setPitch(float pitch);
		
		void setRoll(float roll);

		void setPitchYaw(float pitch,float yaw);


		void roll(float roll); 

		void pitch(float pitch); 


		/**
		Vira la entidad. Avisa a los componentes del cambio.

		@param yaw Viraje a aplicar.
		*/
		void yaw(float yaw);

		/**
		Devuelve el viraje de la entidad.
		<p>
		La orientación es inicialmente leída del mapa como un simple 
		viraje (si no aparece, se colocará a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Viraje en el entorno.
		*/
		const float getYaw() const { return Math::getYaw(_transform); }

		/**
		Indica si la entidad se encuentra activa.

		@return true si la entidad está activa.
		*/
		bool isActivated() {return _activated;}

		const Vector3 fromLogicalToCartesian(const float grados,const float altura, const unsigned short base, const Logic::LogicalPosition::Ring ring);
		const Vector3 fromLogicalToCartesian(const CLogicalPosition* pos);

		/**
		Add - ESC
		Función con el tipo de retorno parametrizado,
		permite optener una referencia al componente del tipo indicado.

		Hace uso de de un mecanismo RTTI propio para ello
		*/

		template <typename T>
		T* getComponent()
		{
			IComponent* component = _compoTable[ TAltTypeIdGenerator<T>::GetAltTypeId() ];
				if (component)			
					return static_cast<T*>(component);
				else {
					std::cerr << "id no encontrado" << std::endl;
					return NULL;	
				}
		}
		
		/**
		Add - Pep
		
		*/
		void setOriginBase(unsigned short originBase)
		{
			_originBase=originBase;
		}

		unsigned short getOriginBase()
		{
			return _originBase;
		}

		void detachFromMap();
		void attachToMap(CMap* map);

	protected:

		/**
		Clase amiga que puede modificar los atributos (en concreto se 
		usa para modificar el mapa.
		*/
		friend class CMap;

		/**
		Identificador único de la entidad.
		*/
		Logic::TEntityID _entityID;
		
		/**
		Estructura de datos que contiene la posición lógica
		*/
		Logic::CLogicalPosition *_pos;
		//Logic::CLogicalPosition _pos;
		
		/**
		Tipo para la lista de componetes.
		*/
		typedef std::map<altTypeId, IComponent*> TComponentMap;
		typedef std::vector<IComponent*>		TComponentVector;

		/**
		Lista de los componentes de la entidad.
		*/
		TComponentMap		_compoTable;
		TComponentVector	_compoList;

		/**
		Indica si la entidad está activa.
		*/
		bool _activated;

		/**
		Indica si la entidad ha sido puesta en Maps.txt como coordenadas polares (Lógicas)
		*/
		bool _logicInput;


		/**
		Tipo de la entidad declarado en el archivo blueprints.
		*/
		std::string _type;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Nombre gráfico de la entidad (name + id)
		*/
		std::string _graphicalName;

		/**
		Mapa lógico donde está la entidad.
		*/
		Logic::CMap *_map;

		/**
		Matriz de transformación de la entidad. Contiene posición y orientación.
		*/
		Matrix4 _transform;

		/**
		Material inicial de la entidad.
		*/
		std::string _initialMaterial;
		/**
		Escala inicial de la entidad.
		*/
		float _initialScale;
		

		/*
		Posición de la entidad.
		*
		Vector3 _position;

		/*
		Posición de la entidad.
		*
		float _orientation;

		/**
		Atributo que indica si la entidad es el jugador; por defecto
		es false a no ser que se lea otra cosa de los atributos.
		*/
		bool _isPlayer;

		/** pep: añado variable que me sirve para el gameStatus 
		Indica la base de origen de la entidad.
		El índice de la base en el array se corresponde
		siempre con la posición del player en su array.
		Conociendo el indice podemos acceder directamente a su
		CPlayerInfo sin riesgo a equivocarnos.
		*/
		unsigned short _originBase;

		/**
		Pep, Valor offset para la altura de un modelo, por ejemplo que no tenga su pivote bien.
		*/
		float _offsetHeight;

	}; // class CEntity

} // namespace Logic

#endif // __Logic_Entity_H
