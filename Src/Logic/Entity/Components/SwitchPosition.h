/**
@file SwitchPosition.h

Contiene la declaración del componente que controla el movimiento de
una entidad que tiene dos estado (posiciones) para ir de una a otra.

@see Logic::CSwitchPosition
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_SwitchPosition_H
#define __Logic_SwitchPosition_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
	/**
	Este componente controla el movimiento de una entidad que tiene dos estados 
	(posiciones) y la mueve de uno a otro.
	<p>
	Las posiciones entre las que se transita pueden configurarse desde el mapa.
	De la misma manera se puede estableces el atributo "loop" que mantendrá constante
	las transiciones. El atributo "speed" determina la velocidad del movimiento.
	<p>
	Recibe mensajes de tipo "SWITCH" para transitar de un estado a otro en caso de
	que no se encuentre en loop. El atributo "_int" del mensaje indica el estado
	al que se debe mover la entidad.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
	*/
	class CSwitchPosition : public IComponent
	{
		DEC_FACTORY(CSwitchPosition);
	public:

		/**
		Constructor por defecto.
		*/
		CSwitchPosition();
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo SWITCH. 
		*/
		virtual bool accept(const TMessage &message);

		/**
		Al recibir un mensaje de tipo SWITCH la entidad empieza a moverse hacia la
		posición indicada por el atributo "_int" del mensaje.
		*/
		virtual void process(const TMessage &message);

		/**
		Mueve el componente hacia la posición de destino. Una vez la alcance se para
		(loop = false) o empieza a moverse hacia la otra posición (loop = true).
		*/
		virtual void tick(unsigned int msecs);

	protected:

		/**
		Posición inicial (_position[0]) y final (_position[1]).
		*/
		Vector3 _position[2];
	
		/**
		Posición hacia la que nos estamos moviendo.
		*/
		Vector3 _targetPosition;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _speed;
		
		/**
		Atributo que indica si estamos constantemente moviendo de un punto a otro.
		*/
		bool _loop;

	}; // class CSwitchPosition

	REG_FACTORY(CSwitchPosition);

} // namespace Logic

#endif // __Logic_SwitchPosition_H
