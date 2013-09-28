/**
@file Map.cpp

Contiene la implementación de la clase CMap, Un mapa lógico.

@see Logic::Map

@author David Llansó
@date Agosto, 2010
*/

#include "Map.h"

#include "EntityFactory.h"

#include <Logic/Entity/Entity.h>
#include <Logic/Server.h>
#include <Graphics/Server.h>
#include <Graphics/Scene.h>
#include <Map/MapParser.h>
#include <Physics/Server.h>
#include <Physics/Scene.h>
#include <cassert>


// HACK. Debería leerse de algún fichero de configuración
#define FILE_PATH "./media/maps/"

#include "Application/BaseApplication.h"
#include "../Entity/LogicalPosition.h"


#include "..\..\Application\Clock.h"


namespace Logic {
		

	/*****************
		CONSTRUCTORES
	*****************/
	

	// ƒ®§ Creación de un mapa con nombre name (normalmente el propio filename). => Creación de escenas física y gráfica
	CMap::CMap(const std::string &name, int mapNumber) : _name(name), _mapNumber(mapNumber), _isActive(false), _nAllies(0)
	{
		_graphicScene = Graphics::CServer::getSingletonPtr()->createScene(name);
		_physicsScene  = Physics::CServer::getSingletonPtr()->createScene(name); 
		// ƒ®§ aunque se creen las escenas, la escena activa debe ser la dummy hasta la activación del map

	} // CMap

	//--------------------------------------------------------

	// TODO FRS Revisar estos if (necesarios?)
	CMap::~CMap()
	{
		deactivate();
		destroyAllEntities();
		Graphics::CServer::getSingletonPtr()->removeScene(_graphicScene);
		Physics::CServer::getSingletonPtr()->removeScene(_physicsScene);
	} // ~CMap

	

	/************
		FLOW
	************/

	bool CMap::activate()
	{			
		if(_isActive)
			return true;
		

		// Activamos todas las entidades registradas en el mapa.
		_isActive = true;
		TEntityList::const_iterator it = _entityList.cbegin();
		TEntityList::const_iterator end = _entityList.cend();
			for(; it != end; ++it)
				_isActive = (*it)->activate() && _isActive;

		// Activamos la escena gráfica y física correspondiente a este mapa (_inits)
		Graphics::CServer::getSingletonPtr()->activate(_graphicScene);
		Physics::CServer::getSingletonPtr()->activate(_physicsScene);

		bullet=0;
		return _isActive;
	} // activate

	//--------------------------------------------------------

	void CMap::deactivate()
	{
		if(!_isActive)
			return;

		// Desactivamos todas las entidades activas registradas en el mapa.
		TEntityList::const_iterator it  = _entityList.cbegin();
		TEntityList::const_iterator end = _entityList.cend();
			for(; it != end; it++)
				if((*it)->isActivated())
					(*it)->deactivate();

		//ponemos la dummyCamera
		Graphics::CServer::getSingletonPtr()->activatePlayerCam(0); 

		//deinicilizamos las escenas
		Graphics::CServer::getSingletonPtr()->deactivate(_graphicScene);
		Physics::CServer::getSingletonPtr()->deactivate(_physicsScene);

		_isActive = false;
	} // deactivate


	void CMap::tick(unsigned int msecs) 
	{
		TEntityList::const_iterator it = _entityList.cbegin();
		TEntityList::const_iterator end = _entityList.cend();
			for(; it != end; ++it )
				(*it)->tick(msecs);
	} // tick



	
	/**********
		CAMS
	**********/

	void CMap::activatePlayerCam()
	{
		Graphics::CServer::getSingletonPtr()->activatePlayerCam( this->_graphicScene );
	}

	//---------------------------------------------------------

	void CMap::activateBaseCam()
	{
		Graphics::CServer::getSingletonPtr()->activateBaseCam( this->_graphicScene );
	}
	


