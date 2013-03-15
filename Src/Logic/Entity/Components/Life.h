/**
@file Life.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_Life_H
#define __Logic_Life_H

#include "Logic/Entity/Component.h"
#include "Graphics/Billboard.h" //Pablo

namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el daño a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	TODO  Si la vida pasa a ser 0 que la entidad muera (poner animación de muerte?)
	y si es el jugador habrá que terminar el juego. Si la vida sigue siendo mayor 
	que 0 trás un golpe ¿poner la animación de herido?.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
*/
	class CLife : public IComponent
	{
		DEC_FACTORY(CLife);
	public:

		CLife() : IComponent(GetAltTypeIdOf(CLife)), _LIFE_MAX(0), _life(0), _lifeBarPosition(0), _lifeBarWidth(0), _lifeBarHeight(0),_modifiyingLife(0){}

		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		void deactivate();
		
		bool accept(const CMessage *message);

		void process(CMessage *message);

		void updateLife(float);


	protected:


		// Billboard
		Graphics::CBillboard _lifeBar;
		/**
		Vida máxima de la entidad
		*/
		int _LIFE_MAX;

		/**Atributo que indica la magnitud de la vida de la entidad.*/
		int _life;

		/** Atributo que indica el offset de la Y del posicionamiento del billboard de vida.*/
		float _lifeBarPosition;

		/** Pablo. 01-02-2013
		Atributo que indica la anchura del billboard
		*/
		float _lifeBarWidth;

		/** Atributo que indica la altura del billboard		*/
		float _lifeBarHeight;

		int _modifiyingLife;

		void CLife::modifyLife(int);

	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
