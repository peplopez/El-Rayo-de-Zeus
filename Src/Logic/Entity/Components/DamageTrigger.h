/**
@file DamageTrigger.h

Contiene la declaración de un componente que envia un mensaje DAMAGED cuando su 
entidad es tocada. El mensaje se envía a la entidad que se ha tocado.

@see Logic::CDamageTrigger
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_DamageTrigger_H
#define __Logic_DamageTrigger_H

#include "Logic/Entity/Component.h"

// Los componentes pertenecen al namespace Logic
namespace Logic 
{
	/**
	Este componente procesa mensajes de tipo TOUCHED (indican que la entidad ha 
	sido tocada) para enviar un mensaje DAMAGED a la entidad que tocó.
	<p>
	El daño que recibe la entidad se especifica en el mapa con el atributo "damage".
	
    @ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
*/
	class CDamageTrigger: public IComponent
	{
		DEC_FACTORY(CDamageTrigger);
	public:

		/**
		Constructor por defecto.
		*/
		CDamageTrigger() : IComponent(), _damage(20.f) {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo TOUCHED.
		*/
		virtual bool accept(const TMessage &message);

		/**
		Al recibir un mensaje TOUCHED se envía otro mensaje de tipo DAMAGED a la
		entidad tocada.
		*/
		virtual void process(const TMessage &message);

	protected:

		/**
		Daño que se produce cada vez que se toca.
		*/
		float _damage;

	}; // class CDamageTrigger

	REG_FACTORY(CDamageTrigger);

} // namespace Logic

#endif // __Logic_DamageTrigger_H