	/********************
		MAP CREATORS
	*******************/

	CMap* CMap::createMap(CPlayerSettings& settings, int mapNumber)
	{				
		// Creación de las escenas física y gráfica
		CMap *map = new CMap( settings.getMapName(), mapNumber ); 
			map->setProperties( settings.getMapProperties() );

		// TODO FRS Habría que tener cuidado y considerar si realmente queremos que entityName = nickName...
		// Podría darse el caso de necesitar acceder a las entidades por nombres explicitos (PlayerRed,
		// PlayerGreen, etc...) y que el nick fuera un atributo aparte...

		CEntityFactory::getSingletonPtr()->fillMapUsingPattern(map); 
		// FRS desencadena también la creación del Player, ya que map_pattern.txt define un player genérico
		// configurado a través de Keywords.
		
		return map;
	} // createMap

	
	
	//--------------------------------------------------------


	/***********************
		ENTITY CREATORS
	**********************/


	// FRS DEPRECATED

	// [ƒ®§] Creamos un nuevo jugador. Deberíamos tener la info del player
	// almacenada en _playerInfo así que solo habría que modificarle el
	// "name". Luego se crea la entidad del jugador con la factoría de 
	// entidades y se le dice si es o no el jugador local (con setisLocalPlayer())

	//PT. Se le pasan los argumentos de nickname, modelo, y color del Player.
	void CMap::createPlayer(bool isLocalPlayer, const std::string& nickname, const std::string& color, const std::string& model)
	{
		Map::CEntity playerInfo( nickname );
			playerInfo.setType("Player");	
			playerInfo.setAttribute("isLocalPlayer", isLocalPlayer ? "true" : "false");	
			playerInfo.setAttribute("modelColor1",	color );	
			playerInfo.setAttribute("model",		model );				
		CEntity* newPlayer = CEntityFactory::getSingletonPtr()->createEntity(playerInfo, this);
	} // createPlayer


	//--------------------------------------------------------

	//PT
	void CMap::createAlly(std::string entityName, const std::string& type, const unsigned short base, const unsigned short ring, const unsigned short degrees, const unsigned short sense)
	{

		// [PT] Creamos un nuevo aliado. Deberíamos tener la info del aliado
		// almacenada en aliedInfo así que solo habría que modificarle el
		// "name" (FRS a eso se le llama Archetype). Luego se crea la entidad del aliado con la factoría de 
		// entidades

		std::ostringstream eName, eBase, eRing, eDegrees, eSense;
		eName << entityName << _nAllies; //alied es un contador

		eBase << base;
		eRing << ring;
		eDegrees << degrees;
		eSense << sense;

		Map::CEntity aliedInfo(eName.str());

		aliedInfo.setType(type);


		//Atributos
		aliedInfo.setAttribute("base", eBase.str());
		aliedInfo.setAttribute("ring", eRing.str());
		aliedInfo.setAttribute("sense", eSense.str());
		aliedInfo.setAttribute("degrees", eDegrees.str());

		CEntity* newAlied = CEntityFactory::getSingletonPtr()->createEntity(aliedInfo, this);

		//activate the new entity
		newAlied->activate();

		++_nAllies;

		//newAlied->setPosition(newAlied->getPosition() + (rand()%50-25) * Vector3(1, 0, 1) );

	}
	void CMap::createProjectile(const std::string entityName, const CLogicalPosition pos,const CEntity* father)
	{

		// [PT] Creamos un proyectil, flecha. Lo hago tal como crea los aliados Pablo

		std::ostringstream eName, eBase, eRing, eDegrees, eSense;
		eName << entityName; //bullet es un contador

		eBase << pos.getBase();
		eRing << (unsigned short) pos.getRing();
		eDegrees << (float)pos.getDegree();
		eSense << (unsigned short) pos.getSense();

		Map::CEntity bulletInfo(eName.str());

		bulletInfo.setType(entityName);


		//Atributos
		bulletInfo.setAttribute("base", eBase.str());
		bulletInfo.setAttribute("ring", eRing.str());
		bulletInfo.setAttribute("sense", eSense.str());
		bulletInfo.setAttribute("degrees", eDegrees.str());

		CEntity* newBullet = CEntityFactory::getSingletonPtr()->createMergedEntity(&bulletInfo, this, father);

		//activate the new entity
		//newBullet->getLogicalPosition()->setSense(eSense);
		newBullet->activate();

		bullet++;

		//newAlied->setPosition(newAlied->getPosition() + (rand()%50-25) * Vector3(1, 0, 1) );

	}

	



