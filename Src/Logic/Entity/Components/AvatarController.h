/**
@file AvatarController.h

Contiene la declaración del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#pragma warning(disable: 4482)

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"

namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes métodos que permiten avances o giros. El uso de este componente
	es un poco atípico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las órdenes que se deben
	llevar a cabo mediante llamadas a métodos públicos del componente. Puede
	no obstante ampliarse este componente para aceptar mensajes tradicionales
	con las órdenes, sin embargo de momento así es suficiente.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Agosto, 2010
*/

	class CAvatarController : public IComponent
	{
		DEC_FACTORY(CAvatarController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAvatarController() : IComponent(GetAltTypeIdOf(CAvatarController)), _angularSpeed(0), _turnSpeedFactor(0), _totalYaw(0),
				_targetSense(Logic::LogicalPosition::UNDEFINED), _walkingRight(false), _walkingLeft(false), _acumRotation(0), _wander(false){}
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra así mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y ratón
		(ver CEntity::deactivate() )
		*/
		virtual void deactivate();

		/**
		*/
		virtual void awake();

		/**
		*/
		virtual void sleep();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		CONTROL.

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
		Provoca que la entidad avance a la RIGHT.
		*/
		void walkRight();

		/**
		Provoca que la entidad avance a la LEFT
		*/
		void walkLeft();

		/**
		Provoca que la entidad cese el desplazamiento.
		*/
		void stopMovement();

		/**
		*/
		bool isWalkingRight() {return _walkingRight;}

		bool isWalkingLeft() {return _walkingLeft;}

		void setWander(bool wander) {_wander=wander;}

	protected:

		/**
		Atributo para saber si la entidad está andando a la RIGHT.
		*/
		bool _walkingRight;

		/**
		Atributo para saber si la entidad está andando a la LEFT.
		*/
		bool _walkingLeft;

		/**
		*/
		Logic::Sense _targetSense;

		/**
		*/
		float _totalYaw;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _angularSpeed;

		/**
		Factor que se aplica a la velocidad de giro por defecto
		*/
		float _turnSpeedFactor;
		
		/**
		*/
		float _acumRotation;
		
		/**
		*/
		void emitAngularSpeed(Logic::Sense sense);

		/**
		*/
		void estimateRotation(Logic::Sense sense);

		/**
		*/
		void rotate(Logic::Sense sense, unsigned int msecs);
		
		/**
		Variable que establece si tiene un comportamiento de estar vagando. Lo usa cancerbero, que tiene posibilidad de andar o correr.
		En estos casos cambia la velocidad
		*/
		bool _wander;

	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H */
