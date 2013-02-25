/**
@file BaseTraveler.h

Contiene la declaración del componente que controla la capacidad de cambiar de anillo
de los elementos del juego

@see Logic::CBaseTraveler
@see Logic::IComponent

@author Jose Luis López Sánchez
@date Febrero, 2013
*/
#ifndef __Logic_BaseTraveler_H
#define __Logic_BaseTraveler_H

#include "Logic/Entity/Component.h"
#include "RingTraveler.h"

namespace Logic
{
	class CMessage;
}
//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de los cambios de anillo. Hereda de RingTraveler, lo amplia, hace que no solo
	pueda cambiar de anillo sino también a un anillo de otra base.
	
    @ingroup logicGroup

	@author Jose Luis López Sánchez
	@date Febrero, 2013
*/
	class CBaseTraveler :  public CRingTraveler
	{
		DEC_FACTORY(CBaseTraveler);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CBaseTraveler() : CRingTraveler(GetAltTypeIdOf(CBaseTraveler)),_changingBase(false),_changingBaseTime(0),_maxChangingBaseTime(5000) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CBaseTraveler();
		
		
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


		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		
		/**
		Provoca que la entidad cambie de base. Conlleva un cambio del eje de giro en su coordenada y
		*/
		void changeBase(int base);

	protected:

		bool _changingBase;	

		float _changingBaseTime;

		float _maxChangingBaseTime;

	}; // class CBaseTraveler

	REG_FACTORY(CBaseTraveler);

} // namespace Logic

#endif // __Logic_CBaseTraveler_H
