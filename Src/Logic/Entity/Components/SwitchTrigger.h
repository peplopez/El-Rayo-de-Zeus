/**
@file SwitchTrigger.h

Contiene la declaración del componente que envia un mensaje SWITCH a una
entidad cuando su entidad es tocada o dejada de ser tocada.

@see Logic::CSwitchTrigger
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_SwitchTrigger_H
#define __Logic_SwitchTrigger_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente procesa mensajes de tipo TOUCHED o UNTOUCHED (indican que la 
	entidad ha sido tocada o dejada de ser tocada) para enviar un mensaje SWITCH a 
	una entidad objetivo.
	<p>
	La entidad objetivo se especifica en el mapa con el atributo "target". Este 
	atributo <em>debe</em> ser especificado.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
*/
	class CSwitchTrigger : public IComponent
	{
		DEC_FACTORY(CSwitchTrigger);
	public:

		/**
		Constructor por defecto.
		*/
		CSwitchTrigger() : IComponent(), _targetName(""),
			_target(0) {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que se invoca para activar el componente.
		*/
		virtual bool activate();
		
		/**
		Método que se invoca al desactivar el componente.
		*/
		virtual void deactivate();

		/**
		Este componente sólo acepta mensaje de tipos TOUCHED y UNTOUCHED.
		*/
		virtual bool accept(const TMessage &message);

		/**
		Al recibir mensaje TOUCHED y UNTOUCHED emite mensajes SWITCH
		a la entidad objetivo para que cambie de posición.
		*/
		virtual void process(const TMessage &message);

	protected:

		/**
		Nombre de la entidad a la que se desea enviar un SWITCH cuando se 
		recibe un mensaje TOUCHED o UNTOUCHED.
		*/
		std::string _targetName;

		/**
		Entidad a la que se desea enviar un SWITCH cuando se recibe un mensaje 
		TOUCHED o UNTOUCHED.
		*/
		CEntity *_target;

	}; // class CSwitchTrigger

	REG_FACTORY(CSwitchTrigger);

} // namespace Logic

#endif // __Logic_SwitchTrigger_H
