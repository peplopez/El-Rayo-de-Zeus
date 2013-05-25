//---------------------------------------------------------------------------
// Actor.cpp
//---------------------------------------------------------------------------

/**
@file Actor.cpp


@see Physics::CActor

@author Emilio Santalla
@date Febrero, 2013
*/
#include <string>

#include "Actor.h"
#include "Scene.h"
#include "IObserver.h"

#include <Box2D\Dynamics\b2Body.h>
#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Dynamics\b2Fixture.h>

#include <Box2D\Dynamics\Joints\b2DistanceJoint.h>
#include <Box2D\Collision\Shapes\b2CircleShape.h>
#include <Box2D\Collision\Shapes\b2PolygonShape.h>

#include "Physics\Scales.h"

#include <string>

namespace Physics
{
	CActor::CActor(float degree, float height, Logic::Ring ring, std::string type, IObserver *component) : 
		 _body(0), _ghostBody(0), _bodyDef(0), _scene(0), _isTrigger(false), _heightCorrection(0), _loaded(false), _ghosted(false), _component(component)
	{
		_bodyDef = new b2BodyDef();

		if (degree > 180)
			degree -= 360;

		switch (ring)
		{
		case Logic::Ring::CENTRAL_RING:
			_bodyDef->position.Set(degree * PHYSIC_DOWNSCALE, height * PHYSIC_DOWNSCALE);
			break;
		case Logic::Ring::LOWER_RING:
			_bodyDef->position.Set(degree * PHYSIC_DOWNSCALE, height * PHYSIC_DOWNSCALE - 50 * PHYSIC_DOWNSCALE);
			break;
		case Logic::Ring::UPPER_RING:
			_bodyDef->position.Set(degree * PHYSIC_DOWNSCALE, height * PHYSIC_DOWNSCALE + 50 * PHYSIC_DOWNSCALE);
			break;
		}

		_bodyDef->angle = 0;
		_bodyDef->userData = this;
		if (type == "static")
			_bodyDef->type = b2_staticBody;
		else if (type == "dynamic")
			_bodyDef->type = b2_dynamicBody;
		else if (type == "kinematic")
			_bodyDef->type = b2_kinematicBody;
	}

	//--------------------------------------------------------

	bool CActor::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡El actor físico debe asociarse a una escena!!");
		
		if(_loaded) {	// Si el elemento ya había sido cargado... 
			if(_scene != scene)
				return false;	// ... por otro gestor de escena.
			else
				return true;	// ... por la misma escena
		
		} else { // Si no estaba cargado forzamos su carga.
			_scene = scene;
			return load();
		}

	} // attachToScene

	//--------------------------------------------------------

	bool CActor::detachFromScene()
	{
		if(!_loaded) // Si el elemento no estaba cargado no se puede quitar de
			return false; // una escena, ya que no pertenecerá a ninguna.
		
		else { // Si el elemento se había cargado forzamos su descarga.
			assert(_scene && "¡¡El actor físico debe estar asociada a una escena!!");
			unload();
			_scene = 0;
			return true;
		}

	} // deattachFromScene

	//--------------------------------------------------------

	bool CActor::load()
	{
		try{
			_body = getPhysicWorld()->CreateBody(_bodyDef);		
			_loaded = true;

		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;
	} // load

	//--------------------------------------------------------

	void CActor::unload()
	{
		if(_body){
				
			_loaded = false;
			getPhysicWorld()->DestroyBody(_body);
			if (_ghostBody)
				getPhysicWorld()->DestroyBody(_ghostBody);
			_body = 0;
		}

	} // unload

	//--------------------------------------------------------

	b2World* CActor::getPhysicWorld() { 
		assert(_scene);
		return _scene->getPhysicWorld();					
	}

	//--------------------------------------------------------

	void CActor::createFixture(float radius, float density, float friction, float restitution, bool isTrigger)
	{
		radius = radius * PHYSIC_DOWNSCALE;
		//correción en la posición del body ya que el pivote en lógica se encuentra en los pies
		_heightCorrection = radius;
		b2Vec2 pos = _body->GetPosition();
		pos.y += _heightCorrection ;
		_body->SetTransform(pos, _body->GetAngle());
		_body->SetFixedRotation(true);


		b2CircleShape* circleShape = new b2CircleShape();
		circleShape->m_p.Set(0, 0); 
		circleShape->m_radius = radius;

		b2FixtureDef* fixtureDef = new b2FixtureDef();
		fixtureDef->isSensor = isTrigger;
		fixtureDef->shape = circleShape; 
		fixtureDef->density = density;
		fixtureDef->friction = friction;
		fixtureDef->restitution = restitution;
		_body->CreateFixture(fixtureDef); 
		_fixtureDefs.push_back(fixtureDef);

	}

	//--------------------------------------------------------

	void CActor::createFixture(float halfWidth, float halfHeight, float density, float friction, float restitution, bool isTrigger)
	{
		halfWidth = halfWidth * PHYSIC_DOWNSCALE;
		halfHeight = halfHeight * PHYSIC_DOWNSCALE;
		//correción en la posición del body ya que el pivote en lógica se encuentra en los pies
		_heightCorrection = halfHeight;
		b2Vec2 pos = _body->GetPosition();
		pos.y += _heightCorrection;
		_body->SetTransform(pos, _body->GetAngle());
		_body->SetFixedRotation(true);


		b2PolygonShape* polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(halfWidth, halfHeight);

		b2FixtureDef* fixtureDef = new b2FixtureDef();
		fixtureDef->isSensor = isTrigger;
		fixtureDef->shape = polygonShape;
		fixtureDef->density = density;
		fixtureDef->friction = friction;
		fixtureDef->restitution = restitution;
		_body->CreateFixture(fixtureDef); 
		_fixtureDefs.push_back(fixtureDef);

	}
	//--------------------------------------------------------

	void CActor::createFixtures(float halfWidth, float halfHeight, float radius, float density, float friction, float restitution, bool isTrigger)
	{

		halfWidth = halfWidth * PHYSIC_DOWNSCALE;
		halfHeight = halfHeight * PHYSIC_DOWNSCALE;
		radius = radius * PHYSIC_DOWNSCALE;

		_heightCorrection = radius + halfHeight;
		b2Vec2 pos = _body->GetPosition();
		pos.y += _heightCorrection;
		_body->SetTransform(pos, _body->GetAngle());
		_body->SetFixedRotation(true);

		b2CircleShape* circleShape1 = new b2CircleShape();
		circleShape1->m_p.Set(0, -halfHeight); //position, relative to body position
		circleShape1->m_radius = radius; //radius

		b2FixtureDef* fixtureDef1 = new b2FixtureDef();
		fixtureDef1->isSensor = isTrigger;
		fixtureDef1->density = density / 3;
		fixtureDef1->friction = friction / 3;
		fixtureDef1->restitution = restitution / 3;

		fixtureDef1->shape = circleShape1; //this is a pointer to the shape above
		_body->CreateFixture(fixtureDef1); //add a fixture to the body
		_fixtureDefs.push_back(fixtureDef1);

		b2CircleShape* circleShape2 = new b2CircleShape();
		circleShape2->m_p.Set(0, halfHeight); //position, relative to body position
		circleShape2->m_radius = radius; //radius

		b2FixtureDef* fixtureDef2 = new b2FixtureDef();
		fixtureDef2->isSensor = isTrigger;
		fixtureDef2->density = density / 3;
		fixtureDef2->friction = friction / 3;
		fixtureDef2->restitution = restitution / 3;

		fixtureDef2->shape = circleShape2; //this is a pointer to the shape above
		_body->CreateFixture(fixtureDef2); //add a fixture to the body
		_fixtureDefs.push_back(fixtureDef2);

		b2PolygonShape* polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(halfWidth, halfHeight);

		b2FixtureDef* fixtureDef3 = new b2FixtureDef();
		fixtureDef3->isSensor = isTrigger;
		fixtureDef3->density = density / 3;
		fixtureDef3->friction = friction / 3;
		fixtureDef3->restitution = restitution / 3;

		fixtureDef3->shape = polygonShape; //this is a pointer to the shape above
		_body->CreateFixture(fixtureDef3); //add a fixture to the body
		_fixtureDefs.push_back(fixtureDef3);
	}
	//--------------------------------------------------------

	void CActor::move(float x, float y)
	{
		x = x * PHYSIC_DOWNSCALE;
		y = y * PHYSIC_DOWNSCALE;

		b2Vec2 pos = _body->GetPosition();

		pos.x += x;
		pos.y += y;

		if (pos.x > 180 * PHYSIC_DOWNSCALE)
			pos.x -= 360 * PHYSIC_DOWNSCALE;
		else if (pos.x < -180 * PHYSIC_DOWNSCALE)
			pos.x += 360 * PHYSIC_DOWNSCALE;

		_body->SetAwake(true);
		_body->SetTransform(pos, _body->GetAngle());
	}

	//--------------------------------------------------------
	void CActor::setLinearVelocity(float x, float y)
	{
	
		_body->SetLinearVelocity(b2Vec2(x * PHYSIC_DOWNSCALE, y * PHYSIC_DOWNSCALE));
		if (_ghostBody)
			_ghostBody->SetLinearVelocity(b2Vec2(x * PHYSIC_DOWNSCALE, y * PHYSIC_DOWNSCALE));
	}

	//--------------------------------------------------------

	float CActor::getDegree()
	{
		return _body->GetPosition().x * PHYSIC_UPSCALE;
	}

	//--------------------------------------------------------

	float CActor::getHeight()
	{

		return _body->GetPosition().y * PHYSIC_UPSCALE - _heightCorrection * PHYSIC_UPSCALE;
	}

	//--------------------------------------------------------

	Logic::CLogicalPosition CActor::getLogicalPosition()
	{
		Logic::CLogicalPosition pos;
		pos.setDegree(_body->GetPosition().x * PHYSIC_UPSCALE);
		
		float height = _body->GetPosition().y * PHYSIC_UPSCALE - _heightCorrection * PHYSIC_UPSCALE;
		if (height > 50)
		{
			pos.setHeight(height - 50);
			pos.setRing(Logic::Ring::UPPER_RING);
		}
		else if (height < 0)
		{
			pos.setHeight(height + 50);
			pos.setRing(Logic::Ring::LOWER_RING);
		}

		else
		{
			pos.setHeight(height);
			pos.setRing(Logic::Ring::CENTRAL_RING);
		}

		return pos;

	}

	//--------------------------------------------------------

	void CActor::onTrigger(CActor* otherActor, bool enter)
	{
		if (_component && otherActor->getPhysicComponent())
			_component->onTrigger(otherActor->getPhysicComponent(), enter);

		else if (!otherActor->getPhysicComponent() && enter)
		{
			if (!_ghosted)
			{
				_scene->deferredGhostBody(this);
				_ghosted = true;
			}

		}
		else if (!otherActor->getPhysicComponent() && !enter)
		{
			if (_ghosted)
			{
				bodyOutOfWorldBoundaries();
				_scene->deferredUnghostBody(this);
				_ghosted = false;
			}
		}

	}

	//--------------------------------------------------------
	void CActor::createGhostBody()
	{
		_ghostBody = getPhysicWorld()->CreateBody(_bodyDef);
		_ghostBody->SetFixedRotation(true);


		b2Transform transform = _body->GetTransform();
		if (transform.p.x > 0)
			transform.p.x -= 360.0f * PHYSIC_DOWNSCALE;
		else
			transform.p.x += 360.0f * PHYSIC_DOWNSCALE;

		_ghostBody->SetTransform(transform.p, transform.q.GetAngle());
		CreateGhostFixtures();


		b2DistanceJointDef jointDef;
		jointDef.bodyA = _body;
		jointDef.bodyB = _ghostBody;
		jointDef.collideConnected = false;
		jointDef.length = 720.0f * PHYSIC_DOWNSCALE;


		jointDef.localAnchorA = _body->GetPosition();
		jointDef.localAnchorB = _ghostBody->GetPosition();


		//create the joint
		b2DistanceJoint* joint = (b2DistanceJoint*)getPhysicWorld()->CreateJoint( &jointDef );	

	}

	//--------------------------------------------------------

	void CActor::deleteGhostBody()
	{

		getPhysicWorld()->DestroyBody(_ghostBody);
		_ghostBody = 0;
	}

	//--------------------------------------------------------

	void CActor::CreateGhostFixtures()
	{
		for (int i = 0; i<_fixtureDefs.size(); ++i)
			_ghostBody->CreateFixture(_fixtureDefs[i]);
	}

	//--------------------------------------------------------

	void CActor::onCollision(CActor* otherActor)
	{
		if (_component && otherActor->getPhysicComponent())
			_component->onCollision(otherActor->getPhysicComponent());
	}
	
	//--------------------------------------------------------

	void CActor::bodyOutOfWorldBoundaries()
	{
		if (_body->GetPosition().x > (180 * PHYSIC_DOWNSCALE) || 
			_body->GetPosition().x < (-180* PHYSIC_DOWNSCALE))
		{
			b2Body* aux;
			aux = _body;
			_body = _ghostBody;
			_ghostBody = aux;
		}
	}

	//--------------------------------------------------------

	void CActor::disableCollisions()
	{

		for (b2Fixture* f = _body->GetFixtureList(); f; f = f->GetNext())
		{
			b2Filter filter = f->GetFilterData();
			filter.maskBits = 0;
			f->SetFilterData(filter);
		}
		if (_ghostBody)
		{
			for (b2Fixture* f = _ghostBody->GetFixtureList(); f; f = f->GetNext())
			{
				b2Filter filter = f->GetFilterData();
				filter.maskBits = 0;
				f->SetFilterData(filter);
			}
		}
	}

	//--------------------------------------------------------

	void CActor::enableCollisions()
	{
		for (b2Fixture* f = _body->GetFixtureList(); f; f = f->GetNext())
		{
			b2Filter filter = f->GetFilterData();
			filter.maskBits = 0xFFFF;
			f->SetFilterData(filter);
		}
		if (_ghostBody)
		{
			for (b2Fixture* f = _ghostBody->GetFixtureList(); f; f = f->GetNext())
			{
				b2Filter filter = f->GetFilterData();
				filter.maskBits = 0xFFFF;
				f->SetFilterData(filter);
			}
		}
	}
		
} // namespace Physics


