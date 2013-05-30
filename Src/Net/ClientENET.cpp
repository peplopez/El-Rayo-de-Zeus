//---------------------------------------------------------------------------
// ClientENET.cpp
//---------------------------------------------------------------------------

/**
 * @file ClientENET.cpp
 *
 * Implemente un cliente de red con ENet
 *
 * @author  Juan A. Recio Garc?a
 * @date Junio, 2006
 */

#include <iostream>

#include "ClientENET.h"
#include "ConnectionENET.h"

#define DEBUG 0

namespace Net {

	CClientENET::CClientENET()
	{
		estado = NO_INIT;
	}


	CClientENET::~CClientENET()
	{
		std::vector<CConnection*>::iterator it = _connectionsList.begin();
		while (it != _connectionsList.end()) {
			delete (*it);
			++it;
		}
		_connectionsList.clear();
	}

	bool CClientENET::init(unsigned int maxConnections, unsigned int maxinbw, unsigned int maxoutbw)
	{
		if(estado != NO_INIT)
			return false;

		if (enet_initialize () != 0)
		{
			printf ("NET::CLIENT>> An error occurred while initializing ENet.\n");
			return false;

		}

		client = enet_host_create (NULL /* create a client host */,
                maxConnections, /* only allow 1 outgoing connection */
                maxinbw ,
                maxoutbw );

		if (client == NULL)
		{
			printf ("NET::CLIENT>> An error occurred while trying to create an ENet client host.\n");
			return false;
		}
		if(DEBUG)
			fprintf(stdout, "NET::CLIENT>> Client initialized.\n");
	
		estado = INIT_NOT_CONNECTED;

		return true;
	}

	CConnection* CClientENET::connect(char* address, int port, int channels, unsigned int timeout)
	{
		if(estado = NO_INIT)
			return NULL;

		ENetAddress _address;
		ENetEvent event;
		CConnectionENET* connection = 0;

		/* Connect */
		enet_address_set_host (& _address, address);
		_address.port = port; 

		ENetPeer* peer = enet_host_connect (client, & _address, channels);    
	    
		if (peer == NULL)		
			printf ("NET::CLIENT>> No available peers for initiating an ENet connection.\n");
		
		else
		{
			/* Wait up to 5 seconds for the connection attempt to succeed.*/
			if (enet_host_service (client, & event, timeout) > 0 &&
				event.type == ENET_EVENT_TYPE_CONNECT)
			{		
				if(DEBUG)
					fprintf(stdout, "NET::CLIENT>> Connection succeeded.\n");

				connection = new CConnectionENET();
					connection->setENetPeer(peer);				
					_connectionsList.push_back(connection);
					event.peer->data = connection;
			}
			else
			{
				/* Either the 5 seconds are up or a disconnect event was */
				/* received. Reset the peer in the event the 5 seconds   */
				/* had run out without any significant event.            */
				enet_peer_reset (peer);

				printf ("NET::CLIENT>> Connection  failed.\n");
			}
		}

	    enet_host_flush (client);
		estado = INIT_AND_CONNECTED;

		return connection;
	}

	std::vector<CConnection*>::iterator CClientENET::listConnections()
	{
		return _connectionsList.begin();
	}

	void CClientENET::sendData(void* data, int longData, int channel, bool reliable, CConnection* connection)
	{
		if(!connection) 
			return;
		
		enet_uint32 rel = 0;
		if(reliable)
			rel = ENET_PACKET_FLAG_RELIABLE;
		
		ENetPacket * packet = enet_packet_create (data, longData, rel);
	    
		enet_peer_send ( static_cast<CConnectionENET*>(connection)->getENetPeer(), channel, packet);

		if(DEBUG)
			fprintf (stdout, "NET::CLIENT>> Packet send .\n");
		enet_host_flush (client);
		
	}


