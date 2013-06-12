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

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::FX>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic 
{
	IMP_FACTORY(CFX);

	//---------------------------------------------------------

	const CFX::TActionToHfxMap CFX::ACTION_TO_HFX = CFX::_initActionToHfxMap();

	//---------------------------------------------------------
	
	CFX::~CFX() {
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


	void CFX::detachFromMap()
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

	void CFX::attachToMap(CMap* map)
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
	bool CFX::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
					

		// READ FXs & CREATE PSs
		for(int i = 1; ; ++i) {
			
			ssAux << "fxScript" << i;
			std::string varName = ssAux.str();
				if( !entityInfo->hasAttribute( varName) )
					break;
			
			std::string hfx1 = entityInfo->getStringAttribute("fxScript1");			
			if( !entityInfo->hasAttribute("fxPos1") ) 
				
			else
				_psTable[hfx1] = new Graphics::CParticleSystem(
				hfx1, _entity->getGraphicalName(), entityInfo->getVector3Attribute("fxPos1") );	
					

			_psTable[hfx1] = new Graphics::CParticleSystem(	hfx1, parentName);
		}

		attachToMap(map); // Add PSs to Scene

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CFX::accept(const CMessage *message)
	{
		return message->getType() == Message::FX_START	||
			   message->getType() == Message::FX_STOP	||
			   message->getType() == Message::CHANGE_RING;

	} // accept
	
	//---------------------------------------------------------

	// HACK FRS Falta extraer un action o arg que indique el efecto específico (ahora solo tenemos uno)
	void CFX::process(CMessage *message)
	{
		if(_psTable.empty())
			return;

		switch(message->getType())
		{
			case Message::FX_START:	
				LOG("Start");
				_psTable.begin()->second->start();			
				break;			
			case Message::FX_STOP:
				LOG("Stop");
				_psTable.begin()->second->stop();	
				break;		
			case Message::CHANGE_RING:
				LOG("Change Ring");
				break;
		}
	
	} // process


	

} // namespace Logic

