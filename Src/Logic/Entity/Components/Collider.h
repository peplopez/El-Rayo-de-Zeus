/**
@file Collider.h

Contiene la declaración del componente que controla la capacidad de colisionar 
de la entidad.

@see Logic::CCollider
@see Logic::IComponent

@author Jose Luis López Sánchez
@date Diciembre, 2012
*/
#pragma once
#ifndef __Logic_Collider_H
#define __Logic_Collider_H

#include "Logic/Entity/Component.h"

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

	@author Jose Luis López Sánchez
	@date Diciembre, 2012
*/
	class CCollider : public IComponent
	{
		DEC_FACTORY(CCollider);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CCollider() : IComponent(),_sentidoColision(false),_hit(0) {}
		
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
		recibir las ordenes dadas a partir de los eventos de teclado y ratón.
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
		virtual bool accept(const TMessage &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const TMessage &message);
		
		/**
		Método virtual que define el comportamiento en caso de colisión.

		@return devuelve la direccion a la que tiene el obstaculo
		*/
		virtual bool contacto( CEntity* entidad1, CEntity* entidad2);

		/**
		Método virtual que determina en una colisión quien está a la izquierda y quien a la derecha
		*/
		virtual bool sentidoColision(const CEntity* entidad1,const CEntity* entidad2);

		virtual bool contactoAngular( CEntity* entidad, CEntity* entidad2);

		virtual	bool CCollider::contactoExtremo( CEntity* entidad1, CEntity* entidad2);

	protected:
		/**
			false si en una colisión, está a la izquierda
		*/
		bool _sentidoColision;
		
		/**
			false si en una colisión, está a la izquierda
		*/
		short _hit;

		CEntity* _excluido;
		
	}; // class CCollider

	REG_FACTORY(CCollider);

} // namespace Logic

#endif // __Logic_Collider_H
