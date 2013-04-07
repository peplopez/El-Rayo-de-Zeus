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

namespace Logic
{
	class CMessage;
}


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
		CDeath() : IComponent(GetAltTypeIdOf(CDeath)) {}
		
		/**
		Este componente sólo acepta mensajes de tipo DEAD.
		*/
		bool accept(const CMessage *message);

		/**
		Al recibir un mensaje de tipo DEAD se elimina la entidad.
		*/
		void process(CMessage *message);


	private:

		void death(CMessage *message);
		void deathAnimated(CMessage *message);
		
		/** ruta del sonido asociado */
		std::string _audio;

	}; // class CLife

	REG_FACTORY(CDeath);

} // namespace Logic

#endif // __Logic_Death_H
