/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la lógica del juego.

@see Logic::CServer

@author David Llansó
@date Agosto, 2010
*/

#include "Server.h"
#include "Logic/Maps/Map.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
//#include "Logic/Entity/Entity.h"

#include "Map/MapParser.h"
#include "Logic/Entity/RingStruct.h"
#include "Logic/GameNetMsgManager.h"
#include "Logic/GameStatus.h"

#include "Logic\Entity\RingStruct.h"

#include <cassert>

namespace Logic {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() :  _gameNetMsgManager(0), _worldIsLoaded(false)
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "Logic::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		// Inicializamos el parser de mapas.
		if (!Map::CMapParser::Init())
			return false;

		// Inicializamos la factoría de entidades.
		if (!Logic::CEntityFactory::Init())
			return false;

		// Inicializamos el gestor de los mensajes de red durante el estado de juego
		if (!Logic::CGameNetMsgManager::Init())
			return false;

		_gameNetMsgManager = Logic::CGameNetMsgManager::getSingletonPtr();

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		Logic::CGameNetMsgManager::Release();

		Logic::CEntityFactory::Release();
		
		Map::CMapParser::Release();
	} // close

	//--------------------------------------------------------

	
	void CServer::tick(unsigned int msecs) 
	{
		// Eliminamos las entidades que se han marcado para ser eliminadas.
		moveDefferedEntities();
		Logic::CEntityFactory::getSingletonPtr()->deleteDefferedEntities();

		TMaps::const_iterator it = _maps.begin();
		TMaps::const_iterator end = _maps.end();
		
		for (; it != end; ++it)
			it->second->tick(msecs);
	} // tick

	//---------------------------------------------------------

	// ƒ®§ Carga el map desde fichero ==> ejecuta el entity.spawn()
	bool CServer::loadMap(const std::string &filename)
	{
		// solo admitimos un mapa cargado, si iniciamos un nuevo nivel 
		// se borra el mapa anterior.
		unLoadMap(filename);

		if(_maps[filename] = CMap::createMapFromFile(filename))
			return true;
	} // loadLevel

	//--------------------------------------------------------

	void CServer::unLoadMap(const std::string &filename)
	{
		TMaps::const_iterator it = _maps.find(filename);
		
		if(it != _maps.end())
		{
			it->second->deactivate();
			delete _maps[filename];
			_maps[filename] = 0;
			_maps.erase(it);
		}

	} // unLoadLevel

	//---------------------------------------------------------

	bool CServer::loadWorld(const TMapNameList mapList)
	{		
		if(_worldIsLoaded)
			unLoadWorld();

		// Inicializamos el gestor de los mensajes de red durante el estado de juego
		if (!Logic::CGameStatus::Init(mapList.size()))
			return false;

		TMapNameList::const_iterator it = mapList.begin();
		TMapNameList::const_iterator end = mapList.end();
		
	
		for (; it != end; ++it)
		{
			_worldIsLoaded = loadMap(*it);
			_mapNames.push_back(*it);
		}	

		return _worldIsLoaded;
	}

	//---------------------------------------------------------

	void CServer::unLoadWorld()
	{
		if(_worldIsLoaded) {

			TMaps::const_iterator it = _maps.begin();
			TMaps::const_iterator end = _maps.end();
		
			while (it != end)
				unLoadMap(it++->first);

			Logic::CGameStatus::Release(); // FRS 1304 Borramos GameStatus
			_player = 0;
			_worldIsLoaded = false;
		}
	}

	//---------------------------------------------------------

	bool CServer::activateMap(const std::string &filename) 
	{
		// Se activa la escucha del oyente de los mensajes de red para el estado de juego.
		//_gameNetMsgManager->activate();
		return _maps[filename]->activate();

	} // activateMap

	//---------------------------------------------------------

	void CServer::deactivateMap(const std::string &filename) 
	{
		TMaps::const_iterator it = _maps.find(filename);
		
		if(it != _maps.end())
			it->second->deactivate();
		//_gameNetMsgManager->deactivate(); // Se desactiva la escucha del oyente de los mensajes de red para el estado de juego.
	} // deactivateMap

	//---------------------------------------------------------

	bool CServer::activateAllMaps()
	{
		TMaps::const_iterator it = _maps.begin();
		TMaps::const_iterator end = _maps.end();

		bool activated = false;

		for (; it != end; ++it)
			activated = it->second->activate();

		_gameNetMsgManager->activate();

		
		_player->getMap()->setVisible();

		return activated;

	}

	//---------------------------------------------------------

	void CServer::deactivateAllMaps()
	{
		TMaps::const_iterator it = _maps.begin();
		TMaps::const_iterator end = _maps.end();

		for (; it != end; ++it)
			it->second->deactivate();

		_gameNetMsgManager->deactivate();
	}

	//---------------------------------------------------------
	
	CMap* CServer::getMap(const std::string mapName)
	{
		TMaps::const_iterator it = _maps.find(mapName);

		if (it != _maps.end())
			return it->second;
		
		return NULL;
	}

	//---------------------------------------------------------

	void CServer::deferredMoveEntity(CEntity *entity, int targetMap)
	{
		assert(entity);
		_entitiesToMove[targetMap].push_back(entity);

	}



	//---------------------------------------------------------

	void CServer::moveDefferedEntities()
	{
		TMapEntityLists::iterator it = _entitiesToMove.begin();
		TMapEntityLists::const_iterator end = _entitiesToMove.end();

		for (; it != end; ++it)
		{
			TEntityList::const_iterator entity = it->second.begin();
			TEntityList::const_iterator entityEnd = it->second.end();

			for (; entity != entityEnd; ++entity)
			{
				(*entity)->detachFromMap();
				(*entity)->attachToMap(_maps[ _mapNames[it->first - 1] ]);
				(*entity)->activate();

				if ((*entity)->isPlayer())
				{
					_player->getMap()->setVisible();
				}

			}

			it->second.clear();
		}

	}

	//---------------------------------------------------------

	void CServer::activateBaseCam(int targetMap)
	{
		_maps[ _mapNames[targetMap - 1] ]->activateBaseCam();
	}

	//---------------------------------------------------------

	void CServer::activatePlayerCam()
	{
		_player->getMap()->setVisible();
	}

	//---------------------------------------------------------

	Vector3 TRingPositions::_up;
	Vector3 TRingPositions::_center;
	Vector3 TRingPositions::_down;

	bool CServer::setRingPositions()
	{
		//inicializamos la estructura de posiciones de los anillos
		//el primer anillo de la primera base, empezando por abajo, será la base de la pila de anillos
		
		TRingPositions::_down = Logic::startingRingPosition;
		TRingPositions::_center  = TRingPositions::_down + Logic::separationBetweenRings;
		TRingPositions::_up  = TRingPositions::_center + Logic::separationBetweenRings;

		return true;
	}

	//---------------------------------------------------------
	Vector3 CServer::getRingPosition(LogicalPosition::Ring ring)

	{
		Vector3 retorno= Vector3::ZERO;
			switch (ring)
			{
				case Logic::LogicalPosition::LOWER_RING:				
					return TRingPositions::_down;
				
				case Logic::LogicalPosition::CENTRAL_RING:				
					return TRingPositions::_center;
				
				case Logic::LogicalPosition::UPPER_RING:				
					return TRingPositions::_up;
				
				default:					
					return TRingPositions::_center;
					//situación anómala, se lanzaría una excepción o trazas por consola. Se le asigna el anillo central para que 
					//pese a todo no pete.
											
			}

	}

	//---------------------------------------------------------

	float CServer::getRingRadio(Logic::LogicalPosition::Ring ring)
	{
			switch (ring)
			{
				case Logic::LogicalPosition::LOWER_RING:				
					return Logic::RADIO_MENOR;
				
				case Logic::LogicalPosition::CENTRAL_RING:				
					return Logic::RADIO_MAYOR;
				
				case Logic::LogicalPosition::UPPER_RING:				
					return Logic::RADIO_MENOR;
				
				default:					
					return Logic::RADIO_MAYOR;
					//situación anómala, se lanzaría una excepción o trazas por consola. Se le asigna el anillo central para que 
					//pese a todo no pete.
												
			}

	}
	

} // namespace Logic
