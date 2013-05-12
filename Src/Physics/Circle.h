//---------------------------------------------------------------------------
// Circle.h
//---------------------------------------------------------------------------

/**
@file Circle.h

Contiene la declaración de la clase que representa una geometría física.

@see Physics::IShape

@author Emilio Santalla
@date Febrero, 2013
*/

#include "BaseSubsystems\Math.h"
#include "IShape.h"

#ifndef __Physics_Circle_H
#define __Physics_Circle_H

namespace Physics
{

	class CCircle : public IShape
	{

	public:

		CCircle(Vector2 position, float radius) : _position(position), _radius(radius)  {}

		virtual float computeArea();
		virtual void move(Vector2 diffPos);
		virtual float getDegree();
		virtual float getHeight();
	
	protected:

		friend class CScene;

		Vector2 _position;
		float _radius;
		
		
	}; //CCircle

	

} // namespace Physics

#endif 