/**
@file FX.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CFX
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "FX.h"

#include "BaseSubsystems/Math.h"

#include "Graphics/ParticleSystem.h"
#include "Graphics/Scene.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"

#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CSpecialEffects);

	//---------------------------------------------------------
	
	CSpecialEffects::~CSpecialEffects() {
		if(!_psTable.empty())
		{			
			TParticleTable::const_iterator it = _psTable.begin();
			TParticleTable::const_iterator end   = _psTable.end();
				for(; it != end; ++it) {
					_graphicalScene->remove( it->second );				
					delete it->second;
				}
			_psTable.clear();
		}
	}

	//---------------------------------------------------------


	void CSpecialEffects::detachFromMap()
	{
		if(!_psTable.empty())
		{			
			TParticleTable::const_iterator it = _psTable.begin();
			TParticleTable::const_iterator end   = _psTable.end();
				for(; it != end; ++it) 
					_graphicalScene->remove( it->second);
		}
		_graphicalScene = 0;
	}

	//---------------------------------------------------------

	void CSpecialEffects::attachToMap(CMap* map)
	{
		_graphicalScene = map->getGraphicScene();

		if(!_psTable.empty())
		{			
			TParticleTable::const_iterator it	= _psTable.begin();
			TParticleTable::const_iterator end  = _psTable.end();
				for(; it != end; ++it) 				
					_graphicalScene->add(it->second);
		}
	}

	//---------------------------------------------------------

	// HACK FRS Esto se debería hacer con loop hasta cargar todos los efectos de la entidad
	bool CSpecialEffects::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicalScene = map->getGraphicScene();

		std::stringstream ssAux; // FRS Importante añadir ID para evitar entidades gráficas con = nombre
			ssAux << _entity->getName() << _entity->getEntityID();
			std::string	parentName = ssAux.str();

		// TODO Determinar si por llevar CFX es obligado un script minimo
		if( entityInfo->hasAttribute("fxScript1") ){
			
			std::string hfx1 = entityInfo->getStringAttribute("fxScript1");			
			if( !entityInfo->hasAttribute("fxPos1") ) 
				_psTable[hfx1] = new Graphics::CParticleSystem(	hfx1, parentName);
			else
				_psTable[hfx1] = new Graphics::CParticleSystem(
					hfx1, parentName, entityInfo->getVector3Attribute("fxPos1") );	
			_graphicalScene->add( _psTable[hfx1] );			
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CSpecialEffects::accept(const CMessage *message)
	{
		return message->getType() == Message::FX_START ||
			   message->getType() == Message::FX_STOP;		

	} // accept
	
	//---------------------------------------------------------

	// HACK FRS Falta extraer un action o arg que indique el efecto específico (ahora solo tenemos uno)
	void CSpecialEffects::process(CMessage *message)
	{
		switch(message->getType())
		{
			case Message::FX_START:	
				_psTable.begin()->second->start();			
				break;			
			case Message::FX_STOP:
				_psTable.begin()->second->stop();	
				break;		
		}
	
	} // process


	

} // namespace Logic

