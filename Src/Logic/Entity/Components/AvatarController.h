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
#include "Logic/Entity/Component.h"

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
		CAvatarController() : IComponent(), _angularSpeed(0.00625f),_sentidoColision(false),_walkingRight(false), _walkBack(false), 
			_walkingLeft(false),_correccionGrados(0) {}
		
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
		Provoca que la entidad retroceda al chocarse con otra entidad en lugar de simplemente pararse, 
		así ya no está colisionando y se puede mover.
		*/	
		void walkBack();

		/**
		Provoca que la entidad avance a la derecha.
		*/
		void walkRight();

		/**
		Provoca que la entidad avance a la izquierda
		*/
		void walkLeft();

		/**
		Provoca que la entidad cese el desplazamiento.
		*/
		void stopMovement();
		
		
		/**
		Provoca que la entidad cambie de dirección.
		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void changeDirection(const bool newDirection);

				/**
		Provoca que la entidad salte.
		*/
		void jump();
		
		/**
		Provoca que la entidad baje de anillo. Conlleva un cambio del eje de giro en su coordenada y
		*/
		void goDown();

		/**
		Provoca que la entidad suba de anillo. Conlleva un cambio del eje de giro en su coordenada y
		*/
		void goUp();


		void changeBase(int base);


		void turn(float amount);

	protected:

		/**
		Atributo para saber si la entidad está andando a la derecha.
		*/
		bool _walkingRight;

		/**
		Atributo para saber si la entidad está andando a la izquierda.
		*/
		bool _walkingLeft;

		/**
		Atributo para saber si la entidad tiene que hacer una recolocación de posición
		seguramente debido a una colisión
		*/
		bool _walkBack;

		bool _sentidoColision;

		bool _sentidoDerecha;

		float _correccionGrados;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _angularSpeed;

	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H */
