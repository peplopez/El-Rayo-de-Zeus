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

#include "Logic/Entity/Components/Physic.h"
#include "Logic/Entity/LogicalPosition.h"



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
	class CPhysicEntity : public CPhysic
	{
		DEC_FACTORY(CPhysicEntity);

	public:

		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		virtual void onTrigger (IObserver *other, bool enter);

	}; // class CPhysicEntity

	REG_FACTORY(CPhysicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
