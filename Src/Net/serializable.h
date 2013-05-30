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

#include <string>

#include "buffer.h"


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

	/**
	 * Serializar cadena en buffer
	 */
	static void serialize(CBuffer& buffer,std::string cad);

	/**
	 * Leer cadena desde buffer
	 */
	static std::string deserializeString(CBuffer& buffer);


};



} // namespace Net

#endif // __SERIALIZABLE_H