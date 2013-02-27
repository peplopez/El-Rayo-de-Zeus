/**
@file RingTraveler.h

Contiene la declaración del componente que controla la capacidad de cambiar de anillo
de los elementos del juego

@see Logic::CRingTraveler
@see Logic::IComponent

@author Jose Luis López Sánchez
@date Febrero, 2013
*/
#ifndef __Logic_RingTraveler_H
#define __Logic_RingTraveler_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	class CMessage;
}
//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de los cambios de anillo. Con este componente no puede cambiar de Base.
	
    @ingroup logicGroup

	@author Jose Luis López Sánchez
	@date Febrero, 2013
*/
	class CRingTraveler : public IComponent
	{
		DEC_FACTORY(CRingTraveler);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CRingTraveler() : IComponent(),_changingRing(false),_changingRingTime(0),_maxChangingRingTime(2000) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CRingTraveler();
		
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		CHANGE_PLANE.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const CMessage *message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		
		///** UNDONE ƒ®§
		//Provoca que la entidad baje de anillo. Conlleva un cambio del eje de giro en su coordenada y
		//*/
		//void goDown();

		///**
		//Provoca que la entidad suba de anillo. Conlleva un cambio del eje de giro en su coordenada y
		//*/
		//void goUp();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

	protected:

		bool _changingRing;	

		float _changingRingTime;
		
		float _maxChangingRingTime;

	}; // class CRingTraveler

	REG_FACTORY(CRingTraveler);

} // namespace Logic

#endif // __Logic_CRingTraveler_H
