/**
@file NetConnector.h

Contiene la declaración del componente que reenvia mensajes por la red.

@see Logic::CNet
@see Logic::IComponent

@author David Llansó
@date Diciembre, 2010
*/

#ifndef __Logic_NetConnector_H
#define __Logic_NetConnector_H

#include "Logic/Entity/Component.h"
#include <vector>

#include "Logic/Entity/Messages/Message.h"

// Declaración de la clase
namespace Logic {

	/**
	Este componente es el nucleo de la comunicación entre entidades
	a través de la red.
	<p>
	Al dividir la entidad en el lado del servidor y del cliente, en 
	ambos lados la entidad tiene una instancia de esta clase. La
	misión de este componente es retransmitir mensajes mandados por
	los compoentes locales de su entidad al otro lado del tubo, 
	previa serialización de esos mensajes.
	<p>
	Para elegir que mensajes se envían por el tubo está el atributo 
	"msgList" que es un string de ids de mensajes separado por comas.
	<p>
	En principio se envian todos los mensajes recibidos, sin embargo,
	para no saturar la red, se puede establecer un tiempo en el cual
	no se enviará más de un mensaje del mismo tipo. Este atributo es
	"blockedTime" y se especifica en milisegundo. Esta aproximación
	evita que se sature la red, pero no es una solución viable para
	un juego. Otras técnicas deberán ser aplicadas en los proyectos
	que usen red.

	@author David Llansó
	@date Diciembre, 2010
	*/
	class CNetConnector : public IComponent 
	{
		DEC_FACTORY(CNetConnector);
	public:

		/**
		Constructor por defecto.
		*/
		CNetConnector() : IComponent(), _timeOfBlocking(0) {}

		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la cámara gráfica y se leen atributos del mapa.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		todos los tipos de mensajes especificados en el mapa.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool accept(const CMessage *message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(CMessage *message);

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		Se controla el número de mensajes del mismo tipo que se envían al 
		otro lado.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

	protected:

		/// Vector que contiene los TMessageType de los mensajes
		/// que debemos propagar por la red.
		std::vector<Logic::TMessageType> _forwardedMsgTypes;

		/// Map que contiene los TMessageType de los mensajes
		/// que están bloqueados (no se envían) debido a que se 
		/// acaba de enviar un mensaje del mismo tipo. El otro
		/// valor es el tiempo que falta para el desbloqueo.
		typedef std::map<Logic::Message::TMessageType, int> TTimeToUnblockMsgDelivery;
		typedef std::pair<Logic::Message::TMessageType, int> TTimeToUnblockMsgDeliveryPair;
		TTimeToUnblockMsgDelivery _timeToUnblockMsgDelivery;

		/// Milisegundos que se esperan entre envios de mensajes del mismo tipo
		unsigned int _timeOfBlocking;

	}; // class CNetConnector

	REG_FACTORY(CNetConnector);

} // namespace OIM

#endif // __Logic_NetConnector_H
