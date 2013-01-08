/**
@file PhysicEntity.h

Contiene la declaración del componente encargado de la representación física de las
entidades. Esta clase sirve para representar cualquier entidad excepto los denominados
character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicCharacter

@author Antonio Sánchez Ruiz-Granados
@date Octubre, 2010
*/
#ifndef __Logic_PhysicEntity_H
#define __Logic_PhysicEntity_H

#include "Logic/Entity/Component.h"
#include "Physics/Server.h"

// Predeclaración de clases para ahorrar tiempo de compilación
class IPhysicModel;
class CPhysicModelSimple;
class CPhysicModelGeneric;
class IPhysicObj;

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	// Constantes para las cadenas que se usan para definir las entidades en el
	// fichero de mapa. 
	const std::string STR_PHYSIC_ENTITY = "physic_entity";
	const std::string STR_SIMPLE = "simple";
	const std::string STR_GENERIC = "generic";
	const std::string STR_CONTROLLER = "controller";
	const std::string STR_PHYSIC_FILE = "physic_file";
	const std::string STR_PHYSIC_TYPE =	"physic_type";
	const std::string STR_STATIC = "static";
	const std::string STR_DYNAMIC = "dynamic";
	const std::string STR_KINEMATIC = "kinematic";
	const std::string STR_PHYSIC_SHAPE = "physic_shape";
	const std::string STR_CAPSULE = "capsule";
	const std::string STR_PHYSIC_RADIUS = "physic_radius";
	const std::string STR_PHYSIC_HEIGHT = "physic_height";
	const std::string STR_PLANE = "plane";
	const std::string STR_PHYSIC_NORMAL = "physic_normal";
	const std::string STR_BOX = "box";
	const std::string STR_PHYSIC_DIMENSIONS = "physic_dimensions";
	const std::string STR_PHYSIC_MASS = "physic_mass";
	const std::string STR_PHYSIC_TRIGGER = "physic_trigger";
	const std::string STR_PHYSIC_COLLISION_GROUP = "physic_collision_group";

	/**
	Componente encargardo de la representación física de una entidad. Puede ser utilizado
	para representar cualquier tipo de entidad física excepto los denominados character 
	controllers, que se representan usando un tipo de componente específico.
	<p>
	En un videojuego más complejo seguramente convendría dividir la funcionalidad que ofrece 
	este componente en varios componentes más especializados. Para nuestro juego, sin embargo, 
	no necesitamos cosas muy elaboradas. 
	<p>
	Esta clase actualiza la posición y rotación de las entidades lógicas en cada tick usando
	la infomación que proporciona el motor de física. 
	<p>
	Con respecto a mover las entidades físicas, debemos tener en cuenta que existen 3 tipos 
	de entidades: estáticas, dinámicas y kinemáticas. Las entidades estáticas no se pueden mover
	una vez creadas. Las entidades dinámicas responden a fuerzas y, en principio, no deberían 
	moverse directamente desde el juego si no es mediante la aplicación	de fuerzas.	Las 
	entidades kinemáticas, por el contrario, se mueven tal y como dicta la lógica del juego. 
	Cuando este componente se utiliza para representar una entidad kinemática acepta mensajes de 
	tipo SET_TRANSFORM, y en cada tick pide la motor de física que mueva la entidad de acuerdo
	a la transformación recibida desde la lógica.
	
    @ingroup logicGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Octubre, 2010
	*/
	class CPhysicEntity : public IComponent
	{
		DEC_FACTORY(CPhysicEntity);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicEntity();

		/**
		Destructor (virtual). Elimina el objeto físico de la escena y lo destruye. 
		*/
		virtual ~CPhysicEntity();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de
		mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


		/**
		Este componente sólo acepta mensajes de tipo SET_TRANSFORM. La matriz de 
		transformación recibida en dichos mensajes sólo se usara para mover la entidad
		física asociada al componente si es de tipo kinemático. */
		virtual bool accept(const TMessage &message);

		/**
		Si se recibe un mensaje de tipo SET_TRANSFORM, se almacena su matriz de 
		trasnformación para posteriormente mover la entidad física en el tick
		(si la entidad física es de tipo kienmático). De este moto, si en un ciclo
		se reciben varios mensaje de tipo SET_TRANSFORM sólo tendrá efecto el último
		que se reciba.
		*/
		virtual void process(const TMessage &message);

		/**
		Este método se invoca en cada ciclo de la simulación y hace lo siguiente:
		<ul>
		<li>Actualiza la posición y rotación de la entidad lógica usando la información 
		que proporciona el motor de física.</li>
		
		<li>Si la entidad física asociada al componente es de tipo kinemático, 
		solicita al motor de física que la mueva de acuerdo al último mensaje 
		SET_TRANSFORM recibido.</li>
		</ul>
		*/
		virtual void tick(unsigned int msecs);

		/**
		Este método se invoca cada vez que la entidad física asociada a este 
		componente choca contra otra entidad. Debemos tener en cuenta que las 
		dos entidades involucradas en el choque serán notificadas a través de 
		este método.

		@param otherEntity La otra entidad involucrada en el choque.
		 */
		virtual void onEntityHit(const CEntity *otherEntity);

		/**
		Este método se invoca cada vez que la entidad física asociada a este 
		componente interactúa con otra, siendo alguna de ellas un trigger. Debemos 
		tener en cuenta que las dos entidades involucradas serán notificadas a 
		través de este método. Por tanto, el parámetro "otherEntity" puede representar
		tanto una entidad asociada a un trigger (cuando se notifica a la otra entidad)
		como una entidad física de cualquier tipo (cuando se notifica al trigger).

		@param otherEntity La otra entidad involucrada.
		@param enter Indica si una entidad física ha entrado en el trigger.
		@param leave Indica si una entidad física ha salido del trigger.
		@param stay Indica si una entidad física permanece dentro del trigger.
		*/
		virtual void onEntityTrigger(const CEntity *otherEntity, bool enter, bool leave, bool stay);

	protected:

		/**
		Este método construye y devuelve la entidad física asociada a este componente, 
		en base a la información del mapa. La implementación por defecto utiliza el 
		método "createPhysicModel" para conseguir el modelo lógico que describe el tipo 
		de entidad física que se debe crear. 
		
		@param entityInfo Descripción de la entidad en el fichero de mapa.
		@return Entidad física creada, NULL si hubo algún problema.
		*/
		virtual IPhysicObj* createPhysicEntity(const Map::CEntity *entityInfo);

		/**
		Este método devuelve el modelo físico que describe el tipo de entidad física
		que se debe crear.

		@param entityInfo Descripción de la entidad en el fichero de mapa.
		@param Modelo físico que describe el tipo de entidad física deseado,
			   NULL si hubo algún problema.
		*/
		virtual IPhysicModel* createPhysicModel(const Map::CEntity *entityInfo);

		/**
		Servidor de física.
		*/
		Physics::CServer *_physicServer;
		
		/**
		Entidad física asociada a este componente.
		*/
		IPhysicObj *_physicObj;

		/**
		Matriz de transformación recibida en el último mensaje de tipo SET_TRANSFORM. Sirve
		para mover entidades físicas kinemáticas.
		*/
		Matrix4 _transform;

	private:

		/**
		Este método indica el tipo de entidad física que debemos crear (estática, dinámica o 
		kinemática) a partir de la descripción de la entidad en el fichero de mapa.

		@param entityInfo Descripción de la entidad en el fichero de mapa.
		@return Tipo de entidad física: estática, dinámica o kinemática.
		*/
		Physics::TPhysicMode getPhysicMode(const Map::CEntity *entityInfo);

		/**
		Este método construye y devuelve un modelo que describe un tipo de entidad física simple.
		Las entidades físicas simples pueden estar formadas por varios volúmenes de colisión.

		@param entityInfo Descripción de la entidad en el fichero de mapa.
		@return Modelo que describe una entidad física simple.
		*/
		CPhysicModelSimple* createPhysicModelSimple(const Map::CEntity *entityInfo);

		/**
		Este método añade un volumen de colisión (shape) a un modelo físco simple. El tipo de volumen
		que se debe crear (caja, cilindro, plano, etc) se obtiene a partir de la descripción de la
		entidad en el fichero de mapa.

		@param entityInfo Descripción de la entidad en el fichero de mapa.
		@param model Modelo físico que describe un tipo de entidad física simple.
		@param scale Factor que permite escalar el tamaño del volumen de colision creado.
		*/
		void createPhysicShape(const Map::CEntity *entityInfo, CPhysicModelSimple *model, float scale);

		/**
		Este método construye y devuelve un modelo que describe un tipo de entidad física genérica.
		Las entidades físicas genéricas pueden estar formadas por varias entidades físicas simples
		o genéricas. Este tipo de entidades se suelen crear a partir de ficheros exportados desde
		editores tipo 3DMAX.

		@param entityInfo Descripción de la entidad en el fichero de mapa.
		@return Modelo que describe una entidad física genérica.
		*/
		CPhysicModelGeneric* createPhysicModelGeneric(const Map::CEntity *entityInfo);

	}; // class CPhysicEntity

	REG_FACTORY(CPhysicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
