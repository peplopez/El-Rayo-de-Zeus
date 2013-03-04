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

#include "Logic/Entity/Messages/MessageUInt.h"
#include "Physics/Server.h"
#include "Physics/Actor.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::PHYSIC>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic {

	IMP_FACTORY(CPhysic);

	//---------------------------------------------------------

	CPhysic::CPhysic() :_physicActor(0), _movDegrees(0), _movHeight(0), _movRing(0), _movBase(0)
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
		const float physicWidth = entityInfo->getFloatAttribute("physicWidth");

		// Leer la altura del angular box
		assert(entityInfo->hasAttribute("physicHeight"));
		const float physicHeight = entityInfo->getFloatAttribute("physicHeight");

		// TRIGGER: Leer si es un trigger (por defecto no)
		// TODO barajar si los character pueden actuar como triggers en algún caso...
		bool isTrigger = false;
		if (entityInfo->hasAttribute("physicTrigger"))
			isTrigger = entityInfo->getBoolAttribute("physicTrigger");

		// Crear el controller de tipo cápsula
		return _server->createActor(logicPos, physicWidth, physicHeight, isTrigger, this);
	} // createActor 

	//---------------------------------------------------------

	void  CPhysic::onTrigger (Physics::IObserver* other, bool enter) 
	{
		// Construimos un mensaje de tipo TOUCHED o UNTOUCHED 
		// y lo enviamos a todos los componentes de la entidad.

		CMessageUInt* txMsg = new CMessageUInt();		
			txMsg->setType( Message::TRIGGER ); 	
			txMsg->setAction( 
				enter ? 
				Message::TRIGGER_ENTER : 
				Message::TRIGGER_EXIT
			);			
			txMsg->setUInt( static_cast<CPhysic*>(other)->getEntity()->getEntityID() );
		_entity->emitMessage(txMsg);


		// FRS: Adjuntamos el entityID para aquellos componentes que necesitan
		// conocer la entidad que ha entrado en este trigger para causarle
		// su efecto correspondiente (p.e enviarle un LIFE_MODIFIER desde CLifeModifier)
		// EntityID mejor que CEntity* para evitar que el receptor del mensaje 
		// se encuentre con un puntero a una entidad borrada (muerta)

	} // onTrigger

	

	
}
