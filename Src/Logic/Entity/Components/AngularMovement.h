/**
@file AngularMovement.h

Contiene la declaración del componente que controla el movimiento 
angular de entidades.

@see Logic::CAngularMovement
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/
#ifndef __Logic_AngularMovement_H
#define __Logic_AngularMovement_H

#include "Logic/Entity/Component.h"
#include "BaseSubsystems/Math.h"
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
	class CAngularMovement : public IComponent
	{
		DEC_FACTORY(CAngularMovement);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		// Pablo. Añadido la inicialización de _jumping y _timeJumping
		CAngularMovement() : IComponent(), _sentidoColision(false),_changingRing(false),_changingBase(false), _walkBack(false), _walkingRight(false), _walkingLeft(false), _goingUp(false), _goingDown(false),
			_angularSpeed(0.00625f), _actualRadius(-55), _actualDegree(-90), _jumpingDown(false), _initialJump(false), _timeJumping(0), _potenciaSaltoInicial(5)/*5*/, _correccionGrados(0){}
		// divididio entre 32
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
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra así mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y ratón.
		*/
		void deactivate();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool accept(CMessage *message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(CMessage *message);

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

		//Pablo
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

		/**
		Provoca que la entidad cese el desplazamiento.
		*/
		void stopMovement();
		
		/**
		Provoca que la entidad gire. Números Positivos para	giro a 
		derechas, negativos para giro izquierdas.

		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void turn(float amount);

		/**
		Provoca que la entidad cambie de dirección.
		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void changeDirection(const bool newDirection);

				
		void changeBase(int base);



	
	protected:

		/**
		Atributo para hacer que retroceda ante una colisión.
		*/
		bool _walkBack;

		/**
		Atributo para saber si la entidad está avanzando a la derecha.
		*/
		bool _walkingRight;

		/**
		Atributo para saber si la entidad está avanzando a la izquierda
		*/
		bool _walkingLeft;


		/**
		Atributo para saber si la entidad está bajando en el salto dentro de un anillo
		*/
		bool _jumpingDown;

		/**
			Atributo para saber si la entidad está bajando de anillo
		*/
		bool _goingUp;
		/**
		Atributo para saber si la entidad está subiendo de anillo
		*/
		bool _goingDown;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _angularSpeed;


		float _actualRadius;

		float _actualDegree;

		float _correccionGrados;

		bool _sentidoDerecha;

		bool _sentidoColision;

		// Pablo. Atributo que indica la velocidad de salto de la entidad
		float jumpSpeed;

		// Pablo. Atributo que indica que acaba de empezar a saltar
		bool _initialJump;

		float _timeJumping;

		// Pablo. Atributo que indica la velocidad de la fuerza de gravedad
		float _gravity;

		//Pablo. Altura maxima del salto
		float _Hmax;

		//Pablo. Tiempo máximo en el aire
		float _Tmax;

		//Pablo. Tiempo hasta alcanzar la máxima altura
		float _Tmaxaltura;

		//Pablo. Y inicial en el momento de saltar
		float _inicialY;

		//Pablo. posicion Vector3 (x,y,z) en el momento de saltar
		Vector3 _posicionSalto;

		bool _changingRing;

		bool _changingBase;
	
		float _potenciaSalto;

		float _potenciaSaltoInicial;


	}; // class CAngularMovement

	REG_FACTORY(CAngularMovement);

} // namespace Logic

#endif // __Logic_AngularMovement_H
