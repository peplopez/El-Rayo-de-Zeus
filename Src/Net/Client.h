//---------------------------------------------------------------------------
// cliente.h
//---------------------------------------------------------------------------

/**
 * @file cliente.h
 *
 * Especifica un cliente de red
 *
 *
 * @author Juan A. Recio-García
 * @date Junio, 2006
 */

#ifndef __Net_Client_H
#define __Net_Client_H

#include <vector>

#include "Connection.h"
#include "packet.h"


namespace Net {

/**
 * Especifica un cliente de red.
 * Esta interfaz podrá ser implementada de distintas maneras o con distintas librerías. 
 * Con ella se pretende representar el comportamiento de un cliente de red genérico 
 * e independiente de la implementación.
 */
class CClient {

public:

	/**
	 * Destructor virtual.
	 */
	virtual ~CClient(){}

	/**
	 * Inicializa el cliente de red.
	 * @param maxConnections Máximo de connectiones permitidas
	 * @param maxinbw Ancho de banda de entrada máximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida máximo en bytes/segundo (0 = ilimitado)
	 * @param true si todo ok
	 */
	virtual bool init(unsigned int maxConnections = 1, unsigned int maxinbw = 0, unsigned int maxoutbw = 0)=0;

	/**
	 * Devuelve la conexión (peer) creada para la dirección y puerto indicados
	 * @param address Dirección IP o nombre de la máquina remota a conectar
	 * @param puerto Puerto remoto al que conectar. (El puerto local utilizado no se especifica en este métdo ya que normalmente es asignado por el S.O.)
     * @param channels Número de canales lógicos a crear sobre la conexión
	 * @param timeout Tiempo máximo de espera para conectar con la máquina remota.
	 * @return La conexión creada. Devuelve NULL si la conexión no ha podido ser establecida o si no se llamó a init() anteriormente.
	 */
	virtual CConnection* connect(char* address, int port, int channels, unsigned int timeout = 5000)=0;

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
	virtual void sendData(void* data, int longData, int channel, bool reliable, CConnection* connection)=0;
	
	/**
	 * Refresca el cliente obteniendo todos los packets recibidos después de la última llamada a este método
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los packets recibidos
	 * @param packetsRecibidos Vector de CPacket* donde se almacenan los packets recibidos
	 */
	virtual void service(std::vector<CPacket*>& packetsRecibidos)=0;

	/**
	 * Obtiene el siguiente packet recibido
	 * IMPORTANTE: Es responsabilidad del invocador de readPacket() borrar el packet recibido
	 * @return último packet recibido
	 */
	virtual CPacket* readPacket() = 0;

	/**
	 * Desconecta la conexión indicada.
	 */
	virtual void disconnect(CConnection * connection)=0;

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Es responsabilidad de este método desconectar todas las connectiones en este punto.
	 */ 
	virtual void deInit()=0;

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

#endif // __CLIENTE_H
