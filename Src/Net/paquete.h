//---------------------------------------------------------------------------
// paquete.h
//---------------------------------------------------------------------------

/**
 * @file paquete.h
 *
 * Representación de los datos transmitidos por red
 *
 *
 * @author Juan A. Recio-García
 * @date Octumbre, 2006
 */

#ifndef __PAQUETE_H
#define __PAQUETE_H

#include "conexion.h"

namespace Net {

	/**
	 * Los paquetes manejarán bytes
	 */
	typedef unsigned char byte;

	/**
	 * Identificador de paquetes
	 */
    enum TipoPaquete{
		DATOS,
		CONEXION,
		DESCONEXION,
		CONTROL
	};

/**
 * Clase que representa de los datos transmitidos por red
 */
class CPaquete {

public:

	/**
	 * Constructora. Crea un paquete de datos.
	 * @param tipo del paquete
	 * @param data es el contenido del paquete
	 * @param dataLength es el tamaño de los datos (data)
	 * @param conexion es la conexión por la que se transmiten
	 * @param canan es el canal por el que se transmiten
	 */
	CPaquete(TipoPaquete tipo, byte* data, size_t dataLength, CConexion* conexion, unsigned char canal);

	/**
	 * Destructura
	 */
	~CPaquete();

	/**
	 * Obtener los datos
	 */
	byte* getData();

	/**
	 * Establecer el contenido del paquete
	 */
	void setData(byte* data, size_t length);

	/**
	 * Obtener el tamaño de los datos del paquete
	 */ 
	size_t getDataLength();

	/**
	 * Obtener el tipo de los datos
	 */
	TipoPaquete getTipo();

	/**
	 * Establecer el tipo de los datos
	 */
	void setTipo(TipoPaquete tipo);

	/**
	 * Establecer la conexión que transmite los datos
	 */ 
	void setConexion(CConexion* conexion);

	/**
	 * Obtener la conexión que transmite los datos
	 */ 
	CConexion* getConexion();

	/**
	 * Establecer el canal que transmite los datos
	 */ 
	void setCanal(unsigned char canal);

	/**
	 * Obtener el canal que transmite los datos
	 */ 
	unsigned char getCanal();

protected:
	unsigned char _canal;
	size_t _dataLength;
	byte* _data;
	TipoPaquete _tipoPaquete;
	CConexion* _conexion;

};


} //namespace Net

#endif