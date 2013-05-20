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


namespace Physics
{

	class CActor
	{
	public:


		CActor(float degrees, float height, Logic::Ring ring,  std::string type, IObserver *component); 
		
		virtual ~CActor() {}

		bool attachToScene(CScene *scene);
		bool detachFromScene();

		void createFixture(float radius, bool isTrigger); //Circle Shape
		void createFixture(float halfWidth, float halfHeigth, bool isTrigger); //BoxShape
		void createFixture(float haldWidth, float halfHeight, float radius, bool isTrigger);

		void move(float x, float y);
		void setLinearVelocity(float x, float y);
		
		
		/************************
			GETTER's & SETTER's
		************************/

		float getDegree();
		float getHeight();

		void setIObserver(IObserver* component) {_component=component;}
		IObserver *getIObserver() {return _component;}

	protected:

		bool load();
		void unload();

		b2World* getPhysicWorld(); 


	private:

		
		b2Body* _body;

		b2BodyDef* _bodyDef;

		CScene* _scene;
		
		bool _isTrigger;

		bool _loaded;

		float _heightCorrection;

		IObserver* _component;


	}; // class CActor


	

} // namespace Physics

#endif 
