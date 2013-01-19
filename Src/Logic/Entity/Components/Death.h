/**
@file Death.h

Contiene la declaración del componente que controla la muerte de una entidad.

@see Logic::CDeath
@see Logic::IComponent

@author ƒ®§
@date Enero, 2013
*/
#ifndef __Logic_Death_H
#define __Logic_Death_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**	
    @ingroup logicGroup

	@author ƒ®§
	@date Enero, 2013
*/
	class CDeath: public IComponent
	{
		DEC_FACTORY(CDeath);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CDeath() : IComponent() {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo DEAD.
		*/
		virtual bool accept(const TMessage &message);

		/**
		Al recibir un mensaje de tipo DEAD se elimina la entidad.
		*/
		virtual void process(const TMessage &message);


	}; // class CLife

	REG_FACTORY(CDeath);

} // namespace Logic

#endif // __Logic_Death_H
