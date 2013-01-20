//---------------------------------------------------------------------------
// servidor.h
//---------------------------------------------------------------------------

/**
 * @file servidor.h
 *
 * Especifica un servidor de red.
 *
 *
 * @author Juan A. Recio-García
 * @date Junio, 2006
 */

#ifndef __SERVIDOR_H
#define __SERVIDOR_H


#include "paquete.h"
#include "conexion.h"
#include <vector>

namespace Net {
/**
 * Especifica un servidor de red.
 * Esta interfaz podrá ser implementada de distintas maneras o con distintas librerías. 
 * Con ella se pretende representar el comportamiento de un servidor de red genérico 
 * e independiente de la implementación.
 */
class CServidor {

public:

	/**
	 * Destructor virtual.
	 */
	virtual ~CServidor(){}

	/**
	 * Inicializa el servidor escuchando en un puerto determinado.
	 * @param port es el puerto donde se escucha
	 * @param clients es el número máximo de clientes. El máximo será establecido por la implementación.
	 * @param maxinbw Ancho de banda de entrada máximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida máximo en bytes/segundo (0 = ilimitado)
	 */
	virtual bool init(int port, int clients, unsigned int maxinbw = 0, unsigned int maxoutbw = 0)=0;
	
	/**
	 * Refresca el servidor obteniendo todos los paquetes recibidos después de la última llamada a este método
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los paquetes recibidos
	 * @param paquetesRecibidos Vector de CPaquete* donde se almacenan los paquetes recibidos
	 */
	virtual void service(std::vector<CPaquete*>& paquetesRecibidos)=0;

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Es responsabilidad de este método desconectar todas las conexiones en este punto.
	 */ 
	virtual void deInit()=0;

	/**
	 * Devuelve una lista de las conexiones creadas 
	 */
	virtual std::vector<CConexion*>::iterator listarConnexiones()=0;

	/**
	 * Envía datos a través de una conexión.
	 * @param conexion por la que enviar los datos
	 * @param data son los datos a enviar
	 * @param longData tamaño de los datos a enviar
	 * @param channel canal lógico por el que se enviarán los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la máquina remota
	 */
	virtual void sendData(void* data, size_t longData, int channel, bool reliable, CConexion* conexion)=0;

	/**
	 * Envía el mismo dato a todos los clientes conectados
	 * @param data son los datos a enviar
	 * @param longData tamaño de los datos a enviar
	 * @param channel canal lógico por el que se enviarán los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la máquina remota
	 */
	virtual void sendAll(void* data, size_t longData, int channel, bool reliable)=0;

	/**
	 * Desconecta la conexión indicada.
	 */
	virtual void disconnect(CConexion * conexion)=0;

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
