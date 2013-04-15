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
#include "Physics/ActorTrigger.h"

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
		if ( _physicalActor ) { // TODO FRS Quizá este tipo de comprobación sucia debería hacerla la propia scene en su remove
			_isTrigger ? 
				_scene->removeActor(  static_cast<Physics::CActorTrigger*>(_physicalActor) ):
				_scene->removeActor(  _physicalActor ); // Eliminar el actor de la escena	

			delete _physicalActor;
		}
		
	} 
	//---------------------------------------------------------
	
	void CPhysics::detachFromMap()
	{
		_scene->removeActor(_physicalActor);
		_scene = NULL;
	}

	//---------------------------------------------------------
	
	void CPhysics::attachToMap(CMap* map)
	{
		_scene = map->getPhysicScene();
		_scene->addActor(_physicalActor);
	}

	//---------------------------------------------------------

	bool CPhysics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		// Invocar al método de la clase padre
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getPhysicScene();
			assert(_scene && "Escena física es NULL");

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
	
		// Leer el ancho del angular box
		assert(entityInfo->hasAttribute("physicWidth")); 
		_physicWidth = entityInfo->getFloatAttribute("physicWidth");

		// Leer la altura del angular box
		assert(entityInfo->hasAttribute("physicHeight"));
		_physicHeight = entityInfo->getFloatAttribute("physicHeight");

		// TRIGGER: Leer si es un trigger (por defecto no)	
		_isTrigger = false;
		if (entityInfo->hasAttribute("physicTrigger"))
			_isTrigger = entityInfo->getBoolAttribute("physicTrigger");
		
		// TRIGGER
		if(_isTrigger)  {
			Physics::CActorTrigger* trigger = new Physics::CActorTrigger(logicPos, _physicWidth, _physicHeight, this);
				if( _scene->addActor(trigger ) ) // Añadir el actor a la escena
					return trigger ;
				else
					return 0;

		// COLLIDER
		} else {
			Physics::CActor* collider = new Physics::CActor(logicPos, _physicWidth, _physicHeight, this);
				if(_scene->addActor(collider) ) // Añadir el actor a la escena
					return collider;
				else
					return 0;
		}

	} // createActor 

	//---------------------------------------------------------

	// Crear el actor físico
	Physics::CActor* CPhysics::reCreateActor()
	{
		assert(_scene && "LOGIC::PHYSICS>> No existe escena física!");		

		delete _physicalActor;
		// Obtenemos la posición de la entidad. 
		CLogicalPosition* logicPos = new CLogicalPosition(*(_entity->getLogicalPosition()));

		_entity->getLogicalPosition();
				
		// TRIGGER
		if(_isTrigger)  {
			_physicalActor = new Physics::CActorTrigger(logicPos, _physicWidth, _physicHeight, this);
				if( _scene->addActor(_physicalActor ) ) // Añadir el actor a la escena
					return _physicalActor ;
				else
					return 0;
		// COLLIDER
		} else {
			_physicalActor = new Physics::CActor(logicPos, _physicWidth, _physicHeight, this);
				if(_scene->addActor(_physicalActor) ) // Añadir el actor a la escena
					return _physicalActor;
				else
					return 0;
		}

	} // reCreateActor 

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

	void  CPhysics::onCollision (Physics::IObserver* other) //PeP
	{

		CMessageUInt* txMsg = new CMessageUInt();
		txMsg->setType( Message::COLLISION ); 	
			
			txMsg->setUInt( static_cast<CPhysics*>(other)->getEntity()->getEntityID() );
		_entity->emitMessage(txMsg);
	}
}
