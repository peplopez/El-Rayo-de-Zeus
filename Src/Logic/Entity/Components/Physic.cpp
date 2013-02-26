/**
@file Physic.h


@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author ƒ®§
@date 26/02/2013
*/

#include "Physic.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/Actor.h"


namespace Logic {

	IMP_FACTORY(CPhysic);

	//---------------------------------------------------------

	CPhysic::CPhysic() : IComponent(), IObserver(), _physicActor(0), _movement()
	{
		_server = Physics::CServer::getSingletonPtr();
	}

	//---------------------------------------------------------

	CPhysic::~CPhysic() 
	{
		if (_physicActor) {
			_physicActor->release();
			_physicActor = 0;
		}
		_server = 0;
	} 

	//---------------------------------------------------------

	bool CPhysic::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		// Invocar al método de la clase padre
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_physicActor = createActor(entityInfo); // Crear el actor asociado al componente
		return true;
	} // spawn

	//---------------------------------------------------------

	Physics::CActor* CPhysic::createActor(const Map::CEntity *entityInfo)
	{
		// Obtenemos la posición de la entidad. 
		const TLogicalPosition logicPos = _entity->getLogicalPosition();
	
		// Leer el ancho del angular box
		assert(entityInfo->hasAttribute("physicWidth")); 
		float physicWidth = entityInfo->getFloatAttribute("physicWidth");

		// Leer la altura del angular box
		assert(entityInfo->hasAttribute("physicHeight"));
		float physicHeight = entityInfo->getFloatAttribute("physicHeight");

		// TRIGGER: Leer si es un trigger (por defecto no)
		// TODO barajar si los character pueden actuar como triggers en algún caso...
		bool isTrigger = false;
		if (entityInfo->hasAttribute("physicTrigger"))
			isTrigger = entityInfo->getBoolAttribute("physicTrigger");

		// Crear el controller de tipo cápsula
		return _server->createActor(logicPos, physicWidth, physicHeight, isTrigger, this);
	} // createActor 

	
}
