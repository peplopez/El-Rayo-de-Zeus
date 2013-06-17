/**
@file SM_Sinbad.h

En este fichero se define la clase CSM_Sinbad,
que es una maquina de estados para un personaje provisional en el juego

@author Jose Luis López Sánchez
@date Junio 2013

*/



#pragma once

#ifndef __AI_SM_Sinbad_H
#define __AI_SM_Sinbad_H

#include "Logic/Entity/Entity.h"
#include "Condition.h"
#include "StateMachine.h"
#include "../LatentActions/LatentAction.h"

using namespace Logic;


namespace AI
{

class CSM_Sinbad : public CStateMachine<CLatentAction>
	{
	public:
		//std::string animNames[] =
		//{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		//"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};


		/**
		Constructor. Añade los nodos y las aristas, establece el nodo de inicio
		y deja la máquina de estado lista para ser ejecutada.
		*/
		CSM_Sinbad(CEntity* entity) : CStateMachine(entity) {

			int idle = this->addNode(new CLAIdle(entity,"Dance")); // Aunque hagamos new, la FSM los delete por dentro
			int l_run=this->addNode(new CLA_Run(entity,Logic::LogicalPosition::LEFT));			
			int r_run=this->addNode(new CLA_Run(entity,Logic::LogicalPosition::RIGHT));
			int jumping=this->addNode(new CLA_Jump(entity));

			//COMBO 1
			//this->addEdge(idle, l_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(idle, l_run, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_LEFT));
			this->addEdge(idle, r_run, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_RIGHT));

			this->addEdge(idle, l_run, new CConditionFlagWalkingActivated(Logic::LogicalPosition::LEFT));
			this->addEdge(idle, r_run, new CConditionFlagWalkingActivated(Logic::LogicalPosition::RIGHT));

			this->addEdge(idle, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			this->addEdge(l_run, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			this->addEdge(r_run, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			
			this->addEdge(jumping, idle, new CConditionFlagJumpingActivated());
			

			this->addEdge(l_run, idle, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_STOP));
			this->addEdge(r_run, idle, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_STOP));
			
			// Por último hay que decir cuál es el nodo inicial.
			this->setInitialNode(idle);
		}
	};

}
#endif // __AI_SM_Sinbad_H
