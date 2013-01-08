//---------------------------------------------------------------------------
// conexion.h
//---------------------------------------------------------------------------

/**
 * @file conexion.h
 *
 * Representa una conexión con un peer.
 *
 *
 * @author Juan A. Recio-García
 * @date Octubre, 2006
 */

#ifndef __CONEXION_H
#define __CONEXION_H

#define DEBUG_CONEXION 1


#include <string>

namespace Net {

class CConexion {

public:
	virtual int getAddress()=0;
	virtual short getPort()=0;
	virtual void setId(const std::string& id)=0;
	virtual std::string getId()=0;
};


} // namespace Net

#endif // __CONEXION_H