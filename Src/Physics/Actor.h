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

		void setBoxWidth(const float angularWidth) {_boxWidth=angularWidth;}
		float getBoxWidth() {return _boxWidth;}

		void setBoxHeight(const float height) {_boxHeight=height;}
		float getBoxHeight() {return _boxHeight;}
		
		void setTrigger(const bool trigger) {_trigger=trigger;}
		bool isTrigger() {return _trigger;}

		void setIObserver(IObserver* component) {_component=component;}
		IObserver *getIObserver() {return _component;}

		void release();

		void move(const Logic::TLogicalPosition &pos);

		bool intersects(CActor *otherActor);

		Logic::TLogicalPosition& getGlobalPose() {return _logicPosition;}

	protected:

		// CScene es la única que puede añadir o eliminar actores.

		//friend class CScene;

		//CScene *_scene;

		Logic::TLogicalPosition _logicPosition;

		float _boxWidth;
		float _boxHeight;

		bool _trigger;

		IObserver* _component;


	}; // class CActor

} // namespace Physics

#endif 
