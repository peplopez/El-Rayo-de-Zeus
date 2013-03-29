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
//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CBasicAI);

	bool CBasicAI::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_reloj=Application::CBaseApplication::getSingletonPtr()->getClock();
	
		/*if(entityInfo->hasAttribute("attackPower"))
			_attackPower = entityInfo->getFloatAttribute("attackPower");
		if (_entity->getType()=="OtherPlayer")
			cover();*/
		return true;
	}

	void CBasicAI::timeArrived()
	{
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
			||message->getType()==Message::ANIMATION_MOMENT
			||message->getType()==Message::COLLISION);
	}
		
	 void CBasicAI::process(CMessage *message)
		 {
		switch(message->getType())
		{
			case Message::TRIGGER:
				{					
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
						
					}break;
				}
				case Message::ALTAR_ACTIVATED:
				{
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
				//	if(_entity->getComponent<CAltarStateSwitcher>()->getTarget() == _entity->getMap()->getEntityByName(static_cast<CMessageString*>(message)->getString()))
					//{
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
						_reloj->addTimeObserver(2,this,2500);		
						break;
						}

					//}
				}
		}
	 }
	
	void CBasicAI::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);
			if (_entity->getName()!="locoCubriendose")
				if (CServer::getSingletonPtr()->getPlayer()->getLogicalPosition()->getBase()==_entity->getLogicalPosition()->getBase())
					if (CServer::getSingletonPtr()->getPlayer()->getLogicalPosition()->getRing()==_entity->getLogicalPosition()->getRing())
					{
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
					{
				
					}
				//activar sus altares o buscarle, cambiar de anillo.
					
	} //fin de CBasicAI:tick


} // namespace Logic