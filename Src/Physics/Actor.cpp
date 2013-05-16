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
#include <Box2D\Dynamics\b2Body.h>
#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Collision\Shapes\b2CircleShape.h>
#include <Box2D\Collision\Shapes\b2PolygonShape.h>

#include <Box2D\Dynamics\b2Fixture.h>

#include <string>

namespace Physics
{
	CActor::CActor(float degree, float height, std::string type, IObserver *component) : 
		 _body(0), _bodyDef(0), _scene(0), _isTrigger(false), _heightCorrection(0),_loaded(false), _component(component)
	{
		_bodyDef = new b2BodyDef();
		_bodyDef->position.Set(degree, height); //set the starting position
		_bodyDef->angle = 0; //set the starting angle
		//_bodyDef->userData = component;
		if (type == "static")
			_bodyDef->type = b2_staticBody; //actor estático
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
			_body = 0;
		}

	} // unload

	//--------------------------------------------------------

	b2World* CActor::getPhysicWorld() { 
		assert(_scene);
		return _scene->getPhysicWorld();					
	}

	//--------------------------------------------------------

	void CActor::createFixture(float radius, bool isTrigger)
	{

		//correción en la posición del body ya que el pivote en lógica se encuentra en los pies
		_heightCorrection = radius;
		b2Vec2 pos = _body->GetPosition();
		pos.y += _heightCorrection;
		_body->SetTransform(pos, _body->GetAngle());
		_body->SetFixedRotation(true);


		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); //position, relative to body position
		circleShape.m_radius = radius; //radius

		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = isTrigger;
		fixtureDef.shape = &circleShape; //this is a pointer to the shape above
		_body->CreateFixture(&fixtureDef); //add a fixture to the body

	}

	//--------------------------------------------------------

	void CActor::createFixture(float halfWidth, float halfHeight, bool isTrigger)
	{

		//correción en la posición del body ya que el pivote en lógica se encuentra en los pies
		_heightCorrection = halfHeight;
		b2Vec2 pos = _body->GetPosition();
		pos.y += _heightCorrection;
		_body->SetTransform(pos, _body->GetAngle());
		_body->SetFixedRotation(true);


		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(halfWidth, halfHeight);

		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = isTrigger;
		fixtureDef.shape = &polygonShape; //this is a pointer to the shape above
		_body->CreateFixture(&fixtureDef); //add a fixture to the body

	}

	//--------------------------------------------------------

	void CActor::move(float x, float y)
	{
		b2Vec2 pos = _body->GetPosition();
		pos.x += x;
		pos.y += y;
		_body->SetAwake(true);
		_body->SetTransform(pos, _body->GetAngle());
	}

	//--------------------------------------------------------

	float CActor::getDegree()
	{
		return _body->GetPosition().x;
	}

	//--------------------------------------------------------

	float CActor::getHeight()
	{
		return _body->GetPosition().y - _heightCorrection;
	}

		
} // namespace Physics


