/**
@file Map.cpp

Contiene la implementación de la clase CMap, Un mapa lógico.

@see Logic::Map

@author David Llansó
@date Agosto, 2010
*/

#include "Map.h"

#include "Logic/Entity/Entity.h"
#include "EntityFactory.h"

#include "Map/MapParser.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include <cassert>

// HACK. Debería leerse de algún fichero de configuración
#define MAP_FILE_PATH "./media/maps/"

namespace Logic {
		
	CMap* CMap::createMapFromFile(const std::string &filename)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(MAP_FILE_PATH);
		completePath = completePath + filename;
		// Parseamos el fichero
		if(!Map::CMapParser::getSingletonPtr()->parseFile(completePath))
		{
			assert(!"No se ha podido parsear el mapa.");
			return false;
		}

		// Si se ha realizado con éxito el parseo creamos el mapa.
		CMap *map = new CMap(filename);

		// Extraemos las entidades del parseo.
		Map::CMapParser::TEntityList entityList = 
			Map::CMapParser::getSingletonPtr()->getEntityList();

		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		Map::CMapParser::TEntityList::const_iterator it, end;
		it = entityList.begin();
		end = entityList.end();

		// Creamos todas las entidades lógicas.
		for(; it != end; it++)
		{
			// La propia factoría se encarga de añadir la entidad al mapa.
			CEntity *entity = entityFactory->createEntity((*it),map);
			assert(entity && "No se pudo crear una entidad del mapa");
		}

		return map;

	} // createMapFromFile

	//--------------------------------------------------------

	CMap::CMap(const std::string &name)
	{
		_name = name;
		_scene = Graphics::CServer::getSingletonPtr()->createScene(name);

	} // CMap

	//--------------------------------------------------------

	CMap::~CMap()
	{
		destroyAllEntities();
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::getSingletonPtr()->removeScene(_scene);

	} // ~CMap

	//--------------------------------------------------------

	bool CMap::activate()
	{
		Graphics::CServer::getSingletonPtr()->setScene(_scene);

		TEntityMap::const_iterator it, end;
		end = _entityMap.end();
		it = _entityMap.begin();

		bool correct = true;

		// Activamos todas las entidades registradas en el mapa.
		for(; it != end; it++)
			correct = (*it).second->activate() && correct;

		return correct;

	} // getEntity

	//--------------------------------------------------------

	void CMap::deactivate()
	{
		TEntityMap::const_iterator it, end;
		end = _entityMap.end();
		it = _entityMap.begin();

		// Desactivamos todas las entidades activas registradas en el mapa.
		for(; it != end; it++)
			if((*it).second->isActivated())
				(*it).second->deactivate();

		Graphics::CServer::getSingletonPtr()->setScene(0);

	} // getEntity

	//---------------------------------------------------------

	void CMap::tick(unsigned int msecs) 
	{
		TEntityMap::const_iterator it;

		for( it = _entityMap.begin(); it != _entityMap.end(); ++it )
			(*it).second->tick(msecs);

	} // tick

	//--------------------------------------------------------

	typedef std::pair<TEntityID,CEntity*> TEntityPair;

	void CMap::addEntity(CEntity *entity)
	{
		if(_entityMap.count(entity->getEntityID()) == 0)
		{
			TEntityPair elem(entity->getEntityID(),entity);
			_entityMap.insert(elem);
		}

	} // addEntity

	//--------------------------------------------------------

	void CMap::removeEntity(CEntity *entity)
	{
		if(_entityMap.count(entity->getEntityID()) != 0)
		{
			if(entity->isActivated())
				entity->deactivate();
			entity->_map = 0;
			_entityMap.erase(entity->getEntityID());
		}

	} // removeEntity

	//--------------------------------------------------------

	void CMap::destroyAllEntities()
	{
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		TEntityMap::const_iterator it, end;
		it = _entityMap.begin();
		end = _entityMap.end();

		// Eliminamos todas las entidades. La factoría se encarga de
		// desactivarlas y sacarlas previamente del mapa.
		while(it != end)
		{
			CEntity* entity = (*it).second;
			it++;
			entityFactory->deleteEntity(entity);
		}

		_entityMap.clear();

	} // removeEntity

	//--------------------------------------------------------

	CEntity* CMap::getEntityByID(TEntityID entityID)
	{
		if(_entityMap.count(entityID) == 0)
			return 0;
		return (*_entityMap.find(entityID)).second;

	} // getEntityByID

	//--------------------------------------------------------

	CEntity* CMap::getEntityByName(const std::string &name, CEntity *start)
	{
		TEntityMap::const_iterator it, end;
		end = _entityMap.end();

		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		if (start)
		{
			it = _entityMap.find(start->getEntityID());
			// si la entidad no existe devolvemos NULL.
			if(it == end)
				return 0;
			it++;
		}
		else
			it = _entityMap.begin();

		for(; it != end; it++)
		{
			// si hay coincidencia de nombres devolvemos la entidad.
			if (!(*it).second->getName().compare(name))
				return (*it).second;
		}
		// si no se encontró la entidad devolvemos NULL.
		return 0;

	} // getEntityByName

	//--------------------------------------------------------

	CEntity* CMap::getEntityByType(const std::string &type, CEntity *start)
	{
		TEntityMap::const_iterator it, end;
		end = _entityMap.end();

		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		if (start)
		{
			it = _entityMap.find(start->getEntityID());
			// si la entidad no existe devolvemos NULL.
			if(it == end)
				return 0;
			it++;
		}
		else
			it = _entityMap.begin();

		for(; it != end; it++)
		{
			// si hay coincidencia de nombres devolvemos la entidad.
			if (!(*it).second->getType().compare(type))
				return (*it).second;
		}
		// si no se encontró la entidad devolvemos NULL.
		return 0;

	} // getEntityByType

} // namespace Logic
