//---------------------------------------------------------------------------
// RigidBody.cpp
//---------------------------------------------------------------------------

/**
@file RigidBody.cpp


@see Physics::RigidBody

@author Emilio Santalla
@date Febrero, 2013
*/

#include "RigidBody.h"
#include "IShape.h"
#include "Circle.h"
#include "AABB.h"
#include "BaseSubsystems\Math.h"

namespace Physics
{
	CRigidBody::CRigidBody(Vector2 position, float radius, float density, float restitution)
		: _velocity(Vector2(0,0)), _diffPos(Vector2(0,0)), _force(Vector2(0,0))
	{
		_shape = new CCircle(position, radius);
		_material.density = density;
		_material.restitution = restitution;
		_massData.mass = _shape->computeArea() * density;
		
		//masa = 0 equivale a masa infinita
		//Calculamos la inversa de la masa sólo en el caso de que no sea infinita para evitar divisiones por 0
		if (_massData.mass == 0)
			_massData.inv_mass = 0; 
		else
			_massData.inv_mass = 1 / _massData.mass;
	}
	//--------------------------------------------------------

	CRigidBody::CRigidBody(Vector2 min, Vector2 max, float density, float restitution)
		: _velocity(Vector2(0,0)), _diffPos(Vector2(0,0)), _force(Vector2(0,0))
	{
		_shape = new CAABB(min, max);
		_material.density = density;
		_material.restitution = restitution;
		_massData.mass = _shape->computeArea() * density;
		
		//masa = 0 equivale a masa infinita
		//Calculamos la inversa de la masa sólo en el caso de que no sea infinita para evitar divisiones por 0
		if (_massData.mass == 0)
			_massData.inv_mass = 0; 
		else
			_massData.inv_mass = 1 / _massData.mass;
	}

} // namespace Physics
