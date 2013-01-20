//---------------------------------------------------------------------------
// servidorENet.h
//---------------------------------------------------------------------------

/**
 * @file servidorENet.h
 *
 * Implementa un servidor de red con ENet
 *
 *
 * @author Juan A. Recio-García
 * @date Junio, 2006
 */

#ifndef __SERVIDORENET_H
#define __SERVIDORENET_H


#include "servidor.h"
#include "enet/enet.h"

namespace Net {

/**
 * Servidor de red implementado mediante la librería eNet
 */
class CServidorENet: public CServidor {

public:

	/**
	 * Constructor
	 */ 
	CServidorENet();

	/**
	 * Destructor
	 */
	virtual ~CServidorENet();

	/**
	 * Inicializa el servidor escuchando en un puerto determinado.
	 * @param port es el puerto donde se escucha
	 * @param clients es el número máximo de clientes. El máximo será establecido por la implementación.
	 * @param maxinbw Ancho de banda de entrada máximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida máximo en bytes/segundo (0 = ilimitado)
	 */
	bool init(int port, int clients, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);
	
	/**
	 * Refresca el servidor obteniendo todos los paquetes recibidos después de la última llamada a este método
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los paquetes recibidos
	 * @param paquetesRecibidos Vector de CPaquete* donde se almacenan los paquetes recibidos
	 */
	void service(std::vector<CPaquete*>& paquetesRecibidos);

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Es responsabilidad de este método desconectar todas las conexiones en este punto.
	 */ 
	void deInit();
	
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
	void sendData(void* data, size_t longData, int channel, bool reliable, CConexion* conexion);

	/**
	 * Envía el mismo dato a todos los clientes conectados
	 * @param data son los datos a enviar
	 * @param longData tamaño de los datos a enviar
	 * @param channel canal lógico por el que se enviarán los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la máquina remota
	 */
	void sendAll(void* data, size_t longData, int channel, bool reliable);

	/**
	 * Desconecta la conexión indicada.
	 */
	void disconnect(CConexion * conexion);

	/**
	 * Indica si el cliente tiene alguna conexión activa.
	 */
	bool isConnected();

	/**
	 * Indica si el cliente está inicializado
	 */
	bool isInitialized();

private:

	enum EstadoServidorENet{
		NO_INIT,
		INIT_NOT_CONNECTED,
		INIT_AND_CONNECTED
	};

	void disconnectAll();

	void disconnectReceived(CConexion* conexion);
	
	EstadoServidorENet estado;

	std::vector<CConexion*> listaConexiones;

	ENetHost* server;
};

} // namespace Net

#endif // __SERVIDOR_H
