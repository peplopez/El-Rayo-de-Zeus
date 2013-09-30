//---------------------------------------------------------------------------
// Actor.h
//---------------------------------------------------------------------------

/**
@file Actor.h

Contiene la declaración de la clase que representa una entidad física.

@see Physics::CActor

@author Emilio Santalla
@date Febrero, 2013
*/

#ifndef __Physics_Actor_H
#define __Physics_Actor_H

#include "Logic\Entity\LogicalPosition.h"

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Physics 
{
	class IObserver;
	class CScene;
}

class b2Body;
class b2World;
struct b2BodyDef;
struct b2FixtureDef;


namespace Physics
{

	class CActor
	{
	public:
		
		typedef std::vector<b2FixtureDef*> TFixtureDefs;
		typedef std::vector<CActor*> TContacts;

		CActor(float degrees, float height, Logic::Ring ring,  std::string type, IObserver *component); 
		
		virtual ~CActor() {}

		bool attachToScene(CScene *scene);
		bool detachFromScene();

		void createFixture(float radius, float density, float friction, float restitution, bool isTrigger); //Circle Shape
		void createFixture(float halfWidth, float halfHeigth, float density, float friction, float restitution, bool isTrigger); //BoxShape
		void createFixtures(float haldWidth, float halfHeight, float radius, float density, float friction, float restitution, bool isTrigger);


		void move(float x, float y);
		void setLinearVelocity(float x, float y);

		void disable();
		void enable();

		void disableCollisions();
		void enableCollisions();

		void onTrigger(CActor* otherActor, bool enter);
		void onCollision(CActor* otherActor, bool enter);

		
		
		/************************
			GETTER's & SETTER's
		************************/

		float getDegree();
		float getHeight();

		Logic::CLogicalPosition getLogicalPosition();
		

	protected:

		friend class CScene;

		friend class CArrow;

		bool load();
		void unload();

		b2World* getPhysicWorld(); 
		void createGhostBody();
		void deleteGhostBody();
		void CreateBodyFixtures();
		void CreateGhostFixtures();

		IObserver *getPhysicComponent() {return _component;}


	private:

		IObserver* _component;
		
		b2Body* _body;
		b2Body* _ghostBody;

		b2BodyDef* _bodyDef;
		TFixtureDefs _fixtureDefs;

		CScene* _scene;

		TContacts _contacts;
		
		bool _isTrigger;
		bool _loaded;
		bool _ghosted;

		float _heightCorrection;


		void bodyOutOfWorldBoundaries(); 


	}; // class CActor


	

} // namespace Physics

#endif 
