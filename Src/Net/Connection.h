//---------------------------------------------------------------------------
// connection.h
//---------------------------------------------------------------------------

/**
 * @file connection.h
 *
 * Representa una conexión con un peer.
 *
 *
 * @author Juan A. Recio-García
 * @date Octubre, 2006
 */

#ifndef __Net_Connection_H
#define __Net_Connection_H

#include <string>

namespace Net {

	/** 
	ID de identificación en la red.
	*/
	typedef unsigned int NetID;

class CConnection {

public:
	virtual int getAddress()=0;
	virtual short getPort()=0;
	virtual void setId(NetID id)=0;
	virtual NetID getId()=0;
};


} // namespace Net

#endif // __Net_Connection_H