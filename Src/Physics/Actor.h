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


#include "Logic/Entity/LogicalPosition.h"

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Logic 
{
	class IPhysics;
}

namespace Physics
{

	class CActor
	{
	public:


		CActor(const Logic::TLogicalPosition &position, const float angularWidth, const float height, 
			bool trigger, Logic::IPhysics *component) : _logicPosition(position), _width(angularWidth), _height(height), _trigger(trigger), _component(component){}

		~CActor();


		
		void move(const Logic::TLogicalPosition &pos);

		bool intersects(const CActor &actor);

		Logic::TLogicalPosition getGlobalPose() {return _logicPosition;}

	protected:

		// CScene es la única que puede añadir o eliminar actores.

		friend class CScene;

		CScene *_scene;

		Logic::TLogicalPosition _logicPosition;

		float _width;
		float _height;

		bool _trigger;

		Logic::IPhysics* _component;


	}; // class CActor

} // namespace Physics

#endif 
