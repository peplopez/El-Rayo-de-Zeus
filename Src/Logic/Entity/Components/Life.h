/**
@file Life.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_Life_H
#define __Logic_Life_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el daño a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	TODO  Si la vida pasa a ser 0 que la entidad muera (poner animación de muerte?)
	y si es el jugador habrá que terminar el juego. Si la vida sigue siendo mayor 
	que 0 trás un golpe ¿poner la animación de herido?.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
*/
	class CLife : public IComponent
	{
		DEC_FACTORY(CLife);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLife() : IComponent(), _life(100.f) {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		bool accept(const CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		void process(CMessage *message);

	protected:

		/**
		Vida de la entidad
		*/
		float _life;

	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
