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


#include "ConnectionENET.h"

namespace Net {

	void CConnectionENET::setENetPeer(ENetPeer* p)
	{
		_peer = p;
	};

	ENetPeer* CConnectionENET::getENetPeer()
	{
		return _peer;
	};

	void CConnectionENET::setId(NetID id)
	{
		_id = id;
	}

	NetID CConnectionENET::getId()
	{
		return _id;
	}

	int CConnectionENET::getAddress()
	{
		return _peer->address.host;
	}

	short CConnectionENET::getPort()
	{
		return _peer->address.port;
	}

} //namespace