/**
@file Map.cpp

Contiene la implementación de la clase CMap, Un mapa lógico.

@see Logic::Map

@author David Llansó
@date Agosto, 2010
*/

#include "Map.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "EntityFactory.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include "Physics/Server.h"
#include "Physics/Scene.h"

#include <cassert>


// HACK. Debería leerse de algún fichero de configuración
#define MAP_FILE_PATH "./media/maps/"

namespace Logic {
		

	// ƒ®§ Creación de un mapa con nombre name (normalmente el propio filename). => Creación de escenas física y gráfica
	CMap::CMap(const std::string &name) : _name(name), _isActive(false)
	{
		_graphicScene = Graphics::CServer::getSingletonPtr()->createScene(name);
		_physicScene  = Physics::CServer::getSingletonPtr()->createScene(name); 
		// ƒ®§ aunque se creen las escenas, la escena activa debe ser la dummy hasta la activación del map

	} // CMap

	//--------------------------------------------------------

	// TODO FRS Revisar estos if (necesarios?)
	CMap::~CMap()
	{
		deactivate();
		destroyAllEntities();
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::getSingletonPtr()->removeScene(_graphicScene);
		if(Physics::CServer::getSingletonPtr() )
			Physics::CServer::getSingletonPtr()->removeScene(_physicScene);

	} // ~CMap

	//--------------------------------------------------------

	bool CMap::activate()
	{			
		if(_isActive)
			return true;

		Graphics::CServer::getSingletonPtr()->setActiveScene(_graphicScene);	
		Physics::CServer::getSingletonPtr()->setActiveScene(_physicScene);	

		// Activamos todas las entidades registradas en el mapa.
		_isActive = true;
		TEntityList::const_iterator it = _entityList.begin();
		TEntityList::const_iterator end = _entityList.end();
			for(; it != end; ++it)
				_isActive = (*it)->activate() && _isActive;

		return _isActive;
	} // getEntity

	//--------------------------------------------------------

	void CMap::deactivate()
	{
		if(!_isActive)
			return;

		// Desactivamos todas las entidades activas registradas en el mapa.
		TEntityList::const_iterator it  = _entityList.begin();
		TEntityList::const_iterator end = _entityList.end();
			for(; it != end; it++)
				if((*it)->isActivated())
					(*it)->deactivate();

		Graphics::CServer::getSingletonPtr()->setActiveScene(0);
		Physics::CServer::getSingletonPtr()->setActiveScene(0);

		_isActive = false;
	} // getEntity

	//---------------------------------------------------------

	void CMap::tick(unsigned int msecs) 
	{
		TEntityList::const_iterator it = _entityList.begin();
		TEntityList::const_iterator end = _entityList.end();
			for(; it != end; ++it )
				(*it)->tick(msecs);
	} // tick

	//--------------------------------------------------------

	CMap* CMap::createMapFromFile(const std::string &filename)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(MAP_FILE_PATH);
			completePath = completePath + filename + ".txt";
				
				if(!Map::CMapParser::getSingletonPtr()->parseFile(completePath)){ // Parseamos el fichero
					assert(!"No se ha podido parsear el mapa.");
					return false;
				}

		// Si se ha realizado con éxito el parseo creamos el mapa.
		CMap *map = new CMap(filename); // Desencadena la creación de las escenas física y gráfica

		// Extraemos las entidades del parseo.
		Map::CMapParser::TEntityList entityList = 
			Map::CMapParser::getSingletonPtr()->getEntityList();

		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
		
		
		// Creamos todas las entidades lógicas.
		Map::CMapParser::TEntityList::const_iterator it = entityList.begin();
		Map::CMapParser::TEntityList::const_iterator end = entityList.end();
			for(; it != end; it++)		{			
				CEntity *entity = entityFactory->createMergedEntity((*it),map); // La propia factoría se encarga de añadir la entidad al mapa.
				assert(entity && "No se pudo crear una entidad del mapa");
			}


		return map;

	} // createMapFromFile

	//--------------------------------------------------------

	void CMap::createPlayer(std::string entityName, std::string model, bool isLocalPlayer)
	{
		// [ƒ®§] Creamos un nuevo jugador. Deberíamos tener la info del player
		// almacenada en _playerInfo así que solo habría que modificarle el
		// "name". Luego se crea la entidad del jugador con la factoría de 
		// entidades y se le dice si es o no el jugador local (con setIsPlayer())
		// Para que no salgan todos los jugadores unos encima de otros podemos
		// cambiar la posición de éstos.

		Map::CEntity playerInfo(entityName);
			playerInfo.setType("Player");			
		if(model.length() > 0)
			playerInfo.setAttribute("model", model);
			playerInfo.setAttribute("isPlayer", isLocalPlayer? "true" : "false");
			
		CEntity* newPlayer = CEntityFactory::getSingletonPtr()->createMergedEntity(&playerInfo, this);
			//newPlayer->setPosition( newPlayer->getPosition() + (rand()%50-25) * Vector3(1, 0, 1) ); // TODO calibrar esta pos
	
	} // createPlayer

	



	/**********************
		ENTITY MANAGEMENT
	**********************/

	void CMap::destroyAllEntities()
	{
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
		
		// Eliminamos todas las entidades. La factoría se encarga de
		// desactivarlas y sacarlas previamente del mapa.
		// FRS No podemos usar el removeEntity ya que modificaría la lista mientras la recorremos
		TEntityList::iterator it = _entityList.begin();
		TEntityList::iterator end = _entityList.end();
			while(it != end)			
				entityFactory->deleteEntity( *it++ ); 
			// FRS Incrementamos el iterador antes del deleteEntity
			// ya que el método modifica el _entityList

		_entityList.clear();
		_entityMap.clear();
	} // destroyAllEntities

	//--------------------------------------------------------	

	void CMap::addEntity(CEntity *entity)
	{
		if( !_entityMap.count(entity->getEntityID() ) )
		{			
			_entityMap[entity->getEntityID()] = entity;
			_entityList.push_back(entity); 
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
		TEntityMap::const_iterator end = _entityMap.end();
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
		TEntityMap::const_iterator end = _entityMap.end();
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

	

} // namespace Logic
