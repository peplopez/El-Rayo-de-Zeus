/**
@file PhysicEntity.h

Contiene la declaración del componente encargado de representar entidades físicas simples.
Este componente no sirve para representar physic characters.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicCharacter
@see Physics::IObserver

@author FRS
@date 23-02-13
*/

#ifndef __Logic_PhysicEntity_H
#define __Logic_PhysicEntity_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"
#include "Physics/IObserver.h"


// Predeclaración de tipos
namespace Physics {
	class CServer;
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
	class CPhysicEntity : public IComponent, public Physics::IObserver
	{
		DEC_FACTORY(CPhysicEntity);

	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicEntity();

		/**
		Destructor. Elimina el objeto físico de la escena y lo destruye. 
		*/
		virtual ~CPhysicEntity();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo KINEMATIC_MOVE. 
		Estos mensajes sólo se utilizan para mover entidades de tipo cinemático.
		*/
		virtual bool accept(const CMessage *message);

		/**
		Cuando recibe mensajes de tipo KINEMATIC_MOVE almacena los movimientos para aplicarlos 
		en el próximo tick sobre la entidad cinemática. Si en un ciclo se reciben varios 
		mensajes KINEMATIC_MOVE se acumulan. 		*/

		virtual void process(CMessage *message);

		/**
		Este método se invoca en cada ciclo de la simulación y hace lo siguiente:
		<ul>
		<li> Si la entidad física es de tipo estático no hace nada. </li>
		<li> Si la entidad física es de tipo dinámico actualiza la posición y rotación de 
		     la entidad lógica usando la información proporcionada por el motor de física. </li>
		<li> Si la entidad física es de tipo cinemático, además solicita al motor de física
		     que mueva la entidad de acuerdo al último mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
// TODO Impl. cuando tengamos dinamicas o cinématicas
// De momento sólo tenemos estáticas
		//virtual void tick(unsigned int msecs);
 
		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		virtual void onTrigger (IObserver *other, bool enter);

	private:

		// Servidor de física
		Physics::CServer* _server;

		// Actor que representa la entidad física
		Physics::CActor* _physicActor;

		// Vector de deplazamiento recibido en el último mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades físicas cinemáticas.
		TLogicalPosition _movement;

		// Crea el actor que representa la entidad física a partir de la información del mapa.*/
		Physics::CActor* createActor(const Map::CEntity* entityInfo);
	
		//Crea un plano estático a partir de la información de mapa. 
		//Physics::CActor* createPlane(const Map::CEntity* entityInfo);
		
		//Crea una entidad rígida (estática, dinámica o cinemática) a partir de la información de mapa. 
		Physics::CActor* createRigid(const Map::CEntity* entityInfo);

	}; // class CPhysicEntity

	REG_FACTORY(CPhysicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
