//---------------------------------------------------------------------------
// ActorTrigger.cpp
//---------------------------------------------------------------------------

/**
@file ActorTrigger.cpp

@see Physics::CActorTrigger

@author FRS
@date Febrero, 2013
*/

#include "ActorTrigger.h"


namespace Physics
{
	bool CActorTrigger::enters(CActor* actor){
		
		if( isOnTrigger(actor) )
			return false;
		else {
			_actorsOnTrigger.insert(actor);
			return true;
		}
	}

	bool CActorTrigger::exits(CActor* actor){

			if (  isOnTrigger(actor) ) {
				_actorsOnTrigger.erase(actor);
				return true;
			} else
				return false;			
	}
}