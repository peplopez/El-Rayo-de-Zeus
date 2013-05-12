/**
@file Physics.h

Contiene la declaración del componente encargado de representar entidades físicas simples.
Este componente no sirve para representar physic characters.

@see Logic::CPhysics
@see Logic::IComponent
@see Logic::CPhysicCharacter
@see Physics::IObserver

@author FRS
@date 23-02-13
*/

#ifndef __Logic_Physics_H
#define __Logic_Physics_H

#include "Logic/Entity/Component.h"
#include "Physics/IObserver.h"


// Predeclaración de tipos
namespace Physics {
	class CScene;
	class CActor;
}

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**
	Componente encargardo de la representación física de una entidad simple. Entendemos por 
	entidades simples aquellas que pueden ser representadas mediante un único actor.
	Esta clase no sirve para representar physic character ni entidades compuestas como ragdolls.
	<p>
	Existen dos tipos de entidades físicas: estáticas y dinámicas. Las entidades 
	estáticas no se pueden mover una vez creadas y sólo sirven como volúmenes de colisión.
	Las entidades dinámicas tienen masa y se mueven aplicándoles fuerzas (por ejemplo la gravedad). 
	También existe un tipo especial de entidades dinámicas llamadas cinemáticas, que están pensadas 
	para representar objetos cuyo movimiento viene dictado desde la lógica del juego. Las entidades 
	cinemáticas se pueden mover mediante desplazamientos en lugar de fuerzas.
	<p>
	En cada tick, este componente actualiza la posición y rotación de las entidades lógicas usando
	la información que proporciona el motor de física (sólo entidades dinámicas). 
	<p>
	Cuando este componente se utiliza para representar una entidad cinemática, acepta mensajes de 
	tipo KINEMATIC_MOVE indicando el movimiento que se quiere realizar. Y en cada tick el componente
	intenta mover la entidad física de acuerdo a los mensajes recibidos. 
	
    @ingroup logicGroup

	@author FRS
	@date 23-02-13
	*/
	class CPhysics : public IComponent, public Physics::IObserver
	{
		DEC_FACTORY(CPhysics);

	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysics::CPhysics() : 
		  IComponent(GetAltTypeIdOf(CPhysics)), _physicalActor(0), _isTrigger(false), _diffDegrees(0), _diffHeight(0), _radius(0), _mass(0), _density(0), _restitution(0) {}
		CPhysics::CPhysics(altTypeId id) : 
		  IComponent(id),  _physicalActor(0), _isTrigger(false), _diffDegrees(0), _diffHeight(0), _radius(0), _mass(0), _density(0), _restitution(0) {}

		/**
		Destructor. Elimina el objeto físico de la escena y lo destruye. 
		*/
		virtual ~CPhysics();
		
		/**	Inicializa el componente usando los atributos definidos en el fichero de mapa.*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		virtual bool accept(const CMessage *message) { return false; }
		virtual void process(CMessage *message) {}
		virtual void tick(unsigned int msecs) {}

		virtual void detachFromMap();
		virtual void attachToMap(CMap* map);

 
		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		virtual void onTrigger(IObserver* other, bool enter);
		virtual void onCollision(IObserver* other);

		void resetScene() {_scene = NULL;}
		void setScene(Physics::CScene* scene) {_scene = scene;}


	protected:

		// UNDONE FRS Physics::CServer* _server; // Servidor de física
		Physics::CScene* _scene; // Servidor de física
		Physics::CActor* _physicalActor; // Actor que representa la entidad física

		float _physicWidth;
		float _physicHeight;

		/*Valores de offset x, y para las AABB de los actores físicos
		*/
		float _radius;
		float _mass;
		float _density;
		float _restitution;

		bool _isTrigger;

		// Desplazamiento recibido en los últimos mensajes de tipo MOVE.
		// Sirve para mover entidades físicas cinemáticas y de character.

		float _diffDegrees;
		float _diffHeight;

		// Crea el actor que representa la entidad física a partir de la información del mapa.*/
		virtual Physics::CActor* createActor(const Map::CEntity* entityInfo);
		// TODO FRS Podría pasar a llamarse physicEntity (por paralelismos con CGraphics)
	
	}; // class CPhysics

	REG_FACTORY(CPhysics);

} // namespace Logic

#endif // __Logic_Physics_H
