/**
@file CStateMachine.h

En este fichero se define la clase CStateMachine,
que es la clase padre de las máquinas de estado,
CStateMachineFactory, factoría de máquinas de estado
para las prácticas, y CSMWander, una máquina de estado
de ejemplo.

@author Gonzalo Flórez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_StateMachine_H
#define __AI_StateMachine_H

#include "Logic/Entity/Entity.h"
#include "Condition.h"

#include "../LatentActions/LatentAction.h"
//#include "../LatentActions/SimpleLatentActions.h"

#include "../LatentActions/LAIdle.h"
#include "../LatentActions/LA_Attack.h"
#include "../LatentActions/LA_Run.h"
#include "../LatentActions/LA_Jump.h"
#include "../LatentActions/LA_ChangeBase.h"
#include "../LatentActions/LA_ChangeRing.h"
#include "../LatentActions/LA_Cover.h"
#include "../LatentActions/LA_Beaten.h"
#include "../LatentActions/LA_Death.h"
#include "../LatentActions/LA_AltarSwitch.h"


#include "Logic/Entity/Messages/Message.h"


using namespace Logic;

namespace AI
{

	/**
	Clase padre para las máquinas de estado.
	<p>
	Es una clase parametrizada. El parámetro es la clase de 
	los elementos en los nodos. En general, este parámetro será
	una acción ejecutable (CLatentAction).
	*/
	template <class TNode>
	class CStateMachine
	{
	public: 
		/**
		Constructor
		*/
		CStateMachine() : _entity(0), _currentNodeId(-1), _initialNodeId(-1) { _edges = new EdgeList(); };
		/**
		Constructor que recibe la entidad que ejecuta la máquina de estado
		*/
		CStateMachine(CEntity* entity) : _entity(entity), _currentNodeId(-1), _initialNodeId(-1) { _edges = new EdgeList(); };
		/**
		Destructor
		*/
		~CStateMachine();
		/**
		Este método añade un nodo a la máquina de estado y devuelve un identificador
		del nodo. Este identificador se usa para referirse a los nodos al añadir
		aristas y establecer el nodo inicial.
		<p>
		Los nodos serán destruidos cuando se destruya la máquina de estado.

		@param content Contenido del nodo.
		@return Identificador para el nodo.
		*/
		int addNode(TNode *content);
		/**
		Añade una arista a la máquina de estado.
		<p>
		El método recibe los identificadores del nodo de origen y destino y una condición
		que indica cuándo se activa la transición de la arista.
		<p>
		Las condiciones serán destruidas cuando se destruya la máquina de estado.

		@param idOrig Identificador del nodo de origen.
		@param idDest Identificador del nodo de destino.
		@param cond Condición asociada a la arista.
		*/
		void addEdge(int idOrig, int idDest, ICondition<TNode> *cond);
		/**
		Este método comprueba las condiciones de las aristas que salen del 
		nodo actual y cambia de nodo si alguna de ellas es cierta. El método
		devuelve true si alguna transición es cierta (aunque no se cambie el
		nodo actual) y false en otro caso.
		<p>
		Las aristas se comprueban en el orden en que se han añadido y 
		la comprobación se detiene en el momento en que una de ellas se hace 
		cierta.
		*/
		bool update();
		/**
		Devuelve el contenido del nodo actual.
		*/
		TNode* getCurrentNode();
		/**
		Establece cuál es la entidad que ejecuta la máquina de estado.
		*/
		void setEntity(CEntity *entity) { _entity = entity; };

		/**
		Establece el nodo inicial.

		@param idNode Identificador del nodo inicial.
		*/
		void setInitialNode(int idNode) { _initialNodeId = idNode; };
		/**
		Devuelve el nodo inicial.

		@param idNode Identificador del nodo inicial.
		*/
		TNode* getInitialNode();
		
		/**
		Reinicia la ejecución de la máquina de estado.
		*/
		void resetExecution(){ _currentNodeId = -1; };


		virtual bool accept(const CMessage *message);
		
		virtual void process(CMessage *message);

		void reset();
	protected:
		/**
		Tipo que guarda la información de las aristas salientes de un nodo.
		Para cada arista tenemos un vector de pares con la condición y el 
		nodo de destino.
		*/
		typedef std::vector<std::pair<ICondition<TNode>*, int>> PairVector;
		/** 
		Tipo que guarda la información de todas las aristas. Está indexado 
		por el identificador del nodo de origen.
		*/
		typedef std::tr1::unordered_map<int, PairVector*> EdgeList;

		/**
		Entidad que ejecuta la máquina de estado.
		*/
		Logic::CEntity *_entity;
		/**
		Valores del nodo actual e inicial
		*/
		int _currentNodeId, _initialNodeId;
		/**
		Lista de nodos. Es un map que relaciona cada identificador de nodo con su contenido.
		*/
		std::tr1::unordered_map<int, TNode*> _nodes;
		/**
		Lista de aristas. Es un map que asocia cada nodo de origen de cada arista con una lista
		formada por pares (condición, nodo destino). Por ejemplo, si tenemos una aristas que sale
		del nodo con id 1 y va hasta el 2 y su condición es c1 y otra que va desde 1 hasta 3 y su 
		condición es c2, la estructura que tendremos será:
			{ 1 -> [ (c1, 2), 
					 (c2, 3) ] }
		*/
		EdgeList * _edges;

	}; // class CStateMachine
	
	class CSHercules : public CStateMachine<CLatentAction>
	{
	public:
		/**
		Constructor. Añade los nodos y las aristas, establece el nodo de inicio
		y deja la máquina de estado lista para ser ejecutada.
		*/
		CSHercules(CEntity* entity) : CStateMachine(entity) {

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

	class CSSinbad : public CStateMachine<CLatentAction>
	{
	public:
		//std::string animNames[] =
		//{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		//"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};


		/**
		Constructor. Añade los nodos y las aristas, establece el nodo de inicio
		y deja la máquina de estado lista para ser ejecutada.
		*/
		CSSinbad(CEntity* entity) : CStateMachine(entity) {

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
class CSCancerbero : public CStateMachine<CLatentAction>
	{
	public:
		//std::string animNames[] =
		//{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		//"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};


		/**
		Constructor. Añade los nodos y las aristas, establece el nodo de inicio
		y deja la máquina de estado lista para ser ejecutada.
		*/
		CSCancerbero(CEntity* entity) : CStateMachine(entity) {

			int idle = this->addNode(new CLAIdle(entity)); // Aunque hagamos new, la FSM los delete por dentro
			int l_attack0=this->addNode(new CLA_Attack(entity,0,Message::LIGHT_ATTACK));
		//	int l_walk=this->addNode(new CLA_Walk(entity,Logic::LogicalPosition::LEFT));			
			//int r_walk=this->addNode(new CLA_Walk(entity,Logic::LogicalPosition::RIGHT));

			int l_run=this->addNode(new CLA_Run(entity,Logic::LogicalPosition::LEFT));			
			int r_run=this->addNode(new CLA_Run(entity,Logic::LogicalPosition::RIGHT));

			int jumping=this->addNode(new CLA_Jump(entity));
			int changingRingUp=this->addNode(new CLA_ChangeRing(entity,Message::GO_UP));			
			int changingRingDown=this->addNode(new CLA_ChangeRing(entity,Message::GO_DOWN));
			int damaged=this->addNode(new CLA_Beaten(entity));
			int dead=this->addNode(new CLA_Death(entity,5000)); //1000 es 1 segundo. 10000 son 10 segundos.


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
			this->addEdge(l_run, l_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(r_run, l_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(idle, l_attack0, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::LIGHT_ATTACK));
			this->addEdge(l_attack0, idle, new CConditionFail());
			this->addEdge(idle, changingRingDown, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_DOWN));
			this->addEdge(idle, changingRingUp, new CConditionMessageAction<CLatentAction>(Message::CONTROL,Message::GO_UP));
			this->addEdge(changingRingUp, idle, new CConditionSuccess);
			this->addEdge(changingRingDown, idle, new CConditionSuccess);
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
				return new CSHercules(entity);
			if (smName == "Sinbad") 
				return new CSSinbad(entity);
			if (smName == "Cancerbero") 
				return new CSCancerbero(entity);
			
			return 0;
		}
	};

	
//////////////////////////////
//	Implementación de CStateMachine
//////////////////////////////
	template <class TNode>
	CStateMachine<TNode>::~CStateMachine() 
	{
		// Borramos las aristas
		for (EdgeList::iterator it = _edges->begin(); it != _edges->end(); it++)
		{
			PairVector* v = it->second;
			for (PairVector::iterator it2 = v->begin(); it2 != v->end(); it2++) {
				// Borra la condición
				delete it2->first;
			}
			// Borra el vector
			delete v;
		}
		delete _edges;

		// Borramos los nodos
		for (std::tr1::unordered_map<int, TNode*>::iterator it = _nodes.begin(); it != _nodes.end(); it++)
		{
			delete it->second;
		}
	}
//////////////////////////////
	template <class TNode>
	int CStateMachine<TNode>::addNode(TNode* content)
	{
		int id = _nodes.size();
		_nodes[id] = content;
		return id;
	} // addNode
//////////////////////////////
	template <class TNode>
	void CStateMachine<TNode>::addEdge(int idOrig, int idDest, ICondition<TNode> *cond)
	{
		// Buscamos la lista de aristas salientes para el nodo de origen.
		EdgeList::iterator it = _edges->find(idOrig);
		PairVector* vector = 0;
		// Si no hay ninguna arista saliente para ese nodo creamos una nueva lista
		if (it == _edges->end()) {
			vector = new PairVector();
			(*_edges)[idOrig] = vector;
		} else {
			vector = (*it).second;
		}
		// Añadimos el par (condición, nodo de destino)
		vector->push_back(std::pair<ICondition<TNode>*, int>(cond, idDest));

	} // addEdge
//////////////////////////////
	template <class TNode>
	bool CStateMachine<TNode>::update()
	{
		// Si es el primer update devolvemos que sí hay un cambio de nodo)
		if (_currentNodeId == -1) {
			_currentNodeId = _initialNodeId;
			return true;
		}
		// Buscamos la lista de aristas que salen del nodo actual
		EdgeList::iterator it = _edges->find(_currentNodeId);
		if (it != _edges->end()) {
			// Sacamos el nodo actual por si hay que comprobar la condición
			TNode* node = _nodes[_currentNodeId];
			PairVector* vector = (*it).second;
			// Para cada elemento del vector (arista que sale del nodo actual)
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
				// Si la condición es verdadera
				if (edgeIt->first->check(node, _entity)) {
					// Sólo hacemos la primera transición que encontramos
					int newNode = edgeIt->second;
					_currentNodeId = newNode;
					// Si la transición se hace cierta siempre consideramos que el comportamiento cambia
					// Esto implica que si se activa una arista circular (empieza y termina en el mismo nodo)
					// el comportamiento asociado se va a reiniciar
					return true;
				}
			}
		}
		return false;
	} // update
	template <class TNode>
	void CStateMachine<TNode>::reset()
	{
		// Buscamos la lista de aristas que salen del nodo actual
		EdgeList::iterator it = _edges->find(_currentNodeId);
		if (it != _edges->end()) {
			// Sacamos el nodo actual por si hay que comprobar la condición
			TNode* node = _nodes[_currentNodeId];
			PairVector* vector = (*it).second;
			// Para cada elemento del vector (arista que sale del nodo actual)
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
				// Si la condición es verdadera
				edgeIt->first->reset();					
				}
			}				
	} // reset
//////////////////////////////
	template <class TNode>
	TNode* CStateMachine<TNode>::getCurrentNode()
	{
		return _nodes[_currentNodeId];
	} // getCurrentNode
//////////////////////////////
	template <class TNode>
	TNode* CStateMachine<TNode>::getInitialNode()
	{
		return _nodes[_initialNodeId];
	} // getCurrentNode

	template <class TNode>
	bool CStateMachine<TNode>::accept(const CMessage *message)
	{
		// Si no hay un nodo actual no hay aristas interesadas
		if (_currentNodeId == -1) 
			return false;
		// Buscamos la lista de aristas que salen del nodo actual
		EdgeList::iterator it = _edges->find(_currentNodeId);
		if (it != _edges->end()) {
			PairVector* vector = (*it).second;
			// Para cada elemento del vector (arista que sale del nodo actual)
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
				// Llamamos al accept de la condición
				if (edgeIt->first->accept(message))
					return true;
			}
		}
		return false;
	}
//////////////////////////////
	template <class TNode>
	void CStateMachine<TNode>::process(CMessage *message){
		// Si no hay un nodo actual no hay aristas interesadas así que lo primero es comprobar si hay un nodo válido en _currentNodeId
		if (_currentNodeId != -1) { 
			// Buscamos la lista de aristas que salen del nodo actual
			EdgeList::iterator it = _edges->find(_currentNodeId);
			if (it != _edges->end()) {
				PairVector* vector = (*it).second;
				// Para cada elemento del vector (arista que sale del nodo actual)
				for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
					// Llamamos al process de la condición
					if (edgeIt->first->accept(message))
						edgeIt->first->process(message);
				}
			}
		}
	}
//////////////////////////////


} // namespace AI 

#endif // __AI_StateMachine_H
