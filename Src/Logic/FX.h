/**
@file FX.h


@see Logic::CFX
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_FX_H
#define __Logic_FX_H

#include "Entity/Component.h"
#include "Entity/Messages/Message.h"

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

		CFX() : IComponent(GetAltTypeIdOf(CFX)), _MAX_FX(10) {}		
		~CFX();
		
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		bool accept(const CMessage *message);
		void process(CMessage *message);

		void detachFromMap();
		void attachToMap(CMap* map);
		
		
	private:

		const int _MAX_FX;
		
		Graphics::CScene* _graphicalScene;

		typedef std::map<std::string, Graphics::CParticleSystem*> TParticleTable;
		TParticleTable _psTable;	

		// DICCIONARIO TAttachPoint -> BoneName
		typedef std::map<Message::TActionType, std::string> TActionToHfxMap;
			static TActionToHfxMap _ACTION_TO_HFX;
				static TActionToHfxMap _initActionToHfxMap() {
					TActionToHfxMap map;
						map[Message::TActionType::FX_BLAST]				="Blast";
						map[Message::TActionType::FX_BLAST_SMALL]		="Blast_Small";
						map[Message::TActionType::FX_CIRCLES]			="Circles";
						map[Message::TActionType::FX_ELECTRIC_ORB_RED]	="ElectricOrbRed";
						map[Message::TActionType::FX_ELECTRIC_ORB_BLUE]	="ElectricOrbBlue";
						map[Message::TActionType::FX_FLAME_THROWER]		="FlameThrower";
						map[Message::TActionType::FX_RAIN]				="Rain";
						map[Message::TActionType::FX_SMOKE]				="Smoke";
						map[Message::TActionType::FX_BURN]				="BurnHit";
						map[Message::TActionType::FX_BURN_SMALL]		="BurnHit_Small";
						map[Message::TActionType::FX_SPARKS]			="Sparks";
						map[Message::TActionType::FX_TRAILS]			="Trails";
						map[Message::TActionType::FX_TRAILS_PROJECTILE]	="TorchFireArrow";
						

					// TODO añadir on demand...
					return map;
				}

	}; // class CFX

	REG_FACTORY(CFX);

} // namespace Logic

#endif // __Logic_FX_H
