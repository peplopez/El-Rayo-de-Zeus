//---------------------------------------------------------------------------
// Server.h
//---------------------------------------------------------------------------

/**
 * @file Server.h
 *
 * Especifica un servidor de red.
 *
 *
 * @author Juan A. Recio-García
 * @date Junio, 2006
 */

#ifndef __Net_Server_H
#define __Net_Server_H

#include <vector>

#include "Packet.h"
#include "Connection.h"


namespace Net {
/**
 * Especifica un servidor de red.
 * Esta interfaz podrá ser implementada de distintas maneras o con distintas librerías. 
 * Con ella se pretende representar el comportamiento de un servidor de red genérico 
 * e independiente de la implementación.
 */
class CServer {

public:

	/**
	 * Destructor virtual.
	 */
	virtual ~CServer(){}

	/**
	 * Inicializa el servidor escuchando en un puerto determinado.
	 * @param port es el puerto donde se escucha
	 * @param clients es el número máximo de clientes. El máximo será establecido por la implementación.
	 * @param maxinbw Ancho de banda de entrada máximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida máximo en bytes/segundo (0 = ilimitado)
	 */
	virtual bool init(int port, int clients, unsigned int maxinbw = 0, unsigned int maxoutbw = 0)=0;
	
	/**
	 * Refresca el servidor obteniendo todos los packets recibidos después de la última llamada a este método
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los packets recibidos
	 * @param packetsRecibidos Vector de CPacket* donde se almacenan los packets recibidos
	 */
	virtual void service(std::vector<CPacket*>& packetsRecibidos)=0;

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Es responsabilidad de este método desconectar todas las connectiones en este punto.
	 */ 
	virtual void deInit()=0;

	/**
	 * Devuelve una lista de las connectiones creadas 
	 */
	virtual std::vector<CConnection*>::iterator listConnections()=0;

	/**
	 * Envía datos a través de una conexión.
	 * @param connection por la que enviar los datos
	 * @param data son los datos a enviar
	 * @param longData tamaño de los datos a enviar
	 * @param channel canal lógico por el que se enviarán los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la máquina remota
	 */
	virtual void sendData(void* data, size_t longData, int channel, bool reliable, CConnection* connection)=0;

	/**
	 * Envía el mismo dato a todos los clientes conectados
	 * @param data son los datos a enviar
	 * @param longData tamaño de los datos a enviar
	 * @param channel canal lógico por el que se enviarán los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la máquina remota
	 */
	virtual void sendAll(void* data, size_t longData, int channel, bool reliable, CConnection* exception)=0;

	/**
	 * Desconecta la conexión indicada.
	 */
	virtual void disconnect(CConnection * connection)=0;

	/**
	 * Indica si el cliente tiene alguna conexión activa.
	 */
	virtual bool isConnected()=0;

	/**
	 * Indica si el cliente está inicializado
	 */
	virtual bool isInitialized()=0;
};

} // namespace Net

#endif // __SERVIDOR_H
