/**
@file SM_Hercules.h

En este fichero se define la clase CSM_Hercules,
que es una maquina de estados para un personaje provisional en el juego

@author Jose Luis López Sánchez
@date Junio 2013

*/


#pragma once

#ifndef __AI_SM_Hercules_H
#define __AI_SM_Hercules_H


#include "Logic/Entity/Entity.h"
#include "Condition.h"
#include "StateMachine.h"
#include "../LatentActions/LatentAction.h"

using namespace Logic;
namespace AI
{

class CSM_Hercules : public CStateMachine<CLatentAction>
	{
	public:
		/**
		Constructor. Añade los nodos y las aristas, establece el nodo de inicio
		y deja la máquina de estado lista para ser ejecutada.
		*/
		CSM_Hercules(CEntity* entity) : CStateMachine(entity) {

			int idle = this->addNode(new CLAIdle(entity)); // Aunque hagamos new, la FSM los delete por dentro
			int l_attack0=this->addNode(new CLA_Attack(entity,0,Message::LIGHT_ATTACK));
			int l_attack1=this->addNode(new CLA_Attack(entity,1,Message::LIGHT_ATTACK));
			int h_attack2=this->addNode(new CLA_Attack(entity,2,Message::HEAVY_ATTACK));

			int h_attack0=this->addNode(new CLA_Attack(entity,0,Message::HEAVY_ATTACK));
			int h_attack1=this->addNode(new CLA_Attack(entity,1,Message::HEAVY_ATTACK));
			int l_attack2=this->addNode(new CLA_Attack(entity,2,Message::LIGHT_ATTACK));

			int l_run=this->addNode(new CLA_Run(entity,Logic::LogicalPosition::LEFT));			
			int r_run=this->addNode(new CLA_Run(entity,Logic::LogicalPosition::RIGHT));
			int jumping=this->addNode(new CLA_Jump(entity));

			int covering=this->addNode(new CLA_Cover(entity));
			int changingBase=this->addNode(new CLA_ChangeBase(entity));			
			int changingRingUp=this->addNode(new CLA_ChangeRing(entity,Message::GO_UP));			
			int changingRingDown=this->addNode(new CLA_ChangeRing(entity,Message::GO_DOWN));

			int switchingAltar=this->addNode(new CLA_AltarSwitch(entity));


			int h_attack2Fatality=this->addNode(new CLA_Attack(entity,2,Message::HEAVY_ATTACK));

			int damaged=this->addNode(new CLA_Beaten(entity));
			//PT
			//int dead=this->addNode(new CLA_Death(entity));
			int dead=this->addNode(new CLA_Death(entity,5)); //5 segundos

			this->addEdge(idle, l_run, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_LEFT));
			this->addEdge(idle, r_run, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_RIGHT));

			this->addEdge(idle, l_run, new CConditionFlagWalkingActivated(Logic::LogicalPosition::LEFT));
			this->addEdge(idle, r_run, new CConditionFlagWalkingActivated(Logic::LogicalPosition::RIGHT));

			this->addEdge(idle, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			this->addEdge(l_run, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			this->addEdge(r_run, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			
			this->addEdge(jumping, idle, new CConditionFlagJumpingActivated());
			
			this->addEdge(idle, switchingAltar, new CConditionMessageAction<CLatentAction>(Message::ALTAR_MS_ORDER,Message::SWITCH_ALTAR));
			this->addEdge(l_run, switchingAltar, new CConditionMessageAction<CLatentAction>(Message::ALTAR_MS_ORDER,Message::SWITCH_ALTAR));
			this->addEdge(r_run, switchingAltar, new CConditionMessageAction<CLatentAction>(Message::ALTAR_MS_ORDER,Message::SWITCH_ALTAR));
			this->addEdge(switchingAltar, idle, new CConditionMessageAction<CLatentAction>(Message::ALTAR_MS_ORDER,Message::STOP_SWITCH));
			this->addEdge(switchingAltar, idle, new CConditionSuccess());


			this->addEdge(l_run, idle, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_STOP));
			this->addEdge(r_run, idle, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_STOP));
			
			this->addEdge(l_run, l_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(r_run, l_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(l_run, h_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::HEAVY_ATTACK));
			this->addEdge(r_run, h_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::HEAVY_ATTACK));

			//COMBO 1
			this->addEdge(idle, l_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(l_attack0, l_attack1,new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK,false,Message::COMBO_MOMENT));
			this->addEdge(l_attack1, h_attack2, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::HEAVY_ATTACK,false,Message::COMBO_MOMENT));
			this->addEdge(l_attack0, idle, new CConditionFail());
			this->addEdge(l_attack1, idle, new CConditionFail());
			this->addEdge(h_attack2, idle, new CConditionFail());

			//COMBO 2
			this->addEdge(h_attack1, l_attack2, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK,false,Message::COMBO_MOMENT));
			this->addEdge(l_attack2, idle, new CConditionFail());
			
			//COMBO 3
			this->addEdge(idle, h_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::HEAVY_ATTACK));
			this->addEdge(h_attack0, idle, new CConditionFail());
			this->addEdge(h_attack0, h_attack1, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::HEAVY_ATTACK,false,Message::COMBO_MOMENT));
			this->addEdge(h_attack1, h_attack2Fatality, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::HEAVY_ATTACK,false,Message::COMBO_MOMENT));
			this->addEdge(h_attack1, idle, new CConditionFail());
			this->addEdge(h_attack2Fatality, idle, new CConditionFail());
			
			//solo hago posible viajar desde idle
			this->addEdge(idle, changingBase, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::CHANGE_BASE));
			this->addEdge(idle, changingRingDown, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_DOWN));
			this->addEdge(idle, changingRingUp, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_UP));
			this->addEdge(changingBase, idle, new CConditionFinished());
			this->addEdge(changingRingUp, idle, new CConditionSuccess);
			this->addEdge(changingRingDown, idle, new CConditionSuccess);

			this->addEdge(idle, covering, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::COVER));
			this->addEdge(l_run, covering, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::COVER));			
			this->addEdge(r_run, covering, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::COVER));			
			this->addEdge(covering,idle, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::NO_COVER));
			
			this->addEdge(idle, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
			this->addEdge(l_run, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
			this->addEdge(r_run, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
			this->addEdge(jumping, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
						
			this->addEdge(damaged, idle,  new CConditionSuccess());  // es un exito que no me hayan dado de nuevo mientras me estaban dando. Vuelvo a Idle, estado desde el cual me puedo cubrir o contraatacar
			this->addEdge(damaged, damaged,  new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));  //si mientras estoy en estado de daño me vuelven a dar vuelvo a poner la animación de daño, vuelvo al principio del estado en el que estoy
			this->addEdge(damaged, dead,  new CConditionMessageAction<CLatentAction>(Message::DEAD,Message::DAMAGE));  //si mientras estoy en estado de daño me vuelven a dar vuelvo a poner la animación de daño, vuelvo al principio del estado en el que estoy
			this->addEdge(dead, idle,  new CConditionSuccess());

			// Por último hay que decir cuál es el nodo inicial.
			this->setInitialNode(idle);
		}
	};
}
#endif // __AI_SM_Hercules_H
