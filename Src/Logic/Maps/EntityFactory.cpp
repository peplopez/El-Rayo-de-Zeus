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

#include "Map/MapEntity.h"

#include <iostream>
#include <fstream>
#include <cassert>

// HACK. Debería leerse de algún fichero de configuración
#define BLUEPRINTS_FILE_PATH "./media/maps/"

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

	} // close
	
	//---------------------------------------------------------

	typedef std::pair<std::string,CEntityFactory::TBluePrint> TStringBluePrintPair;

	bool CEntityFactory::loadBluePrints(const std::string &filename)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(BLUEPRINTS_FILE_PATH);
		completePath = completePath + filename;
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
				// Si el tipo ya estaba definido lo eliminamos.
				if(_bluePrints.count(b.type))
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
	
	//---------------------------------------------------------

	Logic::CEntity *CEntityFactory::assembleEntity(const std::string &type) 
	{
		TBluePrintMap::const_iterator it;

		it = _bluePrints.find(type);
		// si el tipo se encuentra registrado.
		if (it != _bluePrints.end()) 
		{
			CEntity* ent = new CEntity(EntityID::NextID());
			std::list<std::string>::const_iterator itc;
			// Añadimos todos sus componentes.
			IComponent* comp;
			for(itc = (*it).second.components.begin(); 
				itc !=(*it).second.components.end(); itc++)
			{
				if(CComponentFactory::getSingletonPtr()->has((*itc)))
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
	
	//---------------------------------------------------------


	Logic::CEntity *CEntityFactory::createEntity(
								const Map::CEntity *entityInfo,
								Logic::CMap *map)
	{
		CEntity *ret = assembleEntity(entityInfo->getType());

		if (!ret)
			return 0;

		// Añadimos la nueva entidad en el mapa antes de inicializarla.
		map->addEntity(ret);

		// Y lo inicializamos
		if (ret->spawn(map, entityInfo))
			return ret;
		else {
			map->removeEntity(ret);
			delete ret;
			return 0;
		}

	} // createEntity
	
	//---------------------------------------------------------

	void CEntityFactory::deleteEntity(Logic::CEntity *entity)
	{
		assert(entity);
		// Si la entidad estaba activada se desactiva al sacarla
		// del mapa.
		entity->getMap()->removeEntity(entity);
		delete entity;

	} // deleteEntity
	
	//---------------------------------------------------------

	void CEntityFactory::deferredDeleteEntity(Logic::CEntity *entity)
	{
		assert(entity);
		_pendingEntities.push_back(entity);

	} // deferredDeleteEntity
	
	//---------------------------------------------------------

	void CEntityFactory::deleteDefferedEntities()
	{
		TEntityList::const_iterator it(_pendingEntities.begin());
		TEntityList::const_iterator end(_pendingEntities.end());

		while(it != end)
		{
			CEntity *entity = *it;
			it++;
			deleteEntity(entity);
		}

		if (!_pendingEntities.empty())
			_pendingEntities.clear();

	} // deleteDefferedObjects

} // namespace Logic
