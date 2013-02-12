//---------------------------------------------------------------------------
// paquete.cpp
//---------------------------------------------------------------------------

/**
 * @file paquete.cpp
 *
 * Representación de los datos transmitidos por red
 *
 *
 * @author Juan A. Recio-García
 * @date Octumbre, 2006
 */

#include "paquete.h"
#include "string.h"

namespace Net {

	CPaquete::CPaquete( TipoPaquete tipo, 
						byte* data, 
						size_t dataLength, 
						CConexion* conexion, 
						unsigned char canal)
	{
		_data = 0;
		_dataLength = 0;
		_tipoPaquete = tipo;
		setData(data,dataLength);
		_conexion = conexion;
		_canal = canal;	
	}

	CPaquete::~CPaquete()
	{
		if(_data != 0)
			delete[] _data;
	}

	byte* CPaquete::getData()
	{
		return _data;
	}

	void CPaquete::setData(byte* data, size_t length)
	{
		if (_data != 0)
			delete[] _data;
	
		if((data == 0)||(length == 0))
			return;

		_data = new byte[length];
		memcpy(_data, data, length);
		_dataLength = length;
	}

	size_t CPaquete::getDataLength()
	{
		return _dataLength;
	}


	TipoPaquete CPaquete::getTipo()
	{
		return _tipoPaquete;
	}

	void CPaquete::setTipo(TipoPaquete tipo)
	{
		_tipoPaquete = tipo;
	}

	CConexion* CPaquete::getConexion()
	{
		return _conexion;
	}

	void CPaquete::setConexion(CConexion* conexion)
	{
		_conexion = conexion;
	}

	void CPaquete::setCanal(unsigned char canal)
	{
		_canal = canal;
	}

	unsigned char CPaquete::getCanal()
	{
		return _canal;
	}

} // namespace Net