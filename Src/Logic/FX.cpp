/**
@file FX.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CFX
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "FX.h"

#include <BaseSubsystems/Math.h>
#include <Graphics/ParticleSystem.h>
#include <Graphics/Scene.h>
#include <Map/MapEntity.h>

#include "Entity/Entity.h"
#include "Maps/Map.h"


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

	CFX::TActionToHfxMap CFX::_ACTION_TO_HFX = CFX::_initActionToHfxMap();


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
		
		std::string varScript("fxScript0");
		std::string varPos("fxPos0");

		// READ FXs & CREATE PSs
		for(int i = 0; i < _MAX_FX ; ++i) {	// 10 FXs maximo por entidad deberia ser suficente

			if( !entityInfo->hasAttribute( varScript ) )
				break;
			
			std::string hfx = entityInfo->getStringAttribute( varScript );	
				if( entityInfo->hasAttribute( varPos ) ) 
					_psTable[hfx] = new Graphics::CParticleSystem( hfx, _entity->getGraphicalName(), 
										entityInfo->getVector3Attribute( varPos ) );					
				else
					_psTable[hfx] = new Graphics::CParticleSystem( hfx, _entity->getGraphicalName() );	

			++varScript[ varScript.length() - 1];
			++varPos   [ varPos.length()    - 1];
		}

		attachToMap(map); // Add PSs to Scene

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CFX::accept(const CMessage *message)
	{
		return message->getType() == Message::FX_START	||
			   message->getType() == Message::FX_STOP;

	} // accept
	
	//---------------------------------------------------------

	
	void CFX::process(CMessage *message)
	{
		if(_psTable.empty())
			return;

		Graphics::CParticleSystem* ps = 
			message->getAction() == Message::TActionType::UNDEF ?
				_psTable.begin()->second :
				_psTable[ _ACTION_TO_HFX[ message->getAction() ] ]; 
				// Si no se especifica Action actuamos sobre el primer PS

		assert(ps && "La entidad no usa el FX que especifica el ActionType recibido");
		// TODO FRS Quizá habría que generar el sistema, aunque sea en la posicion 0,0,0

		switch(message->getType())
		{
			case Message::FX_START:	
				LOG("Start");
				ps->start();			
				break;		
			
			case Message::FX_STOP:
				LOG("Stop");
				ps->stop();
				break;		
		}
	
	} // process


	

} // namespace Logic

