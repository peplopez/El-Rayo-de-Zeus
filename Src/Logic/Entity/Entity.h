/**
@file Entity.h

Contiene la declaraci�n de la clase Entity, que representa una entidad
de juego. Es una colecci�n de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Logic_Entity_H
#define __Logic_Entity_H

#include "BaseSubsystems/Math.h"

#include "Logic/Maps/EntityID.h"
#include "LogicalPosition.h"


// Mensaje
#include "Message.h"

#include <list>
#include <string>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class IComponent;
	class CEntityFactory;
}

// Declaraci�n de la clase
namespace Logic 
{
	/**
	Clase que representa una entidad en el entorno virtual. Las entidades
	son meros contenedores de componentes, y su funcionamiento depende
	de cu�les tenga. Adem�s, la clase contiene una serie de atributos que
	pueden ser accedidos desde los componentes (Id, nombre, etc.).
	<p>
	Tras la creaci�n de la entidad, donde se inicializan los valores de la
	entidad a su valor por defecto, se invocar� a su m�todo spawn() en el
	que se inicializar�n los atributos de la entidad con los valores leidos 
	del fichero del mapa. Tras esto, en alg�n momento, cuando se active
	el mapa se llamar� al m�todo activate() de la entidad. En este momento
	los componentes har�n visibles los componentes gr�ficos y dem�s ya que 
	apartir de ah� es cuando la entidad empieza su funci�n siendo actualizada 
	por el tick() en cada vuelta de bucle.

    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llans�
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
		utilizarse la factor�a CEntityFactory). El constructor
		no hace m�s que poner el identificador �nico de la entidad
		, pues la inicializaci�n efectiva se hace en el m�todo spawn().
		
		@param entityID Identificador �nico de la entidad.
		*/
		CEntity(TEntityID entityID);

		/**
		Destructor de la clase. Es un m�todo protegido pues para
		eliminar CEntity debe utilizarse la factor�a
		CEntityFactory. El destructor invoca al destructor de
		cada componente.
		Cuando el destructor es invocado, �ste _ya ha sido desenganchado
		del mapa al que perteneci�_, por lo que los destructores de los
		componentes no pueden utilizar el mapa.
		*/
		~CEntity();

		/**
		Inicializaci�n del objeto Logic, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Map::CEntity). Avisar� a los componentes
		de la entidad para que se inicialicen.

		@param map Mapa Logic en el que se registrar� la entidad.
		@param entity Informaci�n de construcci�n de la entidad le�da del
		fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		bool spawn(CMap *map, const Map::CEntity *entity);

	public:
			
		/**
		Activa la entidad. Esto significa que el mapa ha sido activado.
		<p>
		El m�todo llama al activate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true si todo fue bien.
		*/
		bool activate();

		/**
		Desactiva la entidad. Esto significa que el mapa ha sido desactivado.
		<p>
		El m�todo llama al deactivate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Llamar� a los m�todos tick() de todos sus componentes.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		M�todo que a�ade un nuevo componente a la lista de la entidad.

		@param component Componente a a�adir.
		*/
		void addComponent(IComponent* component);

		/**
		M�todo que quita un componente de la lista.

		El componente es eliminado de la lista de componentes de la
		entidad, pero <b>no</b> es desactivado ni eliminado (con delete);
		la responsabilidad de esa tarea se deja al invocante.

		@param component Componente a borrar.
		@return true si se borr� el componente (false si el componente
		no estaba en el objeto).
		*/
		bool removeComponent(IComponent* component);
		
		/**
		M�todo que destruye todos los componentes de una entidad.
		*/
		void destroyAllComponents();

		/**
		Recibe un mensaje que envia a todos los componentes de la lista menos 
		al componente que lo envia, si �ste se especifica en el segundo campo.

		@param message Mensaje a enviar.
		@param emitter Componente emisor, si lo hay. No se le enviar� el mensaje.
		@return true si al menos un componente acept� el mensaje
		*/
		const bool emitMessage (const TMessage &message, IComponent* emitter = 0);

		/**
		Devuelve el identificador �nico de la entidad.

		@return Identificador.
		*/
		Logic::TEntityID getEntityID() const { return _entityID; }

		/**
		M�todo que indica si la entidad es o no el jugador.
		Seguro que hay formas mejores desde el punto de vista de
		dise�o de hacerlo, pero esta es la m�s r�pida: la entidad 
		con la descripci�n de la entidad tiene esta descripci�n que
		establece en el spawn().
		
		@return true si la entidad es el jugador.
		*/
		bool isPlayer() { return _isPlayer; }

		/**
		Devuelve el mapa donde est� la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		CMap *getMap() { return _map; }

		/**
		Devuelve el mapa donde est� la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		const CMap *getMap() const { return _map; }

		/**
		Devuelve el nombre de la entidad.

		@return Nombre de la entidad.
		*/
		const std::string &getName() const { return _name; }

		/**
		Devuelve el tipo de la entidad. Este atributo es leido de
		la entidad del mapa en spawn().

		@return Tipo de la entidad.
		*/
		const std::string &getType() const { return _type; }

		/**
		Establece la matriz de transformaci�n de la entidad. Avisa a los 
		componentes del cambio.

		@param transform Nueva matriz de transformaci�n de la entidad.
		*/
		void setTransform(const Matrix4& transform);

		/**
		Devuelve la metriz de transformaci�n de la entidad.
		<p>
		La posici�n es inicialmente le�da del mapa (si no aparece,
		se colocar� a (0, 0, 0)) y la orientaci�n es tambi�n inicialmente 
		le�da del mapa, como un simple viraje (si no aparece, se colocar� 
		a 0). Obviamente, pueden cambiar con el tiempo.

		@return Matriz de transformaci�n de la entidad en el entorno.
		*/
		Matrix4 getTransform() const { return _transform; }

		/**
		Establece la posici�n de la entidad. Avisa a los componentes
		del cambio.

		@param position Nueva posici�n.
		*/
		void setPosition(const Vector3 &position);

		/**
		Devuelve la posici�n de la entidad.
		<p>
		La posici�n es inicialmente le�da del mapa (si no aparece,
		se colocar� a (0, 0, 0)), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Posici�n de la entidad en el entorno.
		*/
		Vector3 getPosition() const { return _transform.getTrans(); }

		/**
		Establece la posici�n de la entidad en grados.

		@param position Nueva posici�n.
		*/
		void setDegree(const float &degree);

		/**
		Devuelve la posici�n de la entidad.
		en grados
		@return Posici�n de la entidad en el entorno.
		*/
		const float getDegree() const { 
			if (_pos._degrees>=0 && _pos._degrees<360)
				return _pos._degrees;

			if (_pos._degrees>360)
			{
				return (int)_pos._degrees%360;
			}
			else //menor que cero
				return 360-(int)abs((int)_pos._degrees%360);
		}

		/**
		Establece la base de la entidad. 
		NO Avisa a los componentes
		del cambio. Mas adelante vere si es necesario pero creo que no

		@param base nueva
		*/
		void setBase(const short &base);

		/**
		Devuelve la base de la entidad.
		
		@return Base de la entidad en el entorno.
		*/
		short getBase() const { return _pos._base; }

		/**
		Establece el anillo de la entidad. 
		NO Avisa a los componentes
		del cambio. Mas adelante vere si es necesario pero creo que no

		@param Ring nueva
		*/
		void setRing(const LogicalPosition::Ring &ring);

		/**
		Devuelve el anillo de la entidad.
		
		@return Ring de la entidad en el entorno.
		*/
		LogicalPosition::Ring getRing() const { return _pos._ring; }

		/**
		Devuelve el anillo de la entidad.
		
		@return Ring de la entidad en el entorno.
		*/
		short getY() const { 
			switch(_pos._ring)
			{
				case 0:
				{
					return -50;//abajo
					break;
				}
				case 1:
				{
					return 0;//Centro
					break;
				}
				case 2:
				{
					return 50;//arriba
					break;
				}
		}
	
		}

		/**
		Establece el radio sobre el que se mueve la entidad. 
		NO Avisa a los componentes del cambio.
		Mas adelante vere si es necesario pero creo que no

		@param radio nuevo
		*/
		void setRadio(const float &radio);

		/**
		Devuelve el radio sobre el que se mueve la entidad.
		
		@return Radio de la entidad en el entorno.
		*/
		const float getRadio() const { return _pos._radio; }


		/**
		Establece la anchura de la entidad gr�fica
		@param angularBox nuevo
		*/
		void setAngularBox(const float &angularBox);

		/**
		Devuelve la anchura de la entidad gr�fica.
		
		@return AngularBox de la entidad en el entorno.
		*/
		const float getAngularBox() const { return _pos._angularBox; }

		/**
		Establece la anchura de la entidad gr�fica
		@param angularBox nuevo
		*/
		void setSense(const LogicalPosition::Sense &sense);

		/**
		Devuelve la anchura de la entidad gr�fica.
		
		@return AngularBox de la entidad en el entorno.
		*/
		const LogicalPosition::Sense getSense() const { return _pos._sense; }


		/**
		Establece la orientaci�n de la entidad. Avisa a los componentes
		del cambio.

		@param pos Nueva orientaci�n.
		*/
		void setOrientation(const Matrix3& orientation);

			/**
		Cambia la orientaci�n de la entidad.

		@param orientation Referencia a la matriz de rotaci�n con la que debe 
		orientarse la entidad.
		*/
		void setOrientation(const Quaternion &quat);

		/**
		Devuelve la matriz de rotaci�n de la entidad.
		<p>
		La orientaci�n es inicialmente le�da del mapa como un simple 
		viraje (si no aparece, se colocar� a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Orientaci�n en el entorno.
		*/
		 Matrix3 getOrientation() ;

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
		La orientaci�n es inicialmente le�da del mapa como un simple 
		viraje (si no aparece, se colocar� a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Viraje en el entorno.
		*/
		const float getYaw() const { return Math::getYaw(_transform); }

		/**
		Indica si la entidad se encuentra activa.

		@return true si la entidad est� activa.
		*/
		bool isActivated() {return _activated;}

		const Map::CEntity* getEntityInfo() {return _entityInfo; }

		//bool contactoAngular(CEntity* entidad);

		//void Contacto();

	protected:

		/**
		Clase amiga que puede modificar los atributos (en concreto se 
		usa para modificar el mapa.
		*/
		friend class CMap;

		/**
		Identificador �nico de la entidad.
		*/
		Logic::TEntityID _entityID;
		
		/**
		Estructura de datos que contiene la posici�n l�gica
		*/
		Logic::TLogicalPosition _pos;
		
		/**
		Tipo para la lista de componetes.
		*/
		typedef std::list<IComponent*> TComponentList;

		/**
		Lista de los componentes de la entidad.
		*/
		TComponentList _components;

		/**
		Indica si la entidad est� activa.
		*/
		bool _activated;

		/**
		Indica si la entidad ha sido puesta en Maps.txt como coordenadas polares (L�gicas)
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
		Mapa l�gico donde est� la entidad.
		*/
		Logic::CMap *_map;

		/**
		Matriz de transformaci�n de la entidad. Contiene posici�n y orientaci�n.
		*/
		Matrix4 _transform;

		Quaternion _quat;
		/*
		Posici�n de la entidad.
		*
		Vector3 _position;

		/*
		Posici�n de la entidad.
		*
		float _orientation;

		/**
		Atributo que indica si la entidad es el jugador; por defecto
		es false a no ser que se lea otra cosa de los atributos.
		*/
		bool _isPlayer;

		/**
		Copia de la definici�n de la entidad leida del mapa.
		*/
		const Map::CEntity *_entityInfo;
	}; // class CEntity

} // namespace Logic

#endif // __Logic_Entity_H
