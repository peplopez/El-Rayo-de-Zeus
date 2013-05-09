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

		typedef std::set<CActor*> TActors;

		CActorTrigger(Logic::CLogicalPosition *position, 
					const float angularWidth, const float height, const float widthOffset, const float heightOffset,  IObserver *component) 
					: CActor(position, angularWidth, height, widthOffset, heightOffset, component) {};
		
		// TODO FRS necesario reimpl para que el delete se ejecute sobre este this y no del padre?
		// void release();

		bool enters(CActor*); // TODO FRS no sé si me molan estos nombres de métodos... xD
		bool exits(CActor*);
		bool isOnTrigger(CActor* actor) { return _actorsOnTrigger.count(actor); }
		
	private:

		TActors _actorsOnTrigger;	

	}; // class CActorTrigger

	//typedef std::vector<CActorTrigger*> TTriggerVector;

} // namespace Physics

#endif 