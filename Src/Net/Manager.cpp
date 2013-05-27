/**
@file Manager.cpp

Contiene la implementación de la clase CManager, Singleton que se encarga de
la gestión de la red del juego.

@see Net::CManager

@author David Llansó
@date Diciembre, 2010
*/

#include "Manager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Net/Buffer.h"
#include "Net/FactoryENET.h"
#include "Net/IObserver.h"


#include <cassert>

#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "NET::MANAGER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace Net {

	CManager* CManager::_instance = 0;

	//--------------------------------------------------------

	CManager::CManager(): _netFactory(0), _netServer(0), _netClient(0),
		_id(Net::ID::UNASSIGNED),_nextId(Net::ID::FIRSTCLIENT)
	{
		_instance = this;

	} // CManager

	//--------------------------------------------------------

	CManager::~CManager()
	{
		_instance = 0;

	} // ~CManager
	
	//--------------------------------------------------------

	bool CManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Net::CManager no permitida!");

		new CManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CManager::Release()
	{
		assert(_instance && "Net::CManager no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
			_instance = 0;
		}

	} // Release

	//--------------------------------------------------------

	bool CManager::open()
	{
		 // Inicializamos la factoria de objetos de red
		_netFactory = new Net::CFactoryENET();

		return true;

	} // open

	//--------------------------------------------------------

	void CManager::close() 
	{
		deactivateNetwork();
		if(_netFactory)
			delete _netFactory;

	} // close

	//---------------------------------------------------------


	void CManager::send(void* data, size_t longdata, CConnection* exception) 
	{
		send(data, longdata, true, exception);
	} // send overload 1

	//---------------------------------------------------------

	void CManager::send(void* data, size_t longdata, bool reliable, CConnection* exception) 
	{
		if(_connections.empty())
			return;
#if DEBUG
		if(exception)  
			LOG("Send: Exception NetID = " << exception->getId());
#endif

		// [f®§] Hay más de una conexión, debemos mandar el mensaje por todas si somos servidor
		if(_netServer)
			_netServer->sendAll( data, longdata, 0, reliable, exception);
		if(_netClient) 
		{
			CConnection* serverConnection( getConnection(Net::ID::SERVER) );	
			if( serverConnection != exception) 
				_netClient->sendData( data, longdata, 0, reliable, serverConnection);
		}

	} // send base

	//---------------------------------------------------------

	void CManager::tick(unsigned int msecs) 
	{
		_packets.clear();
		getPackets(_packets);

		for(std::vector<Net::CPacket*>::iterator iterp = _packets.begin();iterp != _packets.end();++iterp)
		{
			Net::CPacket* packet = *iterp;			
			switch (packet->getTipo())
			{
				case Net::CONNECT:
					connect(packet->getConexion());
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->connexionPacketReceived(packet);
					break;
				case Net::DATA:
					if(!isMsgAssignID(packet)) // Comprueba si es un MSG de asignación de NetID y guarda ID en tal caso.
						for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
							(*iter)->dataPacketReceived(packet);
					break;
				case Net::DISCONNECT:
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->disconnexionPacketReceived(packet);
					disconnect(packet->getConexion());
					break;
			}
			delete packet;
		}

	} // tick

	//---------------------------------------------------------


	void CManager::activateAsServer(int port, int clients, unsigned int maxinbw, unsigned int maxoutbw)
	{
		//Creamos el servidor de red
		_netServer = _netFactory->buildServer();

		_netServer->init(port,clients, maxinbw, maxoutbw);

		_id = Net::ID::SERVER;

	} // activateAsServer

	//---------------------------------------------------------

	void CManager::activateAsClient(unsigned int maxConnections, unsigned int maxinbw, unsigned int maxoutbw)
	{
		//Creamos el servidor de red
		_netClient = _netFactory->buildClient();
		_netClient->init(maxConnections, maxinbw, maxoutbw);

	} // activateAsClient

	//---------------------------------------------------------

	//PT
	void CManager::connectTo(char* address, int port, int channels, unsigned int timeout)
	{
		assert(_netClient && "Cliente Red es null"); // Solo se puede ejecutar el connectTo si somos cliente
		assert(_connections.empty() && "Ya hay una connection"); // Capamos al cliente a 1 conexión max: la de con el server

		CConnection* connection = _netClient->connect(address, port, channels,timeout); // CONNECT
		
		// Almacenamos esa conexión y le otorgamos un ID de red
		connection->setId(Net::ID::SERVER); // Un cliente sólo se conecta al SERVER
		addConnection(Net::ID::SERVER, connection); // Guardamos en la tabla
	} // connectTo

	//---------------------------------------------------------

	bool CManager::isMsgAssignID(Net::CPacket* packet)
	{
		Net::CBuffer data;
			data.write(packet->getData(),packet->getDataLength());
			data.reset();
		
		Net::NetMessageType msg;
			data.read(&msg,sizeof(msg));
		switch (msg)
		{
		case Net::ASSIGN_ID: 			
			data.read(&_id,sizeof(_id));// Escribimos el id de red que corresponde a este cliente
			return true;
		default:
			return false;
		}

	} // internalData

	//---------------------------------------------------------

	void CManager::connect(CConnection* connection)// Una nueva conexión de un cliente al sevidor
	{
		// Almacenamos esa conexión y le otorgamos un ID de red
		connection->setId(_nextId);
		addConnection(_nextId,connection);

		NetMessageType type = Net::ASSIGN_ID;// Escribimos el tipo de mensaje de red a enviar
		CBuffer buf;// Avisamos al cliente de cual es su nuevo ID	
			buf.write(&type,sizeof(type));		
			buf.write(&_nextId,sizeof(_nextId));// Escribimos el id del cliente
		_netServer->sendData(buf.getbuffer(),buf.getSize(),0,1, connection);

		// Preparamos para la siguiente conexión
		_nextId = ID::NextID(_nextId);

	} // connect

	//---------------------------------------------------------

	void CManager::disconnect(CConnection* connection)
	{
		if(_netServer)
		{
			_netServer->disconnect(connection);
			removeConnection(connection->getId());
		}
		else if(_netClient)
		{
			_netClient->disconnect(getConnection(Net::ID::SERVER));
			removeConnection(Net::ID::SERVER);
		}

	} // disconnect
		
	//---------------------------------------------------------

	CConnection* CManager::getConnection(NetID id) 
	{
		if( id == ID::UNASSIGNED)
			return 0;
		else if(_connections.find(id) != _connections.end()) // TODO: optimizacion: si somos cliente, devolver el primero directamente? vector vs map?
			return _connections[id];
		else
			return 0;
	} // getConnection
		
	//---------------------------------------------------------

	bool CManager::addConnection(NetID id, CConnection* connection) 
	{
		if(_connections.count(id))
			return false;
		TConnectionPair elem(id,connection);
		_connections.insert(elem); // Insertamos par id - connection
		return true;

	} // addConection
		
	//---------------------------------------------------------
	
	bool CManager::removeConnection(NetID id) 
	{
		if(_connections.count(id))
		{
			CConnection* connection = getConnection(id);
			_connections.erase(id);
			delete connection;
			return true;
		}
		return false;

	} // removeConection

	//---------------------------------------------------------

	void CManager::deactivateNetwork()
	{
		if(_netServer)
		{
			_netServer->deInit(); // TODO analizar que haga disconnect de todo; antes if(_connection) disconnect (OJO estaba comentado)
			delete _netServer;
			_netServer = 0;
		}
		if(_netClient)
		{
			_netClient->deInit();
			delete _netClient;
			_netClient = 0;
		}
		if(!_connections.empty())
		{
			for(TConnectionTable::const_iterator it = _connections.begin(); it != _connections.end(); it++)
				delete (*it).second;
			_connections.clear(); // Quien hace el disconnect
		}
	} // deactivateNetwork

	//---------------------------------------------------------

	void CManager::getPackets(std::vector<Net::CPacket*>& _packets)
	{
		if(_netServer)
			_netServer->service(_packets);
		if(_netClient)
			_netClient->service(_packets);

	} // getPackets

	//---------------------------------------------------------

	void CManager::addObserver(IObserver* listener)
	{
		_observers.push_back(listener);

	} // addObserver

	//---------------------------------------------------------

	void CManager::removeObserver(IObserver* listener)
	{
		for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
			if((*iter)==listener)
			{
				_observers.erase(iter);
				break;
			}
	} // removeObserver

	//---------------------------------------------------------
	
	namespace ID
	{
		/**
		TODO gestionar ids libres para que siempre < 8;
			Devuelve el siguiente ID dado el anterior.

			@param id Último ID asignado.
			@return Nuevo ID.
		*/
		NetID NextID(const NetID &id)
		{
			NetID ret = id + 1;
			assert( id != UNASSIGNED && id != SERVER && 
					ret != UNASSIGNED && ret != SERVER);
			return ret;
		
		} // NextClientID

	} // namespace ID

} // namespace Net
