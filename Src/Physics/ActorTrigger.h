//---------------------------------------------------------------------------
// ActorTrigger.h
//---------------------------------------------------------------------------

/**
@file ActorTrigger.h

Contiene la declaración de la clase que representa un trigger físico.

@see Physics::CActorTrigger

@author FRS
@date Febrero, 2013
*/

#ifndef __Physics_ActorTrigger_H
#define __Physics_ActorTrigger_H

#include "Physics/Actor.h"



namespace Physics
{
	

	class CActorTrigger : public CActor
	{
	public:

		typedef std::vector<CActor*>	TActors;

		CActorTrigger(const Logic::TLogicalPosition &position, 
					const float angularWidth, const float height, IObserver *component) 
					: CActor(position, angularWidth, height, component) {};
		
		// TODO FRS necesario reimpl para que el delete se ejecute sobre este this y no del padre?
		// void release();

		bool enters(CActor*); // TODO FRS no sé si me molan estos nombres de métodos... xD
		bool exits(CActor*);
		
		
	private:

		TActors _actorsOnTrigger;

		bool isOnTrigger(CActor* actor);

	}; // class CActorTrigger

	//typedef std::vector<CActorTrigger*> TTriggerVector;

} // namespace Physics

#endif 