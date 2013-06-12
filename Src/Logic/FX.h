/**
@file FX.h


@see Logic::CFX
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_FX_H
#define __Logic_FX_H

#include "Logic/Entity/Component.h"

namespace Graphics
{
	class CParticleSystem;
	class CScene;
}

namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{
/**	
    @ingroup logicGroup

	@author FRS
	@date Mayo, 2013
*/
	class CFX : public IComponent
	{
		DEC_FACTORY(CFX);
	public:

		CFX() : IComponent(GetAltTypeIdOf(CFX)) {}		
		~CFX();
		
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		bool accept(const CMessage *message);
		void process(CMessage *message);

		void detachFromMap();
		void attachToMap(CMap* map);
		
		
	private:
		
		Graphics::CScene* _graphicalScene;

		typedef std::map<std::string, Graphics::CParticleSystem*> TParticleTable;
		TParticleTable _psTable;	

		// DICCIONARIO TAttachPoint -> BoneName
		typedef std::map<Message::TActionType, std::string> TActionToHfxMap;

			static const TActionToHfxMap ACTION_TO_HFX;

				static TActionToHfxMap _initActionToHfxMap() {
					TActionToHfxMap map;
						map[Message::TActionType::FX_BLAST]			="Blast.hfx";
						map[Message::TActionType::FX_CIRCLES]		="Circles.hfx";
						map[Message::TActionType::FX_ELECTRIC_ORB]	="ElectricOrb.hfx";
						map[Message::TActionType::FX_FLAME_THROWER]	="FlameThrower.hfx";
						map[Message::TActionType::FX_RAIN]			="Rain.hfx";
						map[Message::TActionType::FX_SMOKE]			="Smoke.hfx";
						map[Message::TActionType::FX_SMOKE_BURN]	="BurnHit.hfx";
						map[Message::TActionType::FX_SPARKS]		="Sparks.hfx";
						map[Message::TActionType::FX_TRAILS]		="Trails.hfx";

					// TODO añadir on demand...
					return map;
				}

	}; // class CFX

	REG_FACTORY(CFX);

} // namespace Logic

#endif // __Logic_FX_H
