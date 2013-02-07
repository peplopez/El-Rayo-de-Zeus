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
	CConexionENet():_id(0xFFFFFFFF),_peer(0){}

	int getAddress();
	short getPort();

	void setId(NetID id);

	NetID getId();

    void setENetPeer(ENetPeer* p);

	ENetPeer* getENetPeer();
private:
	ENetPeer* _peer;
	NetID _id;
};


} // namespace Net

#endif // __CONEXION_H