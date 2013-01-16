/**
@file Manager.cpp

Contiene la implementación de la clase CManager, Singleton que se encarga de
la gestión de la red del juego.

@see Net::CManager

@author David Llansó
@date Diciembre, 2010
*/

#include "Manager.h"

#include "buffer.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "factoriaredenet.h"
#include "factoriared.h"
#include <cassert>

namespace Net {

	CManager* CManager::_instance = 0;

	//--------------------------------------------------------

	CManager::CManager(): _factoriaRed(0), _servidorRed(0), _clienteRed(0),
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
		_factoriaRed = new Net::CFactoriaRedEnet();

		return true;

	} // open

	//--------------------------------------------------------

	void CManager::close() 
	{
		deactivateNetwork();
		if(_factoriaRed)
			delete _factoriaRed;

	} // close

	//---------------------------------------------------------

	void CManager::send(void* data, size_t longdata) 
	{
		if(!_connections.empty())
		{
			// TODO Ahora hay más de una conexión, debemos mandar el mensaje por
			// todas si somos servidor.
			if(_servidorRed)
				_servidorRed->sendAll(data,longdata,0,1);
			if(_clienteRed)
				_clienteRed->sendData(getConnection(Net::ID::SERVER),data,longdata,0,1); // TODO Se guardan en cliente conexiones con el resto de clientes?
		}

	} // send

	//---------------------------------------------------------

	void CManager::tick(unsigned int msecs) 
	{
		_paquetes.clear();
		Net::CManager::getSingletonPtr()->getPackets(_paquetes);

		for(std::vector<Net::CPaquete*>::iterator iterp = _paquetes.begin();iterp != _paquetes.end();++iterp)
		{
			Net::CPaquete* paquete = *iterp;
			// El mensaje debe ser de tipo CONEXION
			switch (paquete->getTipo())
			{
				case Net::CONEXION:
					connect(paquete->getConexion());
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->connexionPacketReceived(paquete);
					break;
				case Net::DATOS:
					if(!internalData(paquete)) // Analiza si trae contenido -> TODO: ver funcion
						for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
							(*iter)->dataPacketReceived(paquete);
					break;
				case Net::DESCONEXION:
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->disconnexionPacketReceived(paquete);
					disconnect(paquete->getConexion());
					break;
			}
			delete paquete;
		}

	} // tick

	//---------------------------------------------------------


	void CManager::activateAsServer(int port, int clients, unsigned int maxinbw, unsigned int maxoutbw)
	{
		//Creamos el servidor de red
		_servidorRed = _factoriaRed->buildServidor();

		_servidorRed->init(port,clients, maxinbw, maxoutbw);

		_id = Net::ID::SERVER;

	} // activateAsServer

	//---------------------------------------------------------

	void CManager::activateAsClient(unsigned int maxConnections, unsigned int maxinbw, unsigned int maxoutbw)
	{
		//Creamos el servidor de red
		_clienteRed = _factoriaRed->buildCliente();

		_clienteRed->init(maxConnections, maxinbw, maxoutbw);

	} // activateAsClient

	//---------------------------------------------------------

	void CManager::connectTo(char* address, int port, int channels, unsigned int timeout)
	{
		assert(_clienteRed && "Cliente Red es null"); // Solo se puede ejecutar el connectTo si somos cliente
		assert(_connections.empty() && "Ya hay una conexion"); // Capamos al cliente a 1 conexión max: la de con el server

		CConexion* connection = _clienteRed->connect(address, port, channels,timeout); // CONNECT
		
		// Almacenamos esa conexión y le otorgamos un ID de red
		connection->setId(Net::ID::SERVER); // Un cliente sólo se conecta al SERVER
		addConnection(Net::ID::SERVER, connection); // Guardamos en la tabla
	} // connectTo

	//---------------------------------------------------------

	bool CManager::internalData(Net::CPaquete* packet)
	{
		Net::CBuffer data;
		data.write(packet->getData(),packet->getDataLength());
		data.reset();
		// Ignoramos el tipo de mensaje de red. Ya lo hemos procesado
		Net::NetMessageType msg;
		data.read(&msg,sizeof(msg));
		switch (msg)
		{
		case Net::ASSIGNED_ID: 
			// Escribimos el id de red que corresponde a este cliente
			data.read(&_id,sizeof(_id));
			return true;
		default:
			return false;
		}

	} // internalData

	//---------------------------------------------------------

	void CManager::connect(CConexion* connection)
	{
		// Una nueva conexión de un cliente al sevidor

		// Almacenamos esa conexión y le otorgamos un ID de red
		connection->setId(_nextId);
		addConnection(_nextId,connection);

		// Avisamos al cliente de cual es su nuevo ID
		CBuffer buf;
		// Escribimos el tipo de mensaje de red a enviar
		NetMessageType type = Net::ASSIGNED_ID;
		buf.write(&type,sizeof(type));
		// Escribimos el id del cliente
		buf.write(&_nextId,sizeof(_nextId));
		_servidorRed->sendData(connection, buf.getbuffer(),buf.getSize(),0,1);

		// Preparamos para la siguiente conexión
		_nextId = ID::NextID(_nextId);

	} // connect

	//---------------------------------------------------------

	void CManager::disconnect(CConexion* connection)
	{
		if(_servidorRed)
		{
			_servidorRed->disconnect(connection);
			removeConnection(connection->getId());
		}
		else if(_clienteRed)
		{
			_clienteRed->disconnect(getConnection(Net::ID::SERVER));
			removeConnection(Net::ID::SERVER);
		}

	} // disconnect
		
	//---------------------------------------------------------

	bool CManager::addConnection(NetID id, CConexion* connection) 
	{
		if(_connections.count(id))
			return false;
		TConnectionPair elem(id,connection);
		_connections.insert(elem); // Insertamos par id - conexion
		return true;

	} // addConection
		
	//---------------------------------------------------------

	bool CManager::removeConnection(NetID id) 
	{
		if(_connections.count(id))
		{
			CConexion* connection = getConnection(id);
			_connections.erase(id);
			delete connection;
			return true;
		}
		return false;

	} // removeConection

	//---------------------------------------------------------

	void CManager::deactivateNetwork()
	{
		if(_servidorRed)
		{
			_servidorRed->deInit(); // TODO analizar que haga disconnect de todo; antes if(_conexion) disconnect (OJO estaba comentado)
			delete _servidorRed;
			_servidorRed = 0;
		}
		if(_clienteRed)
		{
			_clienteRed->deInit();
			delete _clienteRed;
			_clienteRed = 0;
		}
		if(!_connections.empty())
		{
			for(TConnectionTable::const_iterator it = _connections.begin(); it != _connections.end(); it++)
				delete (*it).second;
			_connections.clear(); // Quien hace el disconnect
		}
	} // deactivateNetwork

	//---------------------------------------------------------

	void CManager::getPackets(std::vector<Net::CPaquete*>& _paquetes)
	{
		if(_servidorRed)
			_servidorRed->service(_paquetes);
		if(_clienteRed)
			_clienteRed->service(_paquetes);

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