	void CClientENET::service(std::vector<CPacket*>&  paquetesRecibidos)
	{
		ENetEvent event;
		CPacket* paquete;
    
		if(!isConnected())
			return;

		/* Wait up to 0 milliseconds for an event. */
		while (enet_host_service (client, & event, 0) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					if(DEBUG)
						printf ("NET::CLIENT>> A packet of length %u was received from %s on channel %u.\n",
							event.packet->dataLength,
							event.peer->data,
							event.channelID);					

					paquete = new CPacket(DATA, event.packet->data, event.packet->dataLength, (CConnection*)event.peer->data, event.channelID);
					paquetesRecibidos.push_back(paquete);

					enet_packet_destroy (event.packet);            
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					if(DEBUG)
						fprintf(stdout,"NET::CLIENT>> Server disconected.\n");
					paquete = new CPacket(DISCONNECT, 0,0,(CConnection*)event.peer->data,0);
					paquetesRecibidos.push_back(paquete);
					disconnectReceived((CConnection*)event.peer->data);
					break;		           

				default:
					puts("NET::CLIENT>> Unknown packet.");
			}
		}
	}

	CPacket* CClientENET::readPacket()
	{
		ENetEvent event;
		CPacket *packet = NULL;
		enet_host_service (client, & event, 0);		
		switch (event.type)	{
				case ENET_EVENT_TYPE_RECEIVE:
					packet = new CPacket(DATA, event.packet->data, event.packet->dataLength, (CConnection*)event.peer->data, event.channelID);
					enet_packet_destroy (event.packet);            
					break;		           
				case ENET_EVENT_TYPE_DISCONNECT:
					if(DEBUG)
						fprintf(stdout,"NET::CLIENT>> Server disconected.\n");
					packet = new CPacket(DISCONNECT,0,0,(CConnection*)event.peer->data,0);
					disconnectReceived((CConnection*)event.peer->data);
					break;		           
			}	
		return packet;
	}

	void CClientENET::disconnect(CConnection* connection)
	{
		ENetEvent event;
    
		enet_peer_disconnect ( static_cast<CConnectionENET*>(connection)->getENetPeer(),0);

		/* Allow up to 3 seconds for the disconnect to succeed
			and drop any packets received packets.     */
		while (enet_host_service (client, & event, 3000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy (event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				if(DEBUG)
					fprintf (stdout, "NET::CLIENT>> Disconnection succeeded.\n");
					disconnectReceived(connection);
				return;
			}
		}
	    
		/* We've arrived here, so the disconnect attempt didn't */
		/* succeed yet.  Force the connection down.             */
		enet_peer_reset (((CConnectionENET*)connection)->getENetPeer());
		disconnectReceived(connection);

		if(DEBUG)
			fprintf(stdout, "NET::CLIENT>> Disconnection Forced.\n");

		if(_connectionsList.empty())
			estado = INIT_NOT_CONNECTED;
	}

	void CClientENET::disconnectAll()
	{
		if(estado == INIT_AND_CONNECTED)
		{
			while(!_connectionsList.empty())
			{
				disconnect(_connectionsList.back());
			}
		}

		if(DEBUG)
			fprintf(stdout, "NET::CLIENT>> Everything Disconnected.\n");

		estado = INIT_NOT_CONNECTED;
	}

	void CClientENET::deInit()
	{
		if(estado == INIT_AND_CONNECTED)
			disconnectAll();

		enet_host_destroy(client);
		atexit (enet_deinitialize);
		estado = NO_INIT;
	}

	bool CClientENET::isConnected()
	{
		return (estado == INIT_AND_CONNECTED);
	}

	bool CClientENET::isInitialized()
	{
		return (estado != NO_INIT);
	}

	void CClientENET::disconnectReceived(CConnection* connection)
	{
		std::vector<CConnection*>::iterator it = _connectionsList.begin();
		bool found = false;
		while ((it != _connectionsList.end()) && (!found)) {
			if(*it == connection)
				found=true;
			else
				++it;
		}
		if(found)
		{
			_connectionsList.erase(it);

			if(_connectionsList.empty())
				estado = INIT_NOT_CONNECTED;
		}
	}

} // namespace aplicacion
