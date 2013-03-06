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
		CActor(const Logic::TLogicalPosition &position, const float angularWidth, const float height, IObserver *component);
		virtual ~CActor() {}

		virtual void release(); // TODO FRS con hacerlo virtual ya se ejecuta el delete sobre el this del hijo o hay que impl en el hijo?
								// Es más, es necesario hacer el delete sobre el hijo, o el del padre ya lanzaria el destructor del hijo?
		
		// UNDONE RS si la clase CLogicalPos no va admitir valores negativos, nunca podremos implementar el move asin
		//void move(const Logic::TLogicalPosition &pos);  
		
		void move(const float degrees, const float height, const char ring, const char base);

		bool intersects(CActor *otherActor, float &degrees, float &height);
		bool intersects(CActor *otherActor);

		// UNDONE FRS Ya tenemos el getLogicPos que hace exactamente lo mismo 
		//Logic::TLogicalPosition& getGlobalPose() {return _logicPosition;}

		
		/************************
			GETTER's & SETTER's
		************************/
		void setLogicPos(const Logic::TLogicalPosition &position) {_logicPosition=position;}
		Logic::TLogicalPosition &getLogicPos() {return _logicPosition;}

		void setBoxWidth(const float angularWidth) {_boxWidth=angularWidth;}
		float getBoxWidth() {return _boxWidth;}

		void setBoxHeight(const float height) {_boxHeight=height;}
		float getBoxHeight() {return _boxHeight;}

		void setIObserver(IObserver* component) {_component=component;}
		IObserver *getIObserver() {return _component;}

	protected:

		// CScene es la única que puede añadir o eliminar actores.

		//friend class CScene;

		//CScene *_scene;

		Logic::TLogicalPosition _logicPosition;

		float _boxWidth;
		float _boxHeight;

		IObserver* _component;


	}; // class CActor


	typedef std::vector<CActor*> TActorVector;

} // namespace Physics

#endif 
