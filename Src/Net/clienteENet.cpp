//---------------------------------------------------------------------------
// clienteENet.cpp
//---------------------------------------------------------------------------

/**
 * @file clienteENet.cpp
 *
 * Implemente un cliente de red con ENet
 *
 * @author  Juan A. Recio García
 * @date Junio, 2006
 */
#include "clienteENet.h"
#include "conexionENet.h"
#include <iostream>

#define DEBUG_CLIENT 0

namespace Net {

	CClienteENet::CClienteENet()
	{
		estado = NO_INIT;
	}


	CClienteENet::~CClienteENet()
	{
		std::vector<CConexion*>::iterator it = listaConexiones.begin();
		while (it != listaConexiones.end()) {
			delete (*it);
			++it;
		}
		listaConexiones.clear();
	}

	bool CClienteENet::init(unsigned int maxConnections, unsigned int maxinbw, unsigned int maxoutbw)
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
		if(DEBUG_CLIENT)
			fprintf(stdout, "NET::CLIENT>> Client initialized.\n");
	
		estado = INIT_NOT_CONNECTED;

		return true;
	}

	CConexion* CClienteENet::connect(char* address, int port, int channels, unsigned int timeout)
	{
		if(estado = NO_INIT)
			return NULL;

		ENetAddress _address;
		ENetEvent event;
		CConexionENet* conexion = 0;

		/* Connect */
		enet_address_set_host (& _address, address);
		_address.port = port;

		ENetPeer* peer = enet_host_connect (client, & _address, channels);    
	    
		if (peer == NULL)
		{
			printf ("NET::CLIENT>> No available peers for initiating an ENet connection.\n");
		}
		else
		{
			/* Wait up to 5 seconds for the connection attempt to succeed.*/
			if (enet_host_service (client, & event, timeout) > 0 &&
				event.type == ENET_EVENT_TYPE_CONNECT)
			{		
				if(DEBUG_CLIENT)
					fprintf(stdout, "NET::CLIENT>> Connection succeeded.\n");
				conexion = new CConexionENet();
				conexion -> setENetPeer(peer);
				listaConexiones.push_back(conexion);
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

		return conexion;
	}

	std::vector<CConexion*>::iterator CClienteENet::listarConnexiones()
	{
		return listaConexiones.begin();
	}

	void CClienteENet::sendData(CConexion* conexion, void* data, int longData, int channel, bool reliable)
	{
		enet_uint32 rel = 0;
		if(reliable)
			rel = ENET_PACKET_FLAG_RELIABLE;
		
		ENetPacket * packet = enet_packet_create (data, longData, rel);
	    
		enet_peer_send (((CConexionENet*)conexion)->getENetPeer(), channel, packet);

		if(DEBUG_CLIENT)
			fprintf (stdout, "NET::CLIENT>> Packet send .\n");
		enet_host_flush (client);
	}


	void CClienteENet::service(std::vector<CPaquete*>&  paquetesRecibidos)
	{
		ENetEvent event;
		CPaquete* paquete;
    
		if(!isConnected())
			return;

		/* Wait up to 0 milliseconds for an event. */
		while (enet_host_service (client, & event, 0) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					if(DEBUG_CLIENT)
						printf ("NET::CLIENT>> A packet of length %u was received from %s on channel %u.\n",
							event.packet->dataLength,
							event.peer->data,
							event.channelID);
					

					paquete = new CPaquete(DATOS, event.packet->data, event.packet->dataLength, (CConexion*)event.peer->data, event.channelID);
					paquetesRecibidos.push_back(paquete);

					enet_packet_destroy (event.packet);            
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					if(DEBUG_CLIENT)
						fprintf(stdout,"NET::CLIENT>> Server disconected.\n");
					paquete = new CPaquete(DESCONEXION,0,0,(CConexion*)event.peer->data,0);
					paquetesRecibidos.push_back(paquete);
					disconnectReceived((CConexion*)event.peer->data);
					break;		           
				default:
					puts("NET::CLIENT>> Unknown packet.");
			}
		}
	}

	CPaquete* CClienteENet::readPacket()
	{
		ENetEvent event;
		CPaquete *packet = NULL;
		enet_host_service (client, & event, 0);		
		switch (event.type)	{
				case ENET_EVENT_TYPE_RECEIVE:
					packet = new CPaquete(DATOS, event.packet->data, event.packet->dataLength, (CConexion*)event.peer->data, event.channelID);
					enet_packet_destroy (event.packet);            
					break;		           
				case ENET_EVENT_TYPE_DISCONNECT:
					if(DEBUG_CLIENT)
						fprintf(stdout,"NET::CLIENT>> Server disconected.\n");
					packet = new CPaquete(DESCONEXION,0,0,(CConexion*)event.peer->data,0);
					disconnectReceived((CConexion*)event.peer->data);
					break;		           
			}	
		return packet;
	}

	void CClienteENet::disconnect(CConexion* conexion)
	{
		ENetEvent event;
    
		enet_peer_disconnect (((CConexionENet*)conexion)->getENetPeer(),0);

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
				if(DEBUG_CLIENT)
					fprintf (stdout, "NET::CLIENT>> Disconnection succeeded.\n");
					disconnectReceived(conexion);
				return;
			}
		}
	    
		/* We've arrived here, so the disconnect attempt didn't */
		/* succeed yet.  Force the connection down.             */
		enet_peer_reset (((CConexionENet*)conexion)->getENetPeer());
		disconnectReceived(conexion);

		if(DEBUG_CLIENT)
			fprintf(stdout, "NET::CLIENT>> Disconnection Forced.\n");

		if(listaConexiones.empty())
			estado = INIT_NOT_CONNECTED;
	}

	void CClienteENet::disconnectAll()
	{
		if(estado == INIT_AND_CONNECTED)
		{
			while(!listaConexiones.empty())
			{
				disconnect(listaConexiones.back());
			}
		}

		if(DEBUG_CLIENT)
			fprintf(stdout, "NET::CLIENT>> Everything Disconnected.\n");

		estado = INIT_NOT_CONNECTED;
	}

	void CClienteENet::deInit()
	{
		if(estado == INIT_AND_CONNECTED)
			disconnectAll();

		enet_host_destroy(client);
		atexit (enet_deinitialize);
		estado = NO_INIT;
	}

	bool CClienteENet::isConnected()
	{
		return (estado == INIT_AND_CONNECTED);
	}

	bool CClienteENet::isInitialized()
	{
		return (estado != NO_INIT);
	}

	void CClienteENet::disconnectReceived(CConexion* conexion)
	{
		std::vector<CConexion*>::iterator it = listaConexiones.begin();
		bool found = false;
		while ((it != listaConexiones.end()) && (!found)) {
			if(*it == conexion)
				found=true;
			else
				++it;
		}
		if(found)
		{
			listaConexiones.erase(it);

			if(listaConexiones.empty())
				estado = INIT_NOT_CONNECTED;
		}
	}

} // namespace aplicacion
