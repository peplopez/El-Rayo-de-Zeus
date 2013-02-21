/**
@file IObserver.h

Interfaz que deben implementar las clases que quieran ser 
notificadas de eventos de red.

@see Net::IObserver

@author Гоз
@date Febrero, 2013
*/
#ifndef __Net_IObserver_H
#define __Net_IObserver_H


// Predeclaracion de clases
namespace Net {
	class CPaquete;
}

namespace Net
{
	class IObserver{
	public:
		virtual void dataPacketReceived(Net::CPaquete* packet){};
		virtual void connexionPacketReceived(Net::CPaquete* packet){};
		virtual void disconnexionPacketReceived(Net::CPaquete* packet){};
	};
}

#endif // __Net_IObserver_H