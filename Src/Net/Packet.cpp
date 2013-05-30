//---------------------------------------------------------------------------
// packet.cpp
//---------------------------------------------------------------------------

/**
 * @file packet.cpp
 *
 * Representación de los datos transmitidos por red
 *
 *
 * @author Juan A. Recio-García
 * @date Octumbre, 2006
 */

#include "string.h"
#include "Packet.h"


namespace Net {

	CPacket::CPacket( PacketType tipo, 
						byte* data, 
						size_t dataLength, 
						CConnection* connection, 
						unsigned char canal)
	{
		_data = 0;
		_dataLength = 0;
		_packetType = tipo;
		setData(data,dataLength);
		_connection = connection;
		_canal = canal;	
	}

	CPacket::~CPacket()
	{
		if(_data != 0)
			delete[] _data;
	}

	byte* CPacket::getData()
	{
		return _data;
	}

	void CPacket::setData(byte* data, size_t length)
	{
		if (_data != 0)
			delete[] _data;
	
		if((data == 0)||(length == 0))
			return;

		_data = new byte[length];
		memcpy(_data, data, length);
		_dataLength = length;
	}

	size_t CPacket::getDataLength()
	{
		return _dataLength;
	}


	PacketType CPacket::getTipo()
	{
		return _packetType;
	}

	void CPacket::setTipo(PacketType tipo)
	{
		_packetType = tipo;
	}

	CConnection* CPacket::getConexion()
	{
		return _connection;
	}

	void CPacket::setConexion(CConnection* connection)
	{
		_connection = connection;
	}

	void CPacket::setCanal(unsigned char canal)
	{
		_canal = canal;
	}

	unsigned char CPacket::getCanal()
	{
		return _canal;
	}

} // namespace Net