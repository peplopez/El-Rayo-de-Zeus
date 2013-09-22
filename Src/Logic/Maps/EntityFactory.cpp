/**
@file EntityFactory.cpp

Contiene la implementación de la clase factoría de entidades
del juego.

@see Logic::CEntityFactory

@author David Llansó García, Marco Antonio Gómez Martín
@date Agosto, 2010
*/
#include "EntityFactory.h"
#include "ComponentFactory.h"
#include "Logic/Entity/Entity.h"
#include "Map.h"
#include "Map/Entity.h"
#include "Map/MapParser.h"

#include <iostream>
#include <fstream>
#include <cassert>

// HACK. Debería leerse de algún fichero de configuración
#define FILE_PATH "./media/maps/"

/**
Sobrecargamos el operador >> para la lectura de blueprints.
Cada línea equivaldrá a una entidad donde la primera palabra es el tipo
y las siguientes son los componentes que tiene.
*/
std::istream& operator>>(std::istream& is, Logic::CEntityFactory::TBluePrint& blueprint) 
{
	is >> blueprint.type;
	std::string aux;
	getline(is,aux,'\n');
	std::istringstream components(aux);
	while(!components.eof())
	{
		aux.clear();
		components >> aux;
		if(!aux.empty())
			blueprint.components.push_back(aux);
	}
	return is;
}

namespace Logic
{
	/****************
		SINGLETON
	****************/

	CEntityFactory *CEntityFactory::_instance = 0;
	
	//---------------------------------------------------------

	CEntityFactory::CEntityFactory()
	{
		_instance = this;

	} // CEntityFactory
	
	//---------------------------------------------------------

	CEntityFactory::~CEntityFactory()
	{
		_instance = 0;

	} // ~CEntityFactory
	
	//---------------------------------------------------------

	bool CEntityFactory::Init() 
	{
		assert(!_instance && "Segunda inicialización de Logic::CEntityFactory no permitida!");

		new CEntityFactory();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init
	
	//---------------------------------------------------------

	void CEntityFactory::Release() 
	{
		assert(_instance && "Logic::CEntityFactory no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CEntityFactory::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CEntityFactory::close() 
	{
		unloadBluePrints();
		unloadArchetypes();
		unloadPatternEntities();
	} // close
	
	
	/******************
		ENTITIES
	*******************/


	//---------------- PATTERN ENTITIES -------------------------------


	typedef std::pair<std::string,Map::CEntity> TStringCEntityPair;

	bool CEntityFactory::loadPatternEntities(const std::string &mapFileName)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string mapPath(FILE_PATH);
			mapPath.append(mapFileName);
			mapPath.append(".txt");

		if(!Map::CMapParser::getSingletonPtr()->parseFile(mapPath)) {
			assert(!"No se ha podido parsear mapa patron.");
			return false;
		}

		// Extraemos las entidades del parseo.
		 Map::CMapParser::TEntities parsedEntities = Map::CMapParser::getSingletonPtr()->getParsedEntities();
		
		 _patternEntities.clear(); // Vaciamos la lista de patrones actual

		// SAVE & MERGE WITH ARCHETYPES6
		//	Con este premerge ahorramos tener que repetir un merge por cada fillMapUsingPattern
		Map::CMapParser::TEntities::const_iterator entIt  = parsedEntities.cbegin();
		Map::CMapParser::TEntities::const_iterator entEnd = parsedEntities.cend();
		for(; entIt != entEnd; ++entIt) {

			_patternEntities.push_back( *(*entIt) );
			Map::CEntity& entityToMerge = _patternEntities.back();

			TArchetypeMap::const_iterator archIt  = _archetypes.find( entityToMerge.getType() );
			TArchetypeMap::const_iterator archEnd = _archetypes.cend();
			if (archIt !=  archEnd)		
				entityToMerge.mergeWithArchetype(archIt->second);
		}
		
		return true;
	} // loadPatternEntities

	//---------------------------------------------------------

	void CEntityFactory::unloadPatternEntities()
	{
		_patternEntities.clear();
	} // unloadPatternEntities
	
	//---------------------------------------------------------

	void CEntityFactory::fillMapUsingPattern(Logic::CMap *map)
	{		
		// Creamos todas las entidades lógicas.
		TPatternEntities::const_iterator it =  _patternEntities.cbegin();
		TPatternEntities::const_iterator end = _patternEntities.cend();
			for(; it != end; ++it)		{
				CEntity *entity = createEntity( *it, map); // La propia factoría se encarga de añadir la entidad al mapa.
				assert(entity && "No se pudo crear una entidad patron en el mapa");
			}
	}

	//-----------------  CREATION ----------------------------------------

	Logic::CEntity *CEntityFactory::createEntity(
		Map::CEntity entityInfo, Logic::CMap *map, bool useArchetype)
	{		
		// Despejamos las palabras clave del atributo "type" 
		// para sacar partido del subsiguiente mergeWithArchetype
		entityInfo.replaceAttrKeywords( map->getProperties(), "type" );

		// MERGE WITH ARCHETYPE
		//	Se busca en el std::map de archetypes el tipo del *entityInfo
		//	Si encuentra el archetype de ese tipo, fusiono con él en entityInfo
		if(useArchetype) {
			TArchetypeMap::const_iterator it = _archetypes.find(entityInfo.getType());
			TArchetypeMap::const_iterator end = _archetypes.cend();
				if (it !=  end)		
					entityInfo.mergeWithArchetype(it->second);
		}
		
		// Despejamos las palabras clave en el resto de atributos (!= "type")
		entityInfo.replaceAttrKeywords( map->getProperties() );		

		CEntity *newEntity = assembleEntity( entityInfo.getType() );
			if (!newEntity)
				return 0;
		
		map->addEntity(newEntity);// Añadimos la nueva entidad en el mapa.

		// Y la inicializamos		
		if ( newEntity->spawn(map, &entityInfo) )
			return newEntity;
		else {
			map->removeEntity(newEntity);
			delete newEntity;
			return 0;
		}

	} // createEntity

	//-----------------------------------------------------------------

	Logic::CEntity *CEntityFactory::assembleEntity(const std::string &type) 
	{
		TBluePrintMap::const_iterator it;

		it = _bluePrints.find(type); // TBluePrintMap: mapa <"tipoEntidad", TBluePrint>
		// si el tipo se encuentra registrado.
		if (it != _bluePrints.cend()) 
		{
			CEntity* ent = new CEntity(EntityID::NextID());
			std::list<std::string>::const_iterator itc;
			
			// Añadimos todos sus componentes.
			IComponent* comp;
			for(itc = (*it).second.components.cbegin(); // TBluePrintMap.second: TBluePrint: struct { type (string), components (list<string>) }
				itc !=(*it).second.components.cend(); itc++)
			{
				if(CComponentFactory::getSingletonPtr()->has((*itc))) // itc (string cName)
				{
					comp = CComponentFactory::getSingletonPtr()->create((*itc));
				}
				else
				{
					assert(!"Nombre erroneo de un componente, Mira a ver si están todos bien escritos en el fichero de blueprints");
					delete ent;
					return 0;
				}
				if(comp)
					ent->addComponent(comp);
			}

			return ent;
		}
		return 0;

	} // assembleEntity
	

	//----------------- DELETION  ----------------------------------------

	void CEntityFactory::deferredDeleteEntity(Logic::CEntity *entity)
	{
		assert(entity);
		_pendingEntities.push_back(entity);
	} // deferredDeleteEntity
	
	
	//-----------------------------------------------------------------

	void CEntityFactory::deleteDefferedEntities()
	{
		TEntityList::const_iterator it(_pendingEntities.cbegin());
		TEntityList::const_iterator end(_pendingEntities.cend());
			for(; it != end; ++it)
				deleteEntity(*it);
			_pendingEntities.clear();
	} // deleteDefferedObjects
	
	
	//-----------------------------------------------------------------

	void CEntityFactory::deleteEntity(Logic::CEntity *entity)
	{
		assert(entity);		
		entity->getMap()->removeEntity(entity);// Si la entidad estaba activada se desactiva al sacarla del mapa.
		delete entity; // El delete nos toca a nosotros
	} // deleteEntity




	

	

	/**********************
		ARCHETYPES
	*********************/

	bool CEntityFactory::loadArchetypes(const std::string &filename)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string archetypesPath(FILE_PATH);
			archetypesPath.append(filename);
			archetypesPath.append(".txt");

		if(!Map::CMapParser::getSingletonPtr()->parseFile(archetypesPath)) {
			assert(!"No se ha podido parsear el archetypes.");
			return false;
		}

		// Extraemos las entidades del parseo.
		Map::CMapParser::TEntities entityList = 
			Map::CMapParser::getSingletonPtr()->getParsedEntities();
				
		//Guardamos cada uno de los archetypes en el std::map de archetypes
		Map::CMapParser::TEntities::const_iterator it  = entityList.cbegin();
		Map::CMapParser::TEntities::const_iterator end = entityList.cend();
			for (; it != end; ++it)			
				_archetypes[ (*it)->getName() ] = *(*it);

		return true;

	} // loadArchetypes

	//---------------------------------------------------------

	void CEntityFactory::unloadArchetypes()
	{
		_archetypes.clear();

	} // unloadArchetypes
	
	
	


	/*******************
		BLUEPRINTS
	*******************/

	typedef std::pair<std::string,CEntityFactory::TBluePrint> TStringBluePrintPair;

	bool CEntityFactory::loadBluePrints(const std::string &filename)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(FILE_PATH);
			completePath.append(filename);
			completePath.append(".txt");
		
		// Abrimos el fichero
		std::ifstream in(completePath.c_str());        
		if(!in)
			return false;

		while(!in.eof())
		{
			// Se lee un TBluePrint del fichero
			TBluePrint b;
			in >> b;
			// Si no era una línea en blanco
			if(!b.type.empty())
			{				
				if(_bluePrints.count(b.type)) // Si el tipo ya estaba definido lo eliminamos.
					_bluePrints.erase(b.type);

				// Añadimos a la tabla
				TStringBluePrintPair elem(b.type,b);
				_bluePrints.insert(elem);
			}
		}

		return true;

	} // loadBluePrints
	
	//---------------------------------------------------------

	void CEntityFactory::unloadBluePrints()
	{
		_bluePrints.clear();

	} // unloadBluePrints


	

} // namespace Logic
