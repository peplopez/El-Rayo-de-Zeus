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
	class CSpecialEffects : public IComponent
	{
		DEC_FACTORY(CSpecialEffects);
	public:

		CSpecialEffects() : IComponent(GetAltTypeIdOf(CSpecialEffects)) {}		
		~CSpecialEffects();
		
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		bool accept(const CMessage *message);
		void process(CMessage *message);

		void detachFromMap();
		void attachToMap(CMap* map);
		
		
	private:
		
		Graphics::CScene* _graphicalScene;

		typedef std::map<std::string, Graphics::CParticleSystem*> TParticleTable;
		TParticleTable _psTable;		

	}; // class CFX

	REG_FACTORY(CSpecialEffects);

} // namespace Logic

#endif // __Logic_FX_H
