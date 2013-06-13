/**
@file Attack.cpp

Contiene la declaración del componente que controla el ataque 
angular de entidades.

@see Logic::CBasicAI
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/

#include "BasicAI.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"
#include "Application/BaseApplication.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageInt.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Components/AltarStateSwitcher.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "Logic/Entity/Messages/MessageUShort.h"

//declaración de la clase

/*para tener un acceso directo al gamestatus*/
#include "Logic/GameStatus.h"
#include "Logic/RingInfo.h"
#include "Logic/BaseInfo.h"
#include "Logic/PlayerInfo.h"

namespace Logic 
{
	IMP_FACTORY(CBasicAI);

	bool CBasicAI::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		//_gameStatus=Application::CBaseApplication::getSingletonPtr()->getGameState()->getGameStatus();
		_gameStatus=CGameStatus::getSingletonPtr();
		_reloj=Application::CBaseApplication::getSingletonPtr()->getClock();
	
		/*if(entityInfo->hasAttribute("attackPower"))
			_attackPower = entityInfo->getFloatAttribute("attackPower");
		if (_entity->getType()=="OtherPlayer")
			cover();*/
		return true;
	}

	void CBasicAI::timeArrived()
	{
		_waiting=false;
		int v1 = rand() % 2; 
		if (v1==0)
		{
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::WALK_RIGHT);
			_entity->emitMessage(m);
		}
		else
		{
			if (_entity->getLogicalPosition()->getSense()==Logic::LogicalPosition::LEFT )
			{	
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::WALK_RIGHT);
				_entity->emitMessage(m);
			}
			else
			{
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::WALK_LEFT);
				_entity->emitMessage(m);
			
			}
		}
	}

	bool CBasicAI::activate()
	{				
		Logic::CMessage *m = new Logic::CMessage();
		m->setType(Logic::Message::CONTROL);
		m->setAction(Logic::Message::WALK_RIGHT);
		_entity->emitMessage(m);
		return true;
	}		

	void CBasicAI::deactivate()
	{
	}
	
	bool CBasicAI::accept(const CMessage *message)
	{//aviso de que tanto accept como process son un poco hack, pero es es solo hasta tener un componente NPCCONTROLLER
			
		return (message->getType() == Message::TRIGGER && 
			(message->getAction() == Message::TRIGGER_ENTER||
			message->getAction() == Message::TRIGGER_EXIT) || message->getType() == Message::ALTAR_ACTIVATED ||  message->getAction() == Message::COVER) || (message->getType()==Message::ANIMATION_FINISHED 
			||message->getType()==Message::COLLISION);
	}
		
	 void CBasicAI::process(CMessage *message)
		 {
		switch(message->getType())
		{
			case Message::TRIGGER:
				{		
					if (!_agresivo)
					if(message->getAction() == Message::TRIGGER_ENTER)
					{
						Logic::CMessage *m = new Logic::CMessage();
						m->setType(Logic::Message::CONTROL);
						m->setAction(Logic::Message::WALK_STOP);
						_entity->emitMessage(m);

						Logic::CMessageUInt *m2 = new Logic::CMessageUInt();
						m2->setType(Logic::Message::CONTROL);
						m2->setAction(Logic::Message::SWITCH_ALTAR);
						m2->setUInt(_entity->getEntityID());
						_entity->emitMessage(m2);
						
					}
					break;
				}
				case Message::ALTAR_ACTIVATED:
				{	//este if es para saber si yo soy el que estaba activando el altar que se ha activado
					if (!_agresivo)
					if(_entity->getComponent<CAltarStateSwitcher>()->getTarget() == _entity->getMap()->getEntityByName(static_cast<CMessageString*>(message)->getString()))
					{
						int accion=rand()%3;
						if (accion)
						{	
							Logic::CMessage *m = new Logic::CMessage();
							m->setType(Logic::Message::CONTROL);
							m->setAction(Logic::Message::WALK_RIGHT);
							_entity->emitMessage(m);
						}
						else
						{
							if (accion)
							{
								Logic::CMessage *m = new Logic::CMessage();
								m->setType(Logic::Message::CONTROL);
								m->setAction(Logic::Message::WALK_LEFT);
								_entity->emitMessage(m);
							}
							else{
								Logic::CMessage *m = new Logic::CMessage();
								m->setType(Logic::Message::CONTROL);
								m->setAction(Logic::Message::LIGHT_ATTACK);
								_entity->emitMessage(m);
							}
							}
						break;					
					}
				}
				case Message::COLLISION:
				{
					int accion=rand()%2;
					if (accion==0)
					{	
						Logic::CMessage *m = new Logic::CMessage();
						m->setType(Logic::Message::CONTROL);
						m->setAction(Logic::Message::LIGHT_ATTACK);
						_entity->emitMessage(m);
					}
					else
					{
						Logic::CMessage *m4 = new Logic::CMessage();
						m4->setType(Logic::Message::CONTROL);
						m4->setAction(Logic::Message::WALK_STOP);
						_entity->emitMessage(m4);
						
					}
					break;
				}
		}
	 }
	
	void CBasicAI::tick(unsigned int msecs)
	{	//Esta primera comprobación es para saber si debo de ser agresivo
		if (_gameStatus->getPlayer(_entity->getOriginBase())!=NULL)
		if (_gameStatus->getPlayer(_entity->getOriginBase())->inMyBase())
			_agresivo=true;
	/*	else
			_agresivo=false;*/
		
		IComponent::tick(msecs);
		// (_entity->getName()!="locoCubriendose")
		if (CServer::getSingletonPtr()->getPlayer()->getLogicalPosition()->getBase()==_entity->getLogicalPosition()->getBase() || _entity->getType()!="OtherPlayer")
			if (CServer::getSingletonPtr()->getPlayer()->getLogicalPosition()->getRing()==_entity->getLogicalPosition()->getRing() ||  _entity->getType()!="OtherPlayer")
			{
				if (_waiting==false)
				{
					_reloj->addTimeObserver(this, 2500);		
					_waiting=true;
				}
				//en el mismo anillo
				//ser agresivo
				if (!(_entity->getComponent<CAvatarController>()->isWalkingLeft() || _entity->getComponent<CAvatarController>()->isWalkingRight() ))
				{
				/*	Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::CONTROL);
					m->setAction(Logic::Message::WALK_RIGHT);
					_entity->emitMessage(m);*/
				}
			}
			else
			{//en diferente anillo.
				//cambiemos de anillo.
				Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
				m->setAction(Logic::Message::WALK_STOP);
				_entity->emitMessage(m);
					

				Logic::CMessage *m2 = new Logic::CMessage();
				m2->setType(Logic::Message::CONTROL);
				if (_entity->getLogicalPosition()->getRing()<CServer::getSingletonPtr()->getPlayer()->getLogicalPosition()->getRing())	
					m2->setAction(Logic::Message::GO_UP);
				else
					m2->setAction(Logic::Message::GO_DOWN);
				_entity->emitMessage(m2);
			}
		else
		{//en diferente base.						
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::WALK_STOP);
			_entity->emitMessage(m);
					
			Logic::CMessageUShort *m3= new Logic::CMessageUShort();
			m3->setType(Logic::Message::CONTROL);
			m3->setAction(Logic::Message::CHANGE_BASE);
			m3->setUShort(CServer::getSingletonPtr()->getPlayer()->getLogicalPosition()->getBase());
			_entity->emitMessage(m3);
		}
			//activar sus altares o buscarle, cambiar de anillo.
					
	} //fin de CBasicAI:tick


} // namespace Logic