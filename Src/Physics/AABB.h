//---------------------------------------------------------------------------
// AABB.h
//---------------------------------------------------------------------------

/**
@file AABB.h

Contiene la declaración de la clase que representa una geometría física.

@see Physics::IShape

@author Emilio Santalla
@date Febrero, 2013
*/

#include "BaseSubsystems\Math.h"
#include "IShape.h"

#ifndef __Physics_AABB_H
#define __Physics_AABB_H

namespace Physics
{

	class CAABB : public IShape
	{

	public:

		CAABB(Vector2 min, Vector2 max) : _min(min), _max(max)  {}

		virtual float computeArea();
		virtual void move(Vector2 diffPos);
		virtual float getDegree();
		virtual float getHeight();

	protected:

		Vector2 _min;
		Vector2 _max;
		
		
	}; //CAABB

	

} // namespace Physics

#endif 