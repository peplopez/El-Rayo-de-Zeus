//---------------------------------------------------------------------------
// buffer.cpp
//---------------------------------------------------------------------------

/**
 * @file buffer.cpp
 *
 * Implementa un buffer dinámico que los objetos utilizan para serializarse o deserializarse
 *
 *
 * @author Juan A. Recio-García
 * @date Octubre, 2006
 */

#include "buffer.h"
#include "memory.h"


namespace Net {

	CBuffer::CBuffer(size_t initsize, size_t delta)
	{
		_current = _begin = new byte[initsize]; // posicion actual e inicial del buffer
		_maxsize = initsize;
		_size = 0;
		_delta = delta;
	}

	CBuffer::~CBuffer()
	{
		delete[] _begin;
	}

	byte* CBuffer::getbuffer()
	{
		return _begin;
	}

	size_t CBuffer::getSize()
	{
		return _size;
	}

	void CBuffer::reset()
	{
		_current = _begin;
	}

	void CBuffer::write(void* data, size_t datalength)
	{
		while(_size + datalength >= _maxsize) // Mientras no quepa "data"
			realloc();						 // Ampliamos n deltas de tamaño
		memcpy(_current,data,datalength);
		_current+=datalength;
		_size+=datalength;
	}

	void CBuffer::read(void* data,size_t datalength)
	{
		memcpy(data,_current,datalength); // Lee "datalength" bytes desde _current
		_current+=datalength;			  // desplaza current
	}

	void CBuffer::realloc() // Amplia tamaño asignado en cantidad = "delta"
	{
		byte* newbuffer = new byte[_maxsize+_delta]; // Reserva nuevo bloque
		memcpy(newbuffer,_begin, _size);			 // Copia datos al nuevo bloque
		delete[] _begin;							 // Borra antiguo 
		_begin = newbuffer;							 // y asigna nueva config.
		_current = _begin + _size;
		_maxsize+=_delta;	
	}

} // namespace Net