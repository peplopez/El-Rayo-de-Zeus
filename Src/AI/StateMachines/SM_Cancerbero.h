/**
@file SM_Cancerbero.h

En este fichero se define la clase CSM_Cancerbero,
que es una maquina de estados para la criatura
Cancerbero.

@author Jose Luis López Sánchez
@date Junio 2013

*/



#pragma once

#ifndef __AI_SM_Cancerbero_H
#define __AI_SM_Cancerbero_H

#include "Logic/Entity/Entity.h"
#include "Condition.h"
#include "StateMachine.h"
#include "../LatentActions/LatentAction.h"

using namespace Logic;


namespace AI
{

class CSM_Cancerbero : public CStateMachine<CLatentAction>
	{
	public:
		//std::string animNames[] =
		//{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		//"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};


		/**
		Constructor. Añade los nodos y las aristas, establece el nodo de inicio
		y deja la máquina de estado lista para ser ejecutada.
		*/
		CSM_Cancerbero(CEntity* entity) : CStateMachine(entity) {

			//int idle = this->addNode(new CLAIdle(entity)); // Aunque hagamos new, la FSM los delete por dentro
			int attack=this->addNode(new CLA_AttackCreature(entity,0,Message::LIGHT_ATTACK));
			int l_walk=this->addNode(new CLA_Walk(entity,Logic::LogicalPosition::LEFT));			
			int r_walk=this->addNode(new CLA_Walk(entity,Logic::LogicalPosition::RIGHT));

			int l_run=this->addNode(new CLA_Run(entity,Logic::LogicalPosition::LEFT));			
			int r_run=this->addNode(new CLA_Run(entity,Logic::LogicalPosition::RIGHT));

			int alert=this->addNode(new CLA_Alert(entity));
			int jumping=this->addNode(new CLA_Jump(entity));
			int changingRingUp=this->addNode(new CLA_ChangeRing(entity,Message::GO_UP));			
			int changingRingDown=this->addNode(new CLA_ChangeRing(entity,Message::GO_DOWN));
			int damaged=this->addNode(new CLA_Beaten(entity));
			int dead=this->addNode(new CLA_Death(entity,5000)); //1000 es 1 segundo. 10000 son 10 segundos.

			//al estado alert se entra por el módulo de percepción.
			this->addEdge(l_walk, alert, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_LEFT)); //control- enemy_detected
			this->addEdge(r_walk, alert, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_LEFT)); //control- enemy_detected
			
			this->addEdge(alert, r_run, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_RIGHT));
			this->addEdge(alert, l_run, new CConditionFlagWalkingActivated(Logic::LogicalPosition::LEFT));
			
			//creo que es innecesario, ya veremos....
			this->addEdge(alert, r_run, new CConditionFlagWalkingActivated(Logic::LogicalPosition::RIGHT));
			this->addEdge(alert, l_run, new CConditionFlagWalkingActivated(Logic::LogicalPosition::LEFT));

			this->addEdge(l_walk, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			this->addEdge(r_walk, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			this->addEdge(l_run, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			this->addEdge(r_run, jumping, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::JUMP));
			
			this->addEdge(l_walk, changingRingDown, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_DOWN));
			this->addEdge(r_walk, changingRingUp, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_UP));
			this->addEdge(l_run, changingRingDown, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_DOWN));
			this->addEdge(r_run, changingRingUp, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_UP));
		
			this->addEdge(r_walk, changingRingDown, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_DOWN));
			this->addEdge(l_walk, changingRingUp, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_UP));
			this->addEdge(r_run, changingRingDown, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_DOWN));
			this->addEdge(l_run, changingRingUp, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_UP));
			//está forzado a l_walk, de momento, antes solo se podia pasar a idle y era más facil, pero es lo que hay. El perro no tiene idle.
			this->addEdge(changingRingUp, l_walk, new CConditionFinished);
			this->addEdge(changingRingDown, l_walk, new CConditionFinished);

			//creo que es innecesario, ya veremos			
			//this->addEdge(jumping, idle, new CConditionFlagJumpingActivated());

			//será la percepción quien controle este cambio de estado			
			this->addEdge(l_run, l_walk, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_STOP)); //definir el mensaje que proceda.
			this->addEdge(r_run, r_walk, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_STOP));

			//this->addEdge(r_run, idle, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::WALK_STOP));
			this->addEdge(l_run, attack, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(r_run, attack, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(attack, alert, new CConditionFinished());
			
			//no permitido este cambio de momento.
			//this->addEdge(idle, changingRingDown, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_DOWN));
			//this->addEdge(idle, changingRingUp, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_UP));

			this->addEdge(l_walk, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
			this->addEdge(r_walk, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
			this->addEdge(l_run, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
			this->addEdge(r_run, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
			this->addEdge(alert, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));


			this->addEdge(jumping, damaged, new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));
			this->addEdge(damaged, l_walk,  new CConditionFinished());  // es un exito que no me hayan dado de nuevo mientras me estaban dando. Vuelvo a Idle, estado desde el cual me puedo cubrir o contraatacar
			this->addEdge(damaged, damaged,  new CConditionMessageAction<CLatentAction>(Message::LIFE_MODIFIER,Message::DAMAGE));  //si mientras estoy en estado de daño me vuelven a dar vuelvo a poner la animación de daño, vuelvo al principio del estado en el que estoy
			this->addEdge(damaged, dead,  new CConditionMessageAction<CLatentAction>(Message::DEAD,Message::DAMAGE));  //si mientras estoy en estado de daño me vuelven a dar vuelvo a poner la animación de daño, vuelvo al principio del estado en el que estoy
			this->addEdge(dead, l_walk,  new CConditionSuccess());

			// Por último hay que decir cuál es el nodo inicial.
			this->setInitialNode(l_walk);	
		}
	};


}
#endif // __AI_SM_Cancerbero_H