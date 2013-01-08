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

	CManager::CManager(): _factoriaRed(0), _servidorRed(0), _clienteRed(0), _conexion(0)
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

	// TODO Todo se está enviando sobre la misma conexión y canal
	void CManager::send(void* data, size_t longdata) 
	{
		if(_conexion)
		{
			if(_servidorRed)
				_servidorRed->sendData(_conexion, data,longdata,0,1);
			if(_clienteRed)
				_clienteRed->sendData(_conexion,data,longdata,0,1);
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
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->connexionPacketReceived(paquete);
					_conexion = paquete->getConexion();
					break;
				case Net::DATOS:
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->dataPacketReceived(paquete);
					break;
				case Net::DESCONEXION:
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->disconnexionPacketReceived(paquete);
					disconnect();
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
		assert(_clienteRed && "Cliente Red es null");
		assert(!_conexion && "Ya hay una conexion");
		_conexion = _clienteRed->connect(address, port, channels,timeout);

	} // connectTo

	//---------------------------------------------------------

	void CManager::disconnect()
	{
		if(_conexion)
		{
			if(_servidorRed)
				_servidorRed->disconnect(_conexion); // disconnectAll()?
			else if(_clienteRed)
				_clienteRed->disconnect(_conexion);
			delete _conexion;
			_conexion = 0;
		}

	} // disconnect

	//---------------------------------------------------------

	void CManager::deactivateNetwork()
	{
		if(_servidorRed)
		{
			_servidorRed->deInit();
			delete _servidorRed;
			_servidorRed = 0;
		}
		if(_clienteRed)
		{
			_clienteRed->deInit();
			delete _clienteRed;
			_clienteRed = 0;
		}
		if(_conexion)
		{
			delete _conexion;
			_conexion = 0;
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

} // namespace Net
