/**
@file Physics.h

Contiene la declaración de una clase abstracta de la que deben heredar todos los
componentes físicos.

@see Logic::IComponent
@see Logic::CPhysicEntity
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Logic_Physics_H
#define __Logic_Physics_H
//
#include "Logic/Entity/Component.h"

// Predeclaración de tipos
namespace physx {
	struct PxTriggerPair;
};

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**
	Clase abstracta de la que deben heredar todos los componentes físicos. Proporciona un interfaz
	común para recibir eventos desde el motor de física.
	
    @ingroup logicGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class IPhysics : public IComponent
	{
	public:
 
		/**
		Este método es invocado desde el motor de física cuando una entidad entra o sale de un
		trigger físico. Se notifica tanto al componente asociado al trigger como al componente
		asociado a la otra entidad.

		@param otherComponent Componente asociado al trigger o a la otra entidad, dependiendo
		       de a quién se esté notificando.
		@param enter True si la entidad entra en el trigger y false si sale. 
		*/
		virtual void onTrigger(IPhysics *otherComponent, bool enter) = 0;

	}; // class IPhysics

} // namespace Logic

#endif // __Logic_Physics_H
