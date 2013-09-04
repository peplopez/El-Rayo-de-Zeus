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


#define DEBUG 0
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
			TParticleTable::const_iterator it	= _psTable.begin();
			TParticleTable::const_iterator end  = _psTable.end();
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
			TParticleTable::const_iterator it	= _psTable.begin();
			TParticleTable::const_iterator end  = _psTable.end();
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
		
		std::string varScript(		"fxScript0");		
		std::string varLooped(		"fxLooped0");
		std::string varPos(			"fxPos0");
		std::string varCompoName(	"fxCompoName0");
		std::string varCompoMsec(	"fxCompoMsec0");
		std::string varDiffuse(		"fxDiffuse0");
	    std::string varSpecular(	"fxSpecular0");
		std::string varAuto(		"fxAutoStart0");

		std::vector<std::string> autoHFXs; // vector de PS con autoStart

		// READ FXs & CREATE PSs
		for(int i = 0; i < _MAX_FX ; ++i) {	// 10 FXs maximo por entidad deberia ser suficente

			if( !entityInfo->hasAttribute( varScript ) )
				break;
			std::string hfx = entityInfo->getStringAttribute( varScript );	

			Vector3 relativePos(Vector3::ZERO);
				if( entityInfo->hasAttribute( varPos ) ) 
					relativePos = entityInfo->getVector3Attribute( varPos );

			bool isLooped = false;
				if( entityInfo->hasAttribute( varLooped ) ) 
					isLooped = entityInfo->getBoolAttribute( varLooped );

			std::string compoName = "";
				if( entityInfo->hasAttribute( varCompoName ) )
					compoName = entityInfo->getStringAttribute( varCompoName );	

			int compoMsec = 0;
				if( entityInfo->hasAttribute( varCompoMsec ) )
					compoMsec = entityInfo->getIntAttribute( varCompoMsec );	
		
			Vector3 lightColorDiff(Vector3::ZERO);
				if( entityInfo->hasAttribute( varDiffuse ) ) 
					lightColorDiff = entityInfo->getVector3Attribute( varDiffuse );

			Vector3 lightColorSpec(Vector3::ZERO);
				if( entityInfo->hasAttribute( varSpecular ) ) 
					lightColorSpec = entityInfo->getVector3Attribute( varSpecular );
			
			if( entityInfo->hasAttribute( varAuto ) && entityInfo->getBoolAttribute( varAuto ) )
				autoHFXs.push_back(hfx);
			
			_psTable[hfx] = new Graphics::CParticleSystem(
								hfx, _entity->getGraphicalName(), relativePos,
								isLooped,  compoName, compoMsec,
								lightColorDiff, lightColorSpec );	

			++varScript		[ varScript.length()	- 1];
			++varLooped		[ varLooped.length()	- 1];
			++varPos		[ varPos.length()		- 1];
			++varCompoName	[ varCompoName.length()	- 1];
			++varCompoMsec	[ varCompoMsec.length()	- 1];
			++varDiffuse	[ varDiffuse.length()	- 1];
			++varSpecular   [ varSpecular.length()	- 1];
			++varAuto		[ varAuto.length()		- 1];
		}

		attachToMap(map); // Add PSs to Scene

		// Start automático de PSs con autoStart
		for(int i = 0; i < autoHFXs.size(); ++i)
			_psTable[ autoHFXs[i] ]->start();

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
		Graphics::CParticleSystem* ps = 0;

		if(	message->getAction() != Message::TActionType::UNDEF ) {

			std::string hfx = _ACTION_TO_HFX[ message->getAction() ];
			assert(hfx.length() && "Action recibido no se corresponde con ningun recurso HFX");

			ps = _psTable[hfx];							
				if(!ps) {		// FRS Si existe el sistema de partículas, lo construimos en la posicion 0,0,0	
					ps = _psTable[hfx] = new Graphics::CParticleSystem( hfx, _entity->getGraphicalName() );
					_graphicalScene->add(ps);
				}

		} else if( !_psTable.empty() ) {
			ps = _psTable.begin()->second;	// Si no se especifica Action actuamos sobre el primer PS	
			
		} else { 
			return; // Action UNDEF + _psTable.empty() -> No podemos hacer nada
		}	
	
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

