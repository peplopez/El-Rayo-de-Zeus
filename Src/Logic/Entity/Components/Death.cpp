/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CDeath
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Death.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Application/BaseApplication.h"


namespace Logic 
{
	IMP_FACTORY(CDeath);
	
	//---------------------------------------------------------

	bool CDeath::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;
	} // spawn
	
	//---------------------------------------------------------

	bool CDeath::accept(const TMessage &message)
	{
		return message._type == Message::DEAD;

	} // accept
	
	//---------------------------------------------------------

	void CDeath::process(const TMessage &message)
	{
		switch(message._type)
		{

		case Message::DEAD:			
			if(_entity->isPlayer() ) // PLAYER MUERTO -> GameOver
				Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
				
			else // Resto de entidades
				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			break;		
		
		}
	} // process


} // namespace Logic

