/**
@file Item.h

Objetos que se pueden coger del terreno de juego.

@see Logic::CItem
@see Logic::IComponent

@author FRS
@date 21-03-2013
*/
#ifndef __Logic_Item_H
#define __Logic_Item_H

#include "Logic/Entity/Component.h"


namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{
/**
	Objetos que se pueden coger del terreno de juego.
	
    @ingroup logicGroup

	@author FRS
	@date 17-02-2013
*/
	class CItem : public IComponent 
	{
		DEC_FACTORY(CItem);
	public:

		CItem() : IComponent(GetAltTypeIdOf(CItem)), _modelOnHand(""), _type(""), _timer_sparks(_PERIOD_SPARKS) {}

		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool accept(const CMessage *message);
		void process(CMessage *message);
		void tick(unsigned int msecs);


	private:

	///////////// HACK TEST FRS Para probar FX -> La idea sería editarlo para que fuera infinito
		static const int _PERIOD_SPARKS = 100; //  in msecs
		int _timer_sparks;
	/////////////////////////////////////////////////////

		std::string _modelOnHand;
		std::string _type;

	}; // class CItem

	REG_FACTORY(CItem);

} // namespace Logic

#endif // __Logic_Item_H
