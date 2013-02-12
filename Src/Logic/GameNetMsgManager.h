//---------------------------------------------------------------------------
// GameNetMsgManager.h
//---------------------------------------------------------------------------

/**
@file GameNetMsgManager.h

Contiene la declaración del gestor de los mensajes de red durante la partida.

@see Logic::Manager::IObserver
@see Logic::CGameNetMsgManager

@author David Llansó
@date Febrero, 2011
*/

#ifndef __Net_gameNetMsgManager_H
#define __Net_gameNetMsgManager_H

#include "Net/Manager.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Maps/EntityID.h"

namespace Logic 
{
	/**
	Este módulo es un singleton que se usa como front-end de la red en el 
	estado de juego. Su mayor uso es desde la lógica, aunque también 
	gestiona otros eventos.
	<p>
	La clase hace de oyente de los mensajes que se envían por red durante
	el estado de juego. Su función es decidir que hacer con cada tipo de 
	mensaje recibido. Principalmente recibe mensajes relacionados con la 
	lógica de las entidades así que los reenvía a las entidades a las que
	corresponde No obstante también puede recibir otros tipos de mensajes 
	de control. 
	<p>
	Del mismo modo que se encarga de la deserialización y reenvío de los 
	mensajes lógicos que vienen encapsulados por la red, proporciona un
	método sendEntityMessage que se encarga de serializar y enviar al otro
	extremo del tubo un mensaje lógico.

	@ingroup netGroup

	@author David Llansó
	@date Febrero, 2011
	*/
	// TODO Implementar Net::CManager::IObserver para poder recibir 
	// paquetes de datos (mensajes de red)
	class CGameNetMsgManager : public Net::CManager::IObserver
	{
	public:

		/**
		Devuelve la única instancia de la clase CServer.
		
		@return Única instancia de la clase CServer.
		*/
		static CGameNetMsgManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CGameNetMsgManager. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Función llamada para activar la escucha.
		*/
		void activate();

		/**
		Función llamada desactivar la escucha.
		*/
		void deactivate();

		/**
		Método que serializa un mensaje lógico y lo envía por el tubo al otro
		extremo de la red.
		
		@param message Mensaje que debe ser serializado y enviado.
		@param destination ID de la entidad a la que va dirigido el mensaje.
		*/
		void sendEntityMessage(const CMessage *message, TEntityID destination);

		/// IOBSERVER
		virtual void dataPacketReceived(Net::CPaquete* packet);
		virtual void connexionPacketReceived(Net::CPaquete* packet){}
		virtual void disconnexionPacketReceived(Net::CPaquete* packet){}

	protected:
		
		/** 
		Constructor de la clase 
		*/
		CGameNetMsgManager();

		/** 
		Destructor 
		*/
		~CGameNetMsgManager();

		/**
		Método que recibe un buffer que encapsula un mensaje de la lógica que 
		debe ser transmitido a una entidad concreta. El método se encarga de
		deserializar el mensaje, recuperar la entidad concreta y enviarle el
		mensaje lógico a ésta.

		@param packet Datos con la serialización del mensaje.
		*/
		void processEntityMessage(Net::CPaquete* packet);

	private:
		/**
		Única instancia de la clase.
		*/
		static CGameNetMsgManager* _instance;

	}; // CGameNetMsgManager

} // namespace Logic

#endif //  __Net_gameNetMsgManager_H
