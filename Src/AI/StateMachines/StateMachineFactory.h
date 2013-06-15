/**
@file __AI_SM_Factory.h

En este fichero se define la clase CSM_Cancerbero,
que es una maquina de estados para la criatura
Cancerbero.

@author Jose Luis López Sánchez
@date Junio 2013

*/

#include "Logic/Entity/Entity.h"
#include "Condition.h"
#include "StateMachine.h"
#include "../LatentActions/LatentAction.h"
#include "SM_Hercules.h"
#include "SM_Cancerbero.h"
#include "SM_Sinbad.h"

#pragma once

#ifndef __AI_SM_Factory
#define __AI_SM_Factory

using namespace Logic;


namespace AI
{

/**
	Factoría que devuelve máquinas de estado predefinidas.
	Sólo reconoce el valor "wander" como entrada, 
	que recorre puntos aleatorios del mapa
	*/
	class CStateMachineFactory 
	{
	public:
		static CStateMachine<CLatentAction>* getStateMachine(std::string smName, CEntity * entity)
		{
			if (smName == "Hercules") 
				return new CSM_Hercules(entity);
			if (smName == "Sinbad") 
				return new CSM_Sinbad(entity);
			if (smName == "Cancerbero") 
				return new CSM_Cancerbero(entity);
			
			return 0;
		}
	};
}
#endif // __AI_SM_Factory