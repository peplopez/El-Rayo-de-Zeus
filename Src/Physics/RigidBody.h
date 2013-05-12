//---------------------------------------------------------------------------
// RigidBody.h
//---------------------------------------------------------------------------

/**
@file RigidBody.h

Contiene la declaración de la clase que representa una entidad física.

@see Physics::CRigidBody

@author Emilio Santalla
@date Febrero, 2013
*/

#ifndef __Physics_RigidBody_H
#define __Physics_RigidBody_H

#include "BaseSubsystems\Math.h"
// Predeclaración de clases para ahorrar tiempo de compilación
namespace Physics
{
	class IShape;
}

namespace Physics
{
	
	struct Material
	{
		float density;
		float restitution;
	};

	struct MassData
	{
		float mass;
		float inv_mass;
	};


	class CRigidBody
	{
	public:

		CRigidBody();

		CRigidBody(Vector2 position, float radius, float density, float restitution); // Circle Constructor
		CRigidBody(Vector2 min, Vector2 max, float density, float restitution); // AABB Constructor
		
				
		/************************
			GETTER's & SETTER's
		************************/
	
		void setSpeed(float x, float y) {_velocity.x = x; _velocity.y = y;}
		
		


	protected:

		friend class CScene;
		friend class CActor;

		IShape* _shape;
		Material _material;
		MassData _massData;
		Vector2 _velocity;
		Vector2 _force;



	}; // class CActor


	

} // namespace Physics

#endif 
