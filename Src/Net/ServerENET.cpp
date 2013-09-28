//---------------------------------------------------------------------------
// ServerENET.cpp
//---------------------------------------------------------------------------

/**
 * @file ServerENET.cpp
 *
 * Implementa un servidor de red con ENet
 *
 *
 * @author Juan A. Recio-García
 * @date Junio, 2006
 */

#include "ServerENET.h"

#include "ConnectionENET.h"

#define DEBUG 0

namespace Net {

	CServerENET::CServerENET()
	{
		_state = NO_INIT;
	}

	CServerENET::~CServerENET()
	{
		std::vector<CConnection*>::iterator it = _connectionsList.begin();
		while (it != _connectionsList.cend()) {
			delete (*it);
			++it;
		}
		_connectionsList.clear();
	}

	bool CServerENET::init(int port, int clients, unsigned int maxinbw, unsigned int maxoutbw)
	{
		if(_state != NO_INIT)
			return false;

		if (enet_initialize () != 0)
		{
			printf ("NET::SERVER>> An error occurred while trying to init ENet.\n");
			return false;
		}
		
		ENetAddress address;

		/* Bind the server to the default localhost.     */
		/* A specific host address can be specified by   */
		/* enet_address_set_host (& address, "x.x.x.x"); */

		address.host = ENET_HOST_ANY;
		/* Bind the server to port. */
		address.port = port;

		server = enet_host_create (& address      /* the address to bind the server host to */, 
									clients       /* allow up to 32 clients and/or outgoing connections */,
									maxinbw       /* assume any amount of incoming bandwidth */,
									maxoutbw      /* assume any amount of outgoing bandwidth */);
		if (server == NULL)
		{
			printf ("NET::SERVER>> An error occurred while trying to create an ENet server host.\n");
			return false;
		}

		if(DEBUG)
			fprintf(stdout, "NET::SERVER>> Server initialized");

		_state = INIT_NOT_CONNECTED;

		return true;
	}


	void CServerENET::service(std::vector<CPacket*>&  packetsRecibidos)
	{
		ENetEvent event;
		CPacket* packet;
		CConnectionENET* connection;   

		/* Wait up to 0 milliseconds for an event. */
		while (enet_host_service (server, & event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:

				if(DEBUG)
					fprintf(stdout,"NET::SERVER>> A new client connected from %x:%u.\n", 
							event.peer -> address.host,
							event.peer -> address.port);

				connection = new CConnectionENET();
					connection->setENetPeer(event.peer);				
					_connectionsList.push_back(connection);
					event.peer->data = connection;

				packet = new CPacket(CONNECT,0,0,connection,0);
					packetsRecibidos.push_back(packet);
				
				_state = INIT_AND_CONNECTED;
				
				break;

			case ENET_EVENT_TYPE_RECEIVE:

				if(DEBUG)
					fprintf(stdout,"NET::SERVER>> A packet of length %u was received from %s on channel %u.\n",
						event.packet->dataLength,
						event.peer->data,
						event.channelID);

				packet = new CPacket(DATA, event.packet->data, event.packet->dataLength, (CConnection*)event.peer->data, event.channelID);
					packetsRecibidos.push_back(packet);
					enet_packet_destroy (event.packet);            
				break;
	           
			case ENET_EVENT_TYPE_DISCONNECT:

				if(DEBUG)
					fprintf(stdout,"NET::SERVER>> %s disconected.\n", ( (CConnection*) event.peer->data)->getId() );

				packet = new CPacket(DISCONNECT,0,0,(CConnection*)event.peer->data,0);
					packetsRecibidos.push_back(packet);

				disconnectReceived((CConnection*)event.peer->data);
				break;

			default:
				fprintf(stdout,"NET::SERVER>> Unknown packet.\n");
			}
		}
	}


	std::vector<CConnection*>::iterator CServerENET::listConnections()
	{
		return _connectionsList.begin();
	}

	void CServerENET::deInit()
	{
		if(_state == INIT_AND_CONNECTED)
			disconnectAll();

		enet_host_destroy(server);
		atexit(enet_deinitialize);
		_state = NO_INIT;
	}
	

	void CServerENET::sendData(void* data, size_t longData, int channel, bool reliable, CConnection* connection)
	{
		if(!connection) 
			return;

		enet_uint32 rel = 0;
		if(reliable)
			rel = ENET_PACKET_FLAG_RELIABLE;
		
		ENetPacket * packet = enet_packet_create (data, longData, rel);
	    
		enet_peer_send ( static_cast<CConnectionENET*>(connection)->getENetPeer(), channel, packet);

		if(DEBUG)
			fprintf (stdout, "NET::SERVER>> Packet send.\n");
		enet_host_flush (server);
	}


	void CServerENET::sendAll(void* data, size_t longData, int channel, bool reliable, CConnection* exception)
	{
		enet_uint32 rel = 0;
		if(reliable)
			rel = ENET_PACKET_FLAG_RELIABLE;

		ENetPacket * packet = enet_packet_create (data,longData,rel);
	    
		for(std::vector<CConnection*>::const_iterator iter = _connectionsList.cbegin(); iter != _connectionsList.cend(); ++iter)	
			if( (*iter) != exception )
				enet_peer_send (static_cast<CConnectionENET*>(*iter)->getENetPeer(), channel, packet);		

		if(DEBUG)
			fprintf (stdout, "NET::SERVER>> Packet send.\n");
		enet_host_flush (server);
	}

	void CServerENET::disconnect(CConnection* connection)
	{
		ENetEvent event;
    
		enet_peer_disconnect (static_cast<CConnectionENET*>(connection)->getENetPeer(),0);

		/* Allow up to 3 seconds for the disconnect to succeed
			and drop any packets received packets.     */
		while (enet_host_service (server, & event, 3000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy (event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				if(DEBUG)
					fprintf (stdout, "NET::SERVER>> Disconnection succeeded.\n");
					disconnectReceived(connection);
				return;
			}
		}
	    
		/* We've arrived here, so the disconnect attempt didn't */
		/* succeed yet.  Force the connection down.             */
		enet_peer_reset (static_cast<CConnectionENET*>(connection)->getENetPeer());
		disconnectReceived(connection);

		if(DEBUG)
			fprintf(stdout, "NET::SERVER>> Disconnection Forced.\n");

		if(_connectionsList.empty())
			_state = INIT_NOT_CONNECTED;
	}


	void CServerENET::disconnectAll()
	{
		if(_state == INIT_AND_CONNECTED)
		{
			while(!_connectionsList.empty())
			{
				disconnect(_connectionsList.back());
			}
		}

		if(DEBUG)
			fprintf(stdout, "NET::SERVER>> Everything Disconnected.\n");

		_state = INIT_NOT_CONNECTED;
	}


	void CServerENET::disconnectReceived(CConnection* connection)
	{
		std::vector<CConnection*>::const_iterator it = _connectionsList.cbegin();
		bool found = false;
		while ((it != _connectionsList.cend()) && (!found)) {
			if(*it == connection)
				found=true;
			else
				++it;
		}
		if(found)
		{
			_connectionsList.erase(it);

			if(_connectionsList.empty())
				_state = INIT_NOT_CONNECTED;
		}
	}

	bool CServerENET::isConnected()
	{
		return (_state == INIT_AND_CONNECTED);
	}

	bool CServerENET::isInitialized()
	{
		return (_state != NO_INIT);
	}

} // namespace aplicacion
