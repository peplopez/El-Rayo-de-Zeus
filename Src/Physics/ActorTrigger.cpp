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

	void CActorTrigger::exits(CActor* actor){

		std::vector<CActor*>::iterator position = std::find(_actorsOnTrigger.begin(), _actorsOnTrigger.end(), actor);
			if (position != _actorsOnTrigger.end())
			{
				(*position)->release();
				_actorsOnTrigger.erase(position);
			}		
	}



}