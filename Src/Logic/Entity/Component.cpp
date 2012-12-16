/**
@file Components/Component.cpp

Contiene la implementación de la clase base de los componentes.
 
@see Logic::IComponent
@see Logic::CCommunicationPort

@author David Llansó
@date Julio, 2010
*/

#include "Component.h"

//#include "Entity.h"

namespace Logic 
{
	bool IComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		_entity = entity;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	void IComponent::tick(unsigned int msecs)
	{
		processMessages();

	} // tick

} // namespace Logic
