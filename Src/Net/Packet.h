//---------------------------------------------------------------------------
// packet.h
//---------------------------------------------------------------------------

/**
 * @file packet.h
 *
 * Representación de los datos transmitidos por red
 *
 *
 * @author Juan A. Recio-García
 * @date Octumbre, 2006
 */

#ifndef __Net_Packet_H
#define __Net_Packet_H

#include "Connection.h"

namespace Net {

	/**
	 * Los packets manejarán bytes
	 */
	typedef unsigned char byte;

	/**
	 * Identificador de packets
	 */
    enum PacketType{
		DATA,
		CONNECT,
		DISCONNECT,
		CONTROL
	};

/**
 * Clase que representa de los datos transmitidos por red
 */
class CPacket {

public:

	/**
	 * Constructora. Crea un packet de datos.
	 * @param tipo del packet
	 * @param data es el contenido del packet
	 * @param dataLength es el tamaño de los datos (data)
	 * @param connection es la conexión por la que se transmiten
	 * @param canan es el canal por el que se transmiten
	 */
	CPacket(PacketType tipo, byte* data, size_t dataLength, CConnection* connection, unsigned char channel);

	/**
	 * Destructura
	 */
	~CPacket();

	/**
	 * Obtener los datos
	 */
	byte* getData();

	/**
	 * Establecer el contenido del packet
	 */
	void setData(byte* data, size_t length);

	/**
	 * Obtener el tamaño de los datos del packet
	 */ 
	size_t getDataLength();

	/**
	 * Obtener el tipo de los datos
	 */
	PacketType getType();

	/**
	 * Establecer el tipo de los datos
	 */
	void setType(PacketType type);

	/**
	 * Establecer la conexión que transmite los datos
	 */ 
	void setConnection(CConnection* connection);

	/**
	 * Obtener la conexión que transmite los datos
	 */ 
	CConnection* getConnection();

	/**
	 * Establecer el canal que transmite los datos
	 */ 
	void setChannel(unsigned char channel);

	/**
	 * Obtener el canal que transmite los datos
	 */ 
	unsigned char getChannel();

protected:
	unsigned char _channel;
	size_t _dataLength;
	byte* _data;
	PacketType _packetType;
	CConnection* _connection;

};


} //namespace Net

#endif