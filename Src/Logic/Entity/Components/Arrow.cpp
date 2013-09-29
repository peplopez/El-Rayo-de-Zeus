/**
@file Arrow.cpp

Contiene la declaración del componente que se utiliza para representar flechas en el múndo físico
@see Logic::Arrow


@see Logic::CArrow
@see Logic::CPhysics
@see Physics::IObserver

@author PeP
@date 21-09-2013
*/
#include "Arrow.h"
#include "Logic/Entity/Entity.h"
#include "Map/Entity.h"


#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h" // TODO PeP: sería óptimo enviar un unsigned short???
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageUint.h"

#include "Logic/Entity/Components/Life.h"


#include "Physics/Actor.h"
#include "Physics/Scene.h"
//#include "../../../Physics/Box2D/Dynamics/b2World.h";
#include "Logic/Maps/EntityFactory.h"
#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::PHYSIC_CHARACTER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic {

	IMP_FACTORY(CArrow);

	/*bool CArrow::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{			
		
		if(entityInfo->hasAttribute("angularSpeed"))
			_angularSpeed = entityInfo->getFloatAttribute("angularSpeed");
			return true;
		}*/
	//---------------------------------------------------------
			
	bool CArrow::activate()
	{
 		if (_entity->getLogicalPosition()->getSense()==Sense::LEFT)
			_entity->yaw(3.1416);
		_entity->getLogicalPosition()->setHeight(10);
		_diffHeight=	_entity->getLogicalPosition()->getHeight();
		this->_physicalActor->move(0,_entity->getLogicalPosition()->getHeight());

	/*	CMessage *txMsg = new CMessage();	
		txMsg->setType(Message::FX_START);
		txMsg->setAction(Message::FX_TRAILS_PROJECTILE);
		_entity->emitMessage(txMsg,this);
		*/

		return true;
	}

	bool CArrow::accept(const CMessage *message)
	{
		return false; //inhabilitado hasta que de verdad me haga falta		
	} 


	//---------------------------------------------------------

	// Acumulamos las diferentes coordenadas vector de desplazamiento para usarlo posteriormente en  el método tick.
	// De esa forma, si recibimos varios mensajes AVATAR_WALK tendremos la última coordenada de cada tipo (degrees, height, ring, base)
	void CArrow::process(CMessage *message)
	{
		
	} 

	//---------------------------------------------------------

	void CArrow::tick(unsigned int msecs) 
	{
		_diffDegrees+=_angularProjectileSpeed*(_entity->getLogicalPosition()->getSense()==Sense::LEFT?1:-1) * msecs;
		_diffHeight=(_negativeYVelocity-=0.3)*msecs;
		// Llamar al método de la clase padre (IMPORTANTE).


	  float t = 1 / 60.0f; // seconds per time step (at 60fps)
	  Vector2 startingVelocity= Vector2(_diffDegrees,_diffHeight);

	//  this->getPhysicalActor()->getPhysicWorld()->GetGravity();

      Vector2 stepVelocity = t * startingVelocity; // m/s
	  /*Vector2 stepGravity = t * t * this->_physicalActor->getPhysicWorld();
		  -> m_world->GetGravity(); // m/s/s
  
      return startingPosition + n * stepVelocity + 0.5f * (n*n+n) * stepGravity;
	  */

		IComponent::tick(msecs);

		// Actualizar la posición y orientación de la entidad lógica 
		// usando la información proporcionada por el motor de física	
		// Este a genera  SET_TRANSFORM por debajo que informa al CGraphics

		_entity->yaw(Math::fromDegreesToRadians(_entity->getLogicalPosition()->getDegree() - _physicalActor->getDegree()));

		
		CLogicalPosition auxPos = _physicalActor->getLogicalPosition();

		auxPos.setBase(_entity->getLogicalPosition()->getBase() + _diffBase);	
		auxPos.setSense(_entity->getLogicalPosition()->getSense());				
		_entity->setLogicalPosition(&auxPos); 		

		_physicalActor->setLinearVelocity(_diffDegrees, _diffHeight);
		
		//Ponemos el movimiento a cero		
		_diffDegrees = 0;
		_diffHeight = 0; 
		_diffBase = 0;
	}


	/**************
		IOBSERVER
	***************/

	//Se invoca cuando se produce una colisión entre una entidad física y un trigger.
	void CArrow::onCollision(IObserver* other) {
		CPhysics::onCollision(other);
		LOG(_entity->getName() << ": \"Auch! Me he chocado!\"");		

		CEntity* victim=static_cast<CPhysics*>(other)->getEntity();
		
		if ((victim->hasComponent<CLife>() || victim->getType()=="World")  && victim!=NULL && victim!=_entity->getFather() && _entity->getFather()!=NULL)
		{
			CMessageUInt *m2 = new CMessageUInt();
			m2->setUInt(_attackPower);
			m2->setType(Message::LIFE_MODIFIER);						
			m2->setAction(Message::DAMAGE);						
			victim->emitMessage(m2);
			
		 	if (victim->getType()=="World")			
				if (_negativeYVelocity<0)
						DestroyArrow();
		}
	}

	//---------------------------------------------------------
	void CArrow::DestroyArrow()
	{
		
	/*	CMessage *txMsg = new CMessage();	
		txMsg->setType(Message::FX_START);
		txMsg->setAction(Message::FX_TRAILS_PROJECTILE);
		_entity->emitMessage(txMsg,this);*/
		if (_entity!=NULL)
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
	}

	void  CArrow::onTrigger (Physics::IObserver* other, bool enter) 
	{
		CPhysics::onTrigger(other, enter);
	//	CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);

		#if DEBUG
			if(enter)
				LOG(_entity->getName() << ": \"Hora estoy dentro de " << static_cast<CPhysics*>(other)->getEntity()->getName() << "\"")
			else
				LOG(_entity->getName() << ": \"Hora estoy fuera  de " << static_cast<CPhysics*>(other)->getEntity()->getName() << "\"")
		#endif
	}
} // Logic


