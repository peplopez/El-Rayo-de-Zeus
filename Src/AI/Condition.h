/**
@file CCondition.h

En este fichero se define la interfaz para las condiciones
de las aristas de las máquinas de estado, así como cuatro 
condiciones de ejemplo: CConditionTrue (una condición que 
siempre se evalúa como verdadera), CConditionFinished (es 
verdadera si la acción asociada al nodo ha terminado), 
CConditionSuccess (es  verdadera si la acción asociada al 
nodo ha terminado con éxito) y CConditionFail (es verdadera 
si la acción asociada al nodo ha terminado con fallo).

@author Gonzalo Flórez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_Condition_H
#define __AI_Condition_H

#include "Logic/Entity/Entity.h"

#include "LatentAction.h"

using namespace Logic;

namespace AI 
{
	/**
	Interfaz que deberán implementar las condiciones de 
	las máquinas de estado.
	*/
	template <class TNode>
	class ICondition
	{
	public:
		/**
		En este método se comprobará la condición. Recibe el 
		nodo al que pertenece la condición (de esta manera 
		podemos comprobar si el nodo ha terminado y en qué 
		condiciones) y la entidad que ejecuta el comportamiento 
		(que puede ser NULL).

		@param currentNode Nodo al que pertenece la condición.
		@param entity Entidad que ejecuta el comportamiento
		@return true o false según si se cumple la condición o no.
		*/
		virtual bool check(TNode* currentNode, CEntity* entity) = 0;
	}; // class CCondition

	/**
	Esta clase define una condición que siempre se evalúa a true.
	*/
	template <class TNode>
	class CConditionTrue : public ICondition<TNode>
	{
	public:
		bool check(TNode* currentNode, CEntity* entity) { return true; }
	};
	
	/**
	Esta clase define una condición que se evalúa a true si la 
	acción a la que pertenece ha terminado.
	*/
	class CConditionFinished : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			// TODO PRÁCTICA IA
			// Comprobar si el estado del nodo que se está ejecutando es terminado

			CLatentAction::LAStatus status = currentNode->getStatus();
			return status == CLatentAction::SUCCESS || status == CLatentAction::FAIL;
		}
	};

	/**
	Esta clase define una condición que se evalúa a true si la 
	acción a la que pertenece ha terminado con éxito.
	*/
	class CConditionSuccess : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			// TODO PRÁCTICA IA
			// Comprobar si el estado del nodo que se está ejecutando es éxito

			CLatentAction::LAStatus status = currentNode->getStatus();
			return status == CLatentAction::SUCCESS;
		}
	};

	/**
	Esta clase define una condición que se evalúa a true si la 
	acción a la que pertenece ha terminado con fallo.
	*/
	class CConditionFail : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			
			// TODO PRÁCTICA IA
			// Comprobar si el estado del nodo que se está ejecutando es fallo
			
			CLatentAction::LAStatus status = currentNode->getStatus();
			return status == CLatentAction::FAIL;
		}
	};

} // namespace AI 

#endif // __AI_Condition_H
