/**
@file CommunicationPort.h

Contiene la declaración de la clase que se encarga del intercambio 
de los mensajes.

@see Logic::CCommunicationPort

@author David Llansó García
@date Julio, 2010
*/

#ifndef __Logic_CommunicationPort_H
#define __Logic_CommunicationPort_H

#include <list>

#include "Message.h"

// Declaración de la clase
namespace Logic 
{

/**
	La clase CCommunicationPort se encarga de toda la gestion de 
	los mensajes que pueden ser enviados. la interfaz de componente
	heredará de esta clase para delegar en ella toda la comunicación
	entre componentes. 
	<p>
	En cualquier momento se puede enviar un mensaje mediante el 
	método set() que verá si el mensaje es aceptado y en ese caso
	lo meterá en una cola para que sea procesado posteriormente. Los 
	mensajes por tanto no se procesan automáticamente sino que se 
	guardan y posteriormente, generalmente una vez por frame, se
	procesan todos los mensajes recibidos invocando al método
	processMessages().
	<p>
	Para que esto funcione correctamente, las clases hijas deberán
	implementar los métodos virtuales accept() y process().
	El método accept() decidirá si el mensaje recibido es aceptado 
	o rechazado, mientras que el método process() se encargará de 
	procesar un mensaje concreto.

    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó
	@date Julio, 2010
*/
	class CCommunicationPort 
	{
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CCommunicationPort() {}

		/**
		Destructor. Vacía el vector de mensajes.
		*/
		virtual ~CCommunicationPort();

		/**
		Método que añade un mensaje a la cola si éste es aceptado.

		@param message Mensaje a procesar.
		@return true si el mensaje ha sido admitido y puesto en cola.
		*/
		bool set(const TMessage &message);

		/**
		Método virtual que elige que mensajes son aceptados. Las clases
		que hereden del puerto de comunicación deberán reimplementar
		este método si quieren aceptar algún mensaje ya que por defecto
		se rechazan todos.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const TMessage &message) {return false;}

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const TMessage &message) {}

		/**
		Método que procesa la lista de mensajes que faltan por procesar.
		Simplemente invoca a process() con cada uno de los mensajes.
		*/
		void processMessages();

	protected:
		/**
		Tipo lista de CEntity donde guardaremos los pendientes de borrar.
		*/
		typedef std::list<TMessage> TMessageList;

		/**
		Lista de mensajes por procesar
		*/
		TMessageList _messages;

	}; // CCommunicationPort

} // namespace Logic

#endif // __Logic_CommunicationPort_H
