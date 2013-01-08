//---------------------------------------------------------------------------
// conexionENet.h
//---------------------------------------------------------------------------

/**
 * @file conexionENet.h
 *
 * Representa una conexión con un peer implementada con ENet.
 *
 *
 * @author Juan A. Recio-García
 * @date Octubre, 2006
 */

#ifndef __CONEXIONENET_H
#define __CONEXIONENET_H

#define DEBUG_CONEXION 1

#include "enet/enet.h"
#include "conexion.h"

namespace Net {

class CConexionENet: public CConexion{

public:

	int getAddress();
	short getPort();

	void setId(const std::string& id);

	std::string getId();

    void setENetPeer(ENetPeer* p);

	ENetPeer* getENetPeer();
private:
	ENetPeer* _peer;
	std::string _id;
};


} // namespace Net

#endif // __CONEXION_H