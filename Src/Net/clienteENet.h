//---------------------------------------------------------------------------
// clienteENet.h
//---------------------------------------------------------------------------

/**
 * @file clienteEnet.h
 *
 * Implementa un cliente de red mediante la librería eNet
 *
 *
 * @author Juan A. Recio-García
 * @date Junio, 2006
 */

#ifndef __CLIENTEENET_H
#define __CLIENTEENET_H


#include "enet/enet.h"
#include "cliente.h"

namespace Net {

/**
 * Cliente de red implementado mediante la librería eNet
 */
class CClienteENet: public CCliente {

public:

	/**
	 * Constructor
	 */ 
	CClienteENet();

	/**
	 * Destructor
	 */
	virtual ~CClienteENet();

	/**
	 * Inicializa el cliente de red.
	 * @param maxConnections Máximo de conexiones permitidas
	 * @param maxinbw Ancho de banda de entrada máximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida máximo en bytes/segundo (0 = ilimitado)
	 * @param true si todo ok
	 */
	bool init(unsigned int maxConnections = 1, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

	/**
	 * Devuelve la conexión (peer) creada para la dirección y puerto indicados
	 * @param address Dirección IP o nombre de la máquina remota a conectar
	 * @param puerto Puerto remoto al que conectar. (El puerto local utilizado no se especifica en este métdo ya que normalmente es asignado por el S.O.)
     * @param channels Número de canales lógicos a crear sobre la conexión
	 * @param timeout Tiempo máximo de espera para conectar con la máquina remota.
	 * @return La conexión creada. Devuelve NULL si la conexión no ha podido ser establecida o si no se llamó a init() anteriormente.
	 */
	CConexion* connect(char* address, int port, int channels, unsigned int timeout = 5000);

	/**
	 * Devuelve una lista de las conexiones creadas 
	 */
	std::vector<CConexion*>::iterator listarConnexiones();

	/**
	 * Envía datos a través de una conexión.
	 * @param conexion por la que enviar los datos
	 * @param data son los datos a enviar
	 * @param longData tamaño de los datos a enviar
	 * @param channel canal lógico por el que se enviarán los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la máquina remota
	 */
	void sendData(CConexion* conexion, void* data, int longData, int channel, bool reliable);

	/**
	 * Refresca el cliente obteniendo todos los paquetes recibidos después de la última llamada a este método
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los paquetes recibidos
	 * @param paquetesRecibidos Vector de CPaquete* donde se almacenan los paquetes recibidos
	 */
	virtual void service(std::vector<CPaquete*>& paquetesRecibidos);

	/**
	 * Obtiene el siguiente paquete recibido
	 * IMPORTANTE: Es responsabilidad del invocador de readPacket() borrar el paquete recibido
	 * @return último paquete recibido
	 */
	virtual CPaquete* readPacket();

	/**
	 * Desconecta la conexión indicada.
	 */
	void disconnect(CConexion * conexion);

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Desconecta automáticamente todas las conexiones activas.
	 */ 
	void deInit();

	/**
	 * Indica si el cliente tiene alguna conexión activa.
	 */
	bool isConnected();

	/**
	 * Indica si el cliente está inicializado
	 */
	virtual bool isInitialized();

private:
	
	enum EstadoClienteENet{
		NO_INIT,
		INIT_NOT_CONNECTED,
		INIT_AND_CONNECTED
	};

	void disconnectAll();

	void disconnectReceived(CConexion* conexion);

	EstadoClienteENet estado;

	std::vector<CConexion*> listaConexiones;
	ENetHost* client;

};

} // namespace Net

#endif // __CLIENTE_H
