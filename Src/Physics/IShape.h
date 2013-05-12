//---------------------------------------------------------------------------
// IShape.h
//---------------------------------------------------------------------------

/**
@file IShape.h

Contiene la declaración de la clase que representa una geometría física.

@see Physics::IShape

@author Emilio Santalla
@date Febrero, 2013
*/


#ifndef __Physics_IShape_H
#define __Physics_IShape_H

#include "BaseSubsystems\Math.h"

namespace Physics
{

	class IShape
	{

	public:
				

		virtual float computeArea() = 0;
		virtual void move(Vector2 diffPos) = 0;
		virtual float getDegree() = 0;
		virtual float getHeight() = 0;
	
		
	}; 

	
} // namespace Physics

#endif 
