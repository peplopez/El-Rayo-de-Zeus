//---------------------------------------------------------------------------
// serializable.h
//---------------------------------------------------------------------------

/**
 * @file serializable.h
 *
 * Interfaz para objetos serializables
 *
 *
 * @author Juan A. Recio-García
 * @date Junio, 2006
 */

#ifndef __SERIALIZABLE_H
#define __SERIALIZABLE_H

#include "buffer.h"
#include <string>

namespace Net {

/**
 * Los objetos serializables saben cómo leerse y escribirse en un buffer.
 * Además se incluyen dos funciones auxiliares para serializar cadenas. 
 */
class Serializable{
public:
	/**
	 * Serializar objeto en buffer
	 */
	virtual void serialize(CBuffer& buffer)=0;

	/**
	 * Leer objeto desde buffer
	 */
	virtual void deserialize(CBuffer& buffer)=0;

protected:
	/**
	 * Serializar cadena en buffer
	 */
	void serializeString(CBuffer& buffer,std::string cad);

	/**
	 * Leer cadena desde buffer
	 */
	std::string deserializeString(CBuffer& buffer);


};



} // namespace Net

#endif // __SERIALIZABLE_H