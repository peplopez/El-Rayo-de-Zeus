//---------------------------------------------------------------------------
// ConnectionENET.h
//---------------------------------------------------------------------------

/**
 * @file ConnectionENET.h
 *
 * Representa una conexión con un peer implementada con ENet.
 *
 *
 * @author Juan A. Recio-García
 * @date Octubre, 2006
 */

#ifndef __Net_ConnnectionENET_H
#define __Net_ConnnectionENET_H

#include "enet/enet.h"
#include "Connection.h"

namespace Net {

class CConnectionENET: public CConnection{

public:
	CConnectionENET():_id(0xFFFFFFFF),_peer(0){}

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

#endif // __Net_ConnnectionENET_H