//-----------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp


@see Physics::CScene

@author Emilio Santalla
@date Febrero 2013
*/

#include "Physics/IObserver.h"
#include "Physics/Scene.h"
#include "Physics/Actor.h"
#include "Physics/ActorTrigger.h"
#include "Physics/Circle.h"

#include <assert.h>
#include <iostream>


#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "PHYSICS::SCENE>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace Physics
{

	CScene::~CScene() 
	{
		deactivate();	

	} // ~CScene
	
	//--------------------------------------------------------

	bool CScene::activate()
	{
		return true;
	} // activate

	//--------------------------------------------------------

	bool CScene::deactivate()
	{
		return true;
	} // deactivate
	
	//--------------------------------------------------------
	
	void CScene::tick(unsigned int msecs)
	{	
		this->simulate(msecs); // Empezar la simulación física.
	} // tick

	


	/************
		ACTORS
	************/

	// TODO FRS hacerlas inline? 
	bool CScene::addCircleActor(CActor* collider)
	{

		_circleColliders.push_back(collider);
		return true;
	} // addActor

	bool CScene::addCircleActor(CActorTrigger* trigger)
	{
		_circleTriggers.push_back(trigger);
		return true;
	} // addActor

	bool CScene::addAABBActor(CActor* collider)
	{

		_AABBColliders.push_back(collider);
		return true;
	} // addActor

	bool CScene::addAABBActor(CActorTrigger* trigger)
	{
		_AABBTriggers.push_back(trigger);
		return true;
	} // addActor


		

	//--------------------------------------------------------


	void CScene::removeCircleActor(CActor* collider)
	{
		TCircleColliders::iterator colliderIndex = std::find(_circleColliders.begin(), _circleColliders.end(), collider);
		if (colliderIndex != _circleColliders.end())
			_circleColliders.erase(colliderIndex); // FRS El delete es responsabilidad del creador (Logic::CPhysics)		
	} // removeActor

	void CScene::removeCircleActor(CActorTrigger* trigger)
	{
		TCircleTriggers::iterator triggerIndex = std::find(_circleTriggers.begin(), _circleTriggers.end(), trigger);
		if (triggerIndex != _circleTriggers.end())			
			_circleTriggers.erase(triggerIndex);// FRS El delete es responsabilidad del creador (Logic::CPhysics)		
	} // removeActor

	void CScene::removeAABBActor(CActor* collider)
	{
		TCircleColliders::iterator colliderIndex = std::find(_AABBColliders.begin(), _AABBColliders.end(), collider);
		if (colliderIndex != _AABBColliders.end())
			_AABBColliders.erase(colliderIndex); // FRS El delete es responsabilidad del creador (Logic::CPhysics)		
	} // removeActor

	void CScene::removeAABBActor(CActorTrigger* trigger)
	{
		TCircleTriggers::iterator triggerIndex = std::find(_AABBTriggers.begin(), _AABBTriggers.end(), trigger);
		if (triggerIndex != _AABBTriggers.end())			
			_AABBTriggers.erase(triggerIndex);// FRS El delete es responsabilidad del creador (Logic::CPhysics)		
	} // removeActor

	

	/******************
		SIMULATION
	*****************/
	
	void CScene::simulate(unsigned int timeDiff)
	{	
		for (size_t i = 0; i < _circleColliders.size(); ++i)
			updateCirclePos(_circleColliders[i], timeDiff );

		//for (size_t i = 0; i < _AABBColliders.size() - 1; ++i)
		//	updateAABBPos(_AABBColliders[i], timeDiff );
		//
		//for (size_t i = 0; i < _circleTriggers.size() - 1; ++i)
		//	checkCircleTrigger(_circleTriggers[i], timeDiff );

		//for (size_t i = 0; i < _AABBTriggers.size() - 1; ++i)
		//	checkAABBTrigger(_AABBTriggers[i], timeDiff );

	} // simulate	

	//--------------------------------------------------------

	void CScene::updateCirclePos(CActor* circleCollider, unsigned int timeDiff) 
	{
		

		Manifold manifold;
		manifold.A = circleCollider->getRigid();

		for (size_t i=0; i < _circleColliders.size(); ++i)
		{
			if( circleCollider != _circleColliders[i])
			{
				manifold.B = _circleColliders[i]->getRigid();
				if( CheckCircleCircleCollision(manifold) ) 
				{
					circleCollider->getIObserver()->onCollision(_circleColliders[i]->getIObserver());
					_circleColliders[i]->getIObserver()->onCollision(circleCollider->getIObserver());
					ResolveCollision(manifold);
					PositionalCorrection(manifold);
				}
			}
		}

		//circleCollider->getRigid()->_shape->move(circleCollider->getRigid()->_velocity * timeDiff);

		
		
	}

	//--------------------------------------------------------

	void CScene::PositionalCorrection( Manifold &m )
	{
	  const float percent = 0.2; // usually 20% to 80%
	  const float k_slop = 0.01; // usually 0.01 to 0.1
	  Vector2 correction = std::max( m.penetration + k_slop, 0.0f ) * (m.A->_massData.mass + m.B->_massData.mass) * percent * m.normal;
	  m.A->_shape->move(-m.A->_massData.inv_mass * correction);
	  m.B->_shape->move(m.B->_massData.inv_mass * correction);
	}

	//--------------------------------------------------------

	bool CScene::CheckCircleCircleCollision(Physics::Manifold &m)
	{
		// Seteo un puntero a cada Shape del manifold
		CCircle *A = static_cast<CCircle*>(m.A->_shape);
		CCircle *B = static_cast<CCircle*>(m.B->_shape);
 

		// Vector from A to B
		Vector2 n = B->_position - A->_position ;

		if (n.x < -180)
			n.x += 360;
		else if (n.x > 180)
			n.x -= 360;


		float r = A->_radius + B->_radius;
		float r2 = r * r;
 
		if(n.squaredLength() > r2)
			return false;
 
		// Circles have collided, now compute manifold
		float d = n.length(); // perform actual sqrt
 
		// If distance between circles is not zero
		if(r != 0)
		{
			// Distance is difference between radius and distance
			m.penetration = r - d;
 
			// Utilize our d since we performed sqrt on it already within Length( )
			// Points from A to B, and is a unit vector
			m.normal = n / d;
			return true;
		}
 
		// Circles are on same position
		else
		{
			// Choose random (but consistent) values
			m.penetration = A->_radius;
			m.normal = Vector2( 1, 0 );
			return true;
		}
	}

	//--------------------------------------------------------

	void CScene::ResolveCollision(Manifold &m)
	{
		  // Calculate relative velocity
		  Vector2 rv = m.B->_velocity - m.A->_velocity;
 
		  // Calculate relative velocity in terms of the normal direction
		  float velAlongNormal = rv.dotProduct(m.normal);
 
		  // Do not resolve if velocities are separating
		  if(velAlongNormal > 0)
			return;
 
		  // Calculate restitution
		  float e = std::min( m.A->_material.restitution, m.B->_material.restitution);
 
		  // Calculate impulse scalar
		  float j = -(1 + e) * velAlongNormal;
		  j /= m.A->_massData.inv_mass + m.B->_massData.inv_mass;
 
		  // Apply impulse
		  Vector2 impulse = j * m.normal;
		  m.A->_velocity = -(m.A->_massData.inv_mass * impulse);
		  m.B->_velocity = m.B->_massData.inv_mass * impulse;

	}



	//--------------------------------------------------------

	//void CScene::checkTriggers() {

	//	for (size_t i = 0; i < _triggers.size(); ++i)	
	//		for (size_t j = 0; j < _colliders.size(); ++j)
	//			
	//			if ( _colliders[j]->intersects( _triggers[i] ) ) {
	//				if( _triggers[i]->enters( _colliders[j] ) ) {						
	//					LOG("Trigger Enter")
	//					_triggers[i]->getIObserver()->onTrigger( _colliders[j]->getIObserver(), true);
	//					_colliders[j]->getIObserver()->onTrigger( _triggers[i]->getIObserver(), true);	
	//				}
	//				
	//			} else if( _triggers[i]->exits( _colliders[j] ) ) {						
	//					LOG("Trigger Exit")
	//					_triggers[i]->getIObserver()->onTrigger( _colliders[j]->getIObserver(), false);
	//					_colliders[j]->getIObserver()->onTrigger( _triggers[i]->getIObserver(), false);
	//			}

	//} // checkTriggers


	//--------------------------------------------------------




} // namespace Physics