/**
@file EntityId.cpp

Contiene la implementación del tipo de identificación de entidades y una 
función para generación de nuevos IDs.
 
@see Logic::TEntityID
@see Logic::EntityID

@author David Llansó
@date Agosto, 2010
*/

#include "EntityID.h"

#include <cassert>

namespace Logic 
{
	TEntityID EntityID::_nextId = EntityID::FIRST_ID;

	//---------------------------------------------------------

	TEntityID EntityID::NextID() 
	{
		TEntityID ret = _nextId;
		assert(ret != EntityID::UNASSIGNED && "Se han asignado todos los identificadores posibles.");
		_nextId++;
		return ret;

	} // NextEntityId

}; // namespace Logic
