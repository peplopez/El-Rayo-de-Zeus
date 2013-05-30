/**
@file Physics.h

Contiene la implementación del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysics
@see Logic::IComponent
@see Logic::CPhysicController

@author ƒ®§
@date 26/02/2013
*/

#include "Physics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "Logic/Maps/Map.h"

#include "Map/MapEntity.h"

#include "Physics/Scene.h"
#include "Physics/Actor.h"


#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::PHYSIC>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic {

	IMP_FACTORY(CPhysics);

	//---------------------------------------------------------

	CPhysics::~CPhysics() 
	{
		if ( _physicalActor ) 
		{ 
			_scene->remove( _physicalActor ); // Eliminar el actor de la escena	
			delete _physicalActor;
		}
		
	} 
	//---------------------------------------------------------
	
	void CPhysics::detachFromMap()
	{
		_scene->remove(_physicalActor);
		_scene = NULL;
	}

	//---------------------------------------------------------
	
	void CPhysics::attachToMap(CMap* map)
	{
		_scene = map->getPhysicScene();
		_scene->add(_physicalActor);
	}

	//---------------------------------------------------------

	bool CPhysics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		// Invocar al método de la clase padre
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getPhysicScene();
			assert(_scene && "Escena física es NULL");

		assert(entityInfo->hasAttribute("shape") && "falta definir atributo shape en el mapa");
			_shape = entityInfo->getStringAttribute("shape");

		if (entityInfo->hasAttribute("physicTrigger"))
			_isTrigger = entityInfo->getBoolAttribute("physicTrigger");

		_physicalActor = createActor(entityInfo); // Crear el actor asociado al componente
			if(!_physicalActor)
				return false;
	

		return true;
	} // spawn

	//---------------------------------------------------------

	// Crear el actor físico
	Physics::CActor* CPhysics::createActor(const Map::CEntity *entityInfo)
	{
		assert(_scene && "LOGIC::PHYSICS>> No existe escena física!");		

		// Obtenemos la posición de la entidad. 
		CLogicalPosition* logicPos = _entity->getLogicalPosition();

		assert(entityInfo->hasAttribute("physicType") && "falta definir atributo tipo estatico/dinamico/kinematico para el actor en el mapa");
		std::string physicType = entityInfo->getStringAttribute("physicType");
		
		float physicBodyOffset = 0;
		if (entityInfo->hasAttribute("physicBodyOffset"))
			physicBodyOffset = entityInfo->getFloatAttribute("physicBodyOffset");

		Physics::CActor* actor = new Physics::CActor(logicPos->getDegree(), logicPos->getHeight() + physicBodyOffset, logicPos->getRing(), physicType, this);

		if (entityInfo->hasAttribute("physicRadius"))
			_radius = entityInfo->getFloatAttribute("physicRadius");

		if (entityInfo->hasAttribute("physicHalfWidth"))
			_halfWidth = entityInfo->getFloatAttribute("physicHalfWidth");
		
		if (entityInfo->hasAttribute("physicHalfHeight"))
			_halfHeight = entityInfo->getFloatAttribute("physicHalfHeight");

		if (entityInfo->hasAttribute("density"))
			_density = entityInfo->getFloatAttribute("density");

		if (entityInfo->hasAttribute("friction"))
			_friction = entityInfo->getFloatAttribute("friction");

		if (entityInfo->hasAttribute("restitution"))
			_restitution = entityInfo->getFloatAttribute("friction");

		if (_shape == "circle")
			actor->createFixture(_radius, _density, _friction, _restitution, _isTrigger); 
		else if (_shape == "box")
			actor->createFixture(_halfWidth, _halfHeight, _density, _friction, _restitution, _isTrigger); 
		else if (_shape == "capsule")
			actor->createFixtures(_halfWidth, _halfHeight, _radius, _density, _friction, _restitution, _isTrigger);

		if (_scene->add(actor))
			return actor;

		return 0;

	} // createActor 

	//---------------------------------------------------------

	void  CPhysics::onTrigger (Physics::IObserver* other, bool enter) 
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
			txMsg->setUInt( static_cast<CPhysics*>(other)->getEntity()->getEntityID() );
		_entity->emitMessage(txMsg);


		// FRS: Adjuntamos el entityID para aquellos componentes que necesitan
		// conocer la entidad que ha entrado en este trigger para causarle
		// su efecto correspondiente (p.e enviarle un LIFE_MODIFIER desde CLifeModifier)
		// EntityID mejor que CEntity* para evitar que el receptor del mensaje 
		// se encuentre con un puntero a una entidad borrada (muerta)

	} // onTrigger

	//---------------------------------------------------------

	void  CPhysics::onCollision (Physics::IObserver* other) //PeP
	{

		CMessageUInt* txMsg = new CMessageUInt();
		txMsg->setType( Message::COLLISION ); 	
			
			txMsg->setUInt( static_cast<CPhysics*>(other)->getEntity()->getEntityID() );
		_entity->emitMessage(txMsg);
	}

	//---------------------------------------------------------

	void  CPhysics::disableCollisions()
	{
		_physicalActor->disableCollisions();
		
	}

	//---------------------------------------------------------

	void  CPhysics::enableCollisions()
	{
		_physicalActor->enableCollisions();
	}
}
