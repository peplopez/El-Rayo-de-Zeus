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

		CActor(float degrees, float height, Logic::Ring ring,  std::string type, IObserver *component); 
		
		virtual ~CActor() {}

		bool attachToScene(CScene *scene);
		bool detachFromScene();

		void createFixture(float radius, bool isTrigger); //Circle Shape
		void createFixture(float halfWidth, float halfHeigth, bool isTrigger); //BoxShape
		void createFixture(float haldWidth, float halfHeight, float radius, bool isTrigger);

		void move(float x, float y);
		void setLinearVelocity(float x, float y);

		void onTrigger(CActor* otherActor, bool enter);
		void onCollision(CActor*);

		
		
		/************************
			GETTER's & SETTER's
		************************/

		float getDegree();
		float getHeight();
		

	protected:

		friend class CScene;

		bool load();
		void unload();

		b2World* getPhysicWorld(); 
		void createGhostBody();
		void deleteGhostBody();

		IObserver *getPhysicComponent() {return _component;}


	private:

		IObserver* _component;
		
		b2Body* _body;
		b2Body* _ghostBody;

		b2BodyDef* _bodyDef;
		TFixtureDefs _fixtureDefs;

		CScene* _scene;
		
		bool _isTrigger;
		bool _loaded;
		bool _ghosted;

		float _heightCorrection;

		void CreateGhostFixtures();
		void bodyOutOfWorldBoundaries(); 

		


	}; // class CActor


	

} // namespace Physics

#endif 
