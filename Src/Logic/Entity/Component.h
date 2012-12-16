/**
@file Component.h

Contiene la declaración de la clase base de los componentes.

@see Logic::IComponent
@see Logic::CCommunicationPort

@author David Llansó
@date Julio, 2010
*/
#ifndef __Logic_Component_H
#define __Logic_Component_H

#include "CommunicationPort.h"
#include "Logic/Maps/ComponentFactory.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class CEntity;
}

//declaración de la clase
namespace Logic 
{
/**
	Clase base de los componentes que forman las entidades.
	<p>
	Un componente puede recibir mensajes y reaccionar ante ellos
	enviando otros mensajes al resto de componentes hermanos o
	realizando alguna acción sobre el entorno.
	<p>
	Para procesar los mensajes y para realizar cualquier otro
	comportamiento, la entidad a la que el componente pertenece
	invoca periódicamente a la función tick() del componente. 
	La implementación por defecto de ese método simplemente procesa todos
	los mensajes pendientes recibidos por el componente.
	<p>
	Si las clases hija sobreescriben este método, <em>son responsables</em>
	de invocar al método de procesado de mensajes (o en su defecto al
	método tick() de la clase padre). En caso de no hacerlo, el
	componente <em>no procesará ningún mensaje</em>.
	<p>
	El ciclo de vida de un componente típico viene determinado por
	el ciclo de vida de la entidad a la que pertenece. El componente 
	puede estar activado o desactivado:
	<ul>
	   <li> El componente es creado cuando se crea la entidad a la que pertenece
	   al leer el mapa. Después de llamar al constructor de la clase
	   se invoca al método spawn() que podrá leer los parámetros leidos del mapa
	   para inicializar sus atributos. En ese momento se pueden crear recursos
	   que necesite el componente, como las entidades gráficas, físicas, etc.</li>

	   <li> El componente es <em>activado</em> llamando al método activate().
	   Un componente se activa cuando <em>se activa el mapa donde está la
	   entidad a la que pertenece</em>,
	   es decir, cuando el motor de juego decide que se empezará a simular
	   el entorno de ese mapa.</li>

	   <li> El componente finalmente es <em>desactivado</em>, cuando el motor del
	   juego establece que no se desea simular el entorno virtual del mapa
	   donde está el componente. En ese caso, se invoca al método deactivate().</li>

	   <li> Cuando se destruye el mapa cargado (típicamente al final del juego/estado
	   del juego), se invoca al destructor de la entidad.</li>
	</ul>
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó García
	@date Julio, 2010
*/
	class IComponent : public CCommunicationPort 
	{
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		IComponent() : _entity(0) {}

		/**
		Destructor (virtual); en la clase base no hace nada.
		*/
		virtual ~IComponent() {}
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). El método es extendido
		en las clases hijas, que hacen cosas más inteligentes, dependiendo
		del tipo de componente concreto, pero siempre deberán invocar a esta
		antes de hacer nada.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		La implementación registrará al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cronómetro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate() {return true;}
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementación eliminará al componente de algunos observers en los 
		que pueda estar registrado (como el cronómetro del sistema, etc.).
		*/
		virtual void deactivate() {}

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Las clases hijas deberán sobreescribir este método con las 
		instrucciones que quieran realizar cada ciclo.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		Método que devuelve la entidad a la que pertenece el componente.

		@return La entidad a la que pertenece el componente.
		*/
		CEntity *getEntity() const { return _entity; }

	protected:

		/**
		clase amiga que puede establecerse como poseedor del componente.
		*/
		friend class CEntity;

		/**
		Método que establece la entidad a la que pertenece el componente.

		@param entity Entidad a la que pertenece el componente.
		*/
		void setEntity(CEntity *entity) { _entity = entity; }

		/**
		Entidad que contiene al componente. Para reenviar el mensaje a 
		los otros componentes
		*/
		CEntity *_entity;

	}; // class IComponent


/////////////////////////////////////////////////////////////
// Macros para la adición de los componentes a la factoría // 
// de componentes.                                         //
/////////////////////////////////////////////////////////////
	
/** 
Macro para la declaración de los métodos necesarios para que 
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factoría.
*/
#define DEC_FACTORY(Class) \
public: \
	/** \
	Crea un componente de la clase en la que se declara. \
	*/ \
    static IComponent* create(); \
	/** \
	Registra el componente de la clase en la factoría. \
	*/ \
	static bool regist(); \

/** 
Macro para la implementación de los métodos necesarios para que
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factoría.
*/
#define IMP_FACTORY(Class) \
IComponent* Class::create() \
{ \
	IComponent* res = new Class(); \
	return res; \
} \
bool Class::regist() \
{ \
	if (!CComponentFactory::getSingletonPtr()->has(#Class)) \
	{ \
		CComponentFactory::getSingletonPtr()->add(Class::create, #Class); \
	} \
	return true; \
}

/** 
Macro que invoca al método que registra la clase en la factoría.
*/
#define REG_FACTORY(Class) \
static bool RegisteredFactory_##Class = Class::regist();

} // namespace Logic

#endif // __Logic_Component_H
