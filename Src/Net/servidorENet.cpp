//---------------------------------------------------------------------------
// servidorENet.cpp
//---------------------------------------------------------------------------

/**
 * @file servidorENet.cpp
 *
 * Implementa un servidor de red con ENet
 *
 *
 * @author Juan A. Recio-García
 * @date Junio, 2006
 */

#include "servidorENet.h"
#include "conexionENet.h"

#define DEBUG_SERVER 0

namespace Net {

	CServidorENet::CServidorENet()
	{
		estado = NO_INIT;
	}

	CServidorENet::~CServidorENet()
	{
		std::vector<CConexion*>::iterator it = listaConexiones.begin();
		while (it != listaConexiones.end()) {
			delete (*it);
			++it;
		}
		listaConexiones.clear();
	}

	bool CServidorENet::init(int port, int clients, unsigned int maxinbw, unsigned int maxoutbw)
	{
		if(estado != NO_INIT)
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

		if(DEBUG_SERVER)
			fprintf(stdout, "NET::SERVER>> Server initialized");

		estado = INIT_NOT_CONNECTED;

		return true;
	}


	void CServidorENet::service(std::vector<CPaquete*>&  paquetesRecibidos)
	{
		ENetEvent event;
		CPaquete* paquete;
		CConexionENet* conexion;
   

		/* Wait up to 0 milliseconds for an event. */
		while (enet_host_service (server, & event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				if(DEBUG_SERVER)
					fprintf(stdout,"NET::SERVER>> A new client connected from %x:%u.\n", 
							event.peer -> address.host,
							event.peer -> address.port);

				conexion = new CConexionENet();
				conexion->setENetPeer(event.peer);

				event.peer -> data = conexion;
				listaConexiones.push_back(conexion);


				paquete = new CPaquete(CONEXION,0,0,conexion,0);
				paquetesRecibidos.push_back(paquete);
				
				estado = INIT_AND_CONNECTED;
				
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				if(DEBUG_SERVER)
					fprintf(stdout,"NET::SERVER>> A packet of length %u was received from %s on channel %u.\n",
				event.packet -> dataLength,
				event.peer -> data,
				event.channelID);
				paquete = new CPaquete(DATOS, event.packet->data, event.packet->dataLength, (CConexion*)event.peer->data, event.channelID);
				paquetesRecibidos.push_back(paquete);
				enet_packet_destroy (event.packet);            
				break;
	           
			case ENET_EVENT_TYPE_DISCONNECT:
				if(DEBUG_SERVER)
					fprintf(stdout,"NET::SERVER>> %s disconected.\n", event.peer -> data);
				paquete = new CPaquete(DESCONEXION,0,0,(CConexion*)event.peer->data,0);
				paquetesRecibidos.push_back(paquete);
				disconnectReceived((CConexion*)event.peer->data);
				break;
			default:
				fprintf(stdout,"NET::SERVER>> Unknown packet.\n");
			}
		}
	}


	std::vector<CConexion*>::iterator CServidorENet::listarConnexiones()
	{
		return listaConexiones.begin();
	}

	void CServidorENet::deInit()
	{
		if(estado == INIT_AND_CONNECTED)
			disconnectAll();

		enet_host_destroy(server);
		atexit(enet_deinitialize);
		estado = NO_INIT;
	}
	

	void CServidorENet::sendData(CConexion* conexion, void* data, size_t longData, int channel, bool reliable)
	{
		enet_uint32 rel = 0;
		if(reliable)
			rel = ENET_PACKET_FLAG_RELIABLE;
		
		ENetPacket * packet =  enet_packet_create (data, longData, rel);
	    
		enet_peer_send (((CConexionENet*)conexion)->getENetPeer(), channel, packet);

		if(DEBUG_SERVER)
			fprintf (stdout, "NET::SERVER>> Packet send.\n");
		enet_host_flush (server);
	}


	void CServidorENet::sendAll(void* data, size_t longData, int channel, bool reliable)
	{
		enet_uint32 rel = 0;
		if(reliable)
			rel = ENET_PACKET_FLAG_RELIABLE;

		ENetPacket * packet = enet_packet_create (data,longData,rel);
	    
		for(std::vector<CConexion*>::iterator iter = listaConexiones.begin(); iter != listaConexiones.end(); ++iter)
		{
			enet_peer_send (((CConexionENet*)*iter)->getENetPeer(), channel, packet);
		}

		if(DEBUG_SERVER)
			fprintf (stdout, "NET::SERVER>> Packet send.\n");
		enet_host_flush (server);
	}

	void CServidorENet::disconnect(CConexion* conexion)
	{
		ENetEvent event;
    
		enet_peer_disconnect (((CConexionENet*)conexion)->getENetPeer(),0);

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
				if(DEBUG_SERVER)
					fprintf (stdout, "NET::SERVER>> Disconnection succeeded.\n");
					disconnectReceived(conexion);
				return;
			}
		}
	    
		/* We've arrived here, so the disconnect attempt didn't */
		/* succeed yet.  Force the connection down.             */
		enet_peer_reset (((CConexionENet*)conexion)->getENetPeer());
		disconnectReceived(conexion);

		if(DEBUG_SERVER)
			fprintf(stdout, "NET::SERVER>> Disconnection Forced.\n");

		if(listaConexiones.empty())
			estado = INIT_NOT_CONNECTED;
	}


	void CServidorENet::disconnectAll()
	{
		if(estado == INIT_AND_CONNECTED)
		{
			while(!listaConexiones.empty())
			{
				disconnect(listaConexiones.back());
			}
		}

		if(DEBUG_SERVER)
			fprintf(stdout, "NET::SERVER>> Everything Disconnected.\n");

		estado = INIT_NOT_CONNECTED;
	}


	void CServidorENet::disconnectReceived(CConexion* conexion)
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

	bool CServidorENet::isConnected()
	{
		return (estado == INIT_AND_CONNECTED);
	}

	bool CServidorENet::isInitialized()
	{
		return (estado != NO_INIT);
	}

} // namespace aplicacion
