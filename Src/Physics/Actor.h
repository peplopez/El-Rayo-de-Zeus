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

namespace Physics 
{
	class IObserver;
}

namespace Physics
{

	class CActor
	{
	public:

		CActor();
		CActor(const Logic::TLogicalPosition &position, const float angularWidth, const float height, 
					bool trigger, IObserver *component);

		~CActor();

		void setLogicPos(const Logic::TLogicalPosition &position) {_logicPosition=position;}
		Logic::TLogicalPosition &getLogicPos() {return _logicPosition;}

		void setWidth(const float angularWidth) {_width=angularWidth;}
		float getWidth() {return _width;}

		void setHeight(const float height) {_height=height;}
		float getHeight() {return _height;}
		
		void setTrigger(const bool trigger) {_trigger=trigger;}
		bool getTrigger() {return _trigger;}

		void setIObserver(IObserver* component) {_component=component;}
		IObserver *getIObserver() {return _component;}

		void release();

		void move(const Logic::TLogicalPosition &pos);

		bool intersects(const CActor *actor);

		Logic::TLogicalPosition& getGlobalPose() {return _logicPosition;}

	protected:

		// CScene es la única que puede añadir o eliminar actores.

		//friend class CScene;

		//CScene *_scene;

		Logic::TLogicalPosition _logicPosition;

		float _width;
		float _height;

		bool _trigger;

		IObserver* _component;


	}; // class CActor

} // namespace Physics

#endif 