	/**********************
		ENTITY MANAGEMENT
	**********************/

	void CMap::destroyAllEntities()
	{
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
		
		// Eliminamos todas las entidades. La factoría se encarga de
		// desactivarlas y sacarlas previamente del mapa.
		// FRS No podemos usar el removeEntity ya que modificaría la lista mientras la recorremos
		TEntityList::const_iterator it	= _entityList.cbegin();
		TEntityList::const_iterator end = _entityList.cend();
	
			while(it != end)			
				entityFactory->deleteEntity( *it++ ); 
			// FRS Incrementamos el iterador antes del deleteEntity
			// ya que el método modifica el _entityList

				_entityList.clear();
		_entityMap.clear();
		Application::CBaseApplication::getSingletonPtr()->getClock()->removeAllTimeObserver();
		
	} // destroyAllEntities

	//--------------------------------------------------------	

	void CMap::addEntity(CEntity *entity)
	{
		if( !_entityMap.count(entity->getEntityID() ) )
		{			
			_entityMap[entity->getEntityID()] = entity;
			_entityList.push_back(entity); 
			entity->_map = this;
		}

	} // addEntity

	//--------------------------------------------------------

	void CMap::removeEntity(CEntity *entity)
	{
		if(_entityMap.count( entity->getEntityID() ) )
		{
			if(entity->isActivated())
				entity->deactivate();

			entity->_map = 0;
			_entityMap.erase(entity->getEntityID());
			_entityList.remove(entity);
		}

	} // removeEntity

	//--------------------------------------------------------

	CEntity* CMap::getEntityByName(const std::string &name, CEntity *start)
	{
		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		TEntityMap::const_iterator end = _entityMap.cend();
		TEntityMap::const_iterator it;
			if (!start)
				it = _entityMap.begin();
			else {
				it = _entityMap.find(start->getEntityID());				
					if(it == end)
						return 0;// si la entidad no existe devolvemos NULL.
					else	
						++it;
			}		
			
		// BUSQUEDA => TODO FRS Lo mismo esto con el list es más eficiente
		for(; it != end; ++it){			
			if ( (*it).second->getName() == name)// si hay coincidencia de nombres devolvemos la entidad.
				return (*it).second;
		}		
		return 0;// si no se encontró la entidad devolvemos NULL.
	} // getEntityByName

	//--------------------------------------------------------

	CEntity* CMap::getEntityByType(const std::string &type, CEntity *start)
	{
		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		TEntityMap::const_iterator end = _entityMap.cend();
		TEntityMap::const_iterator it;
			if (!start)
				it = _entityMap.begin();
			else {
				it = _entityMap.find(start->getEntityID());		
					if(it == end)
						return 0;// si la entidad no existe devolvemos NULL.
					else
						++it;
			}		

		for(; it != end; ++it)	{// si hay coincidencia de nombres devolvemos la entidad			
			if ( (*it).second->getType() == type )
				return (*it).second;
		}
	
		return 0;	// si no se encontró la entidad devolvemos NULL.

	} // getEntityByType

	//--------------------------------------------------------

	void CMap::switchDebugDraw()
	{
		_physicsScene->switchDebugDraw();
	}
	

} // namespace Logic
