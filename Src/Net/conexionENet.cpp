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


#include "conexionENet.h"

namespace Net {

	void CConexionENet::setENetPeer(ENetPeer* p)
	{
		_peer = p;
	};

	ENetPeer* CConexionENet::getENetPeer()
	{
		return _peer;
	};

	void CConexionENet::setId(NetID id)
	{
		_id = id;
	}

	NetID CConexionENet::getId()
	{
		return _id;
	}

	int CConexionENet::getAddress()
	{
		return _peer->address.host;
	}

	short CConexionENet::getPort()
	{
		return _peer->address.port;
	}

} //namespace