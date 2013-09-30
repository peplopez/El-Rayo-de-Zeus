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

//PT
#include <cegui\CEGUIWindowManager.h>
#include <cegui\elements\CEGUIProgressBar.h>
#include <Graphics\Server.h>

#include <cassert>

namespace Logic {


	/**********************
		SINGLETON
	******************/

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
		Logic::CEntityFactory::getSingletonPtr()->disableDefferedEntitiesPhysics();
		Logic::CEntityFactory::getSingletonPtr()->enableDefferedEntitiesPhysics();

		TMaps::const_iterator it = _maps.cbegin();
		TMaps::const_iterator end = _maps.cend();
		
		for (; it != end; ++it)
			it->second->tick(msecs);
	} // tick


	/************
		WORLD
	************/
	
	bool CServer::loadWorld(TMultiSettings& allSettings)
	{		
		if(_worldIsLoaded)
			unLoadWorld();
		
		const int nPlayers = allSettings.size();
			if ( !Logic::CGameStatus::Init( nPlayers ) )
				return false;
		
		if (!CPlayerSettings::isLowQMode())
		Graphics::CServer::getSingletonPtr()->
			setClimatologyToLoad(Logic::Player::CLIMATOLOGY_STRINGS[CPlayerSettings::getClimatology()]);
				
		//PT creo la barra de progreso
		CEGUI::ProgressBar *hbar = static_cast<CEGUI::ProgressBar*> (CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/Progreso"));
		float progress = hbar->getProgress();		
		std::string texto = "";
	
		float step = (0.9f - progress) /  nPlayers;

		for (int i = 0; i < nPlayers; ++i)
		{
			_worldIsLoaded = loadMap( allSettings[i], i+1 );
			_mapNames.push_back( allSettings[i].getMapName() );

			//PT
			progress+= step;
			hbar->setProgress(progress);
			std::string result;
			std::stringstream ss;
			ss.clear();
			ss << "Loading maps: " << (i + 1) << " / " <<  nPlayers;
			result = ss.str();
			CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/TextoProgreso")->setText(result);
			Graphics::CServer::getSingletonPtr()->tick(0);		
		}	

  		allSettings.clear();
		return _worldIsLoaded;
	}
	
	void CServer::unLoadWorld()
	{
		if(_worldIsLoaded) {


			TMaps::const_iterator it = _maps.cbegin();
			TMaps::const_iterator end = _maps.cend();		
				while (it != end)
					unLoadMap(it++->first);

			_mapNames.clear();
			Graphics::CServer::getSingletonPtr()->setClimatologyToLoad("");
			Logic::CGameStatus::Release(); // FRS 1304 Borramos GameStatus
			CPlayerSettings::resetUsedColors(); // FRS reset del cómputo de colores en uso
			_player = 0;
			_worldIsLoaded = false;
		}
	}
	

	
	/*************
		MAPS
	************/

	//--------------  ALL MAPS ------------------------------------------

	bool CServer::activateAllMaps()
	{
		TMaps::const_iterator it = _maps.cbegin();
		TMaps::const_iterator end = _maps.cend();

		bool activated = false;

		for (; it != end; ++it)
			activated = it->second->activate();

		_gameNetMsgManager->activate();		
		_player->getMap()->activatePlayerCam();

		return activated;
	}

	//------------------------------------------------------------------

	void CServer::deactivateAllMaps()
	{
		TMaps::const_iterator it = _maps.cbegin();
		TMaps::const_iterator end = _maps.cend();

		for (; it != end; ++it)
			it->second->deactivate();

		_gameNetMsgManager->deactivate();
	}


	//--------------  SINGLE MAP ------------------------------------------

	bool CServer::loadMap(CPlayerSettings& settings, int mapNumber)
	{		
		// solo admitimos un mapa cargado con el mismo nombre 
		// si iniciamos un nuevo nivel  se borra el mapa anterior.
		unLoadMap( settings.getMapName() );
		if(_maps[ settings.getMapName() ] = CMap::createMap(settings, mapNumber) )
			return true;

		return false;
	} // loadMap
	
	//--------------------------------------------------------------

	void CServer::unLoadMap(std::string mapName)
	{
		TMaps::const_iterator it = _maps.find(mapName);
		
		if(it != _maps.cend())
		{
			it->second->deactivate();
			delete _maps[mapName];
			_maps[mapName] = 0;
			_maps.erase(it);

			const int N = _mapNames.size();
			for(int i = 0; i < N; ++i)
				if(_mapNames[i] == mapName) {
					for(int j = i; j < N - 1; ++j)
						_mapNames[j] = _mapNames[j + 1];
					_mapNames.pop_back();
					break;
				}
		}

	} // unLoadLevel

	//--------------  SINGLE MAP ------------------------------------------

	bool CServer::activateMap(const std::string &mapName) 
	{
		// Se activa la escucha del oyente de los mensajes de red para el estado de juego.
		//_gameNetMsgManager->activate();
		TMaps::const_iterator it = _maps.find(mapName);		
		if(it != _maps.cend())
			return it->second->activate();
		else 
			return false;
	} // activateMap


	//--------------  SINGLE MAP ------------------------------------------

	void CServer::deactivateMap(const std::string &mapName) 
	{
		TMaps::const_iterator it = _maps.find(mapName);		
		if(it != _maps.cend())
			it->second->deactivate();
		//_gameNetMsgManager->deactivate(); // Se desactiva la escucha del oyente de los mensajes de red para el estado de juego.
	} // deactivateMap

	
	//--------------  SINGLE MAP ------------------------------------------

	CMap* CServer::getMap(const std::string mapName)
	{
		TMaps::const_iterator it = _maps.find(mapName);

		if (it != _maps.cend())
			return it->second;
		
		return NULL;
	}
	

	/********************
		MOVE ENTITIES
	********************/

	void CServer::deferredMoveEntity(CEntity *entity, int targetMap)
	{
		assert(entity);
		_entitiesToMove[targetMap].push_back(entity);
	}

	//---------------------------------------------------------

	void CServer::moveDefferedEntities()
	{
		TEntityIDMap::iterator it = _entitiesToMove.begin();
		TEntityIDMap::const_iterator end = _entitiesToMove.cend();

		for (; it != end; ++it)
		{
			TEntityList::const_iterator entity = it->second.cbegin();
			TEntityList::const_iterator entityEnd = it->second.cend();

			for (; entity != entityEnd; ++entity)
			{
				(*entity)->detachFromMap();
				(*entity)->attachToMap(_maps[ _mapNames[it->first - 1] ]);
				(*entity)->activate();	

				if ((*entity)->isLocalPlayer())
					(*entity)->getMap()->activatePlayerCam(); 
			}
			it->second.clear();
		}

	}

	/**************
		CAMS
	*************/

	void CServer::activateBaseCam(int targetMap)
	{
		_maps[ _mapNames[targetMap - 1] ]->activateBaseCam();
	}

	//---------------------------------------------------------

	void CServer::activatePlayerCam()
	{
		_player->getMap()->activatePlayerCam();
	}

	

	

	

	/******************
		COMPOSITORS
	******************/

	void CServer::compositorEnable(const std::string &name)
	{
		Graphics::CServer::getSingletonPtr()->compositorEnable(name);
	}

	//---------------------------------------------------------

	void CServer::compositorDisable(const std::string &name)
	{
		Graphics::CServer::getSingletonPtr()->compositorDisable(name);
	}



	/**************
		RINGS
	**************/

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
