/**
@file Attack.cpp

Contiene la declaración del componente que controla el ataque 
angular de entidades.

@see Logic::CAttack
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/

#include "Attack.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"
#include "Application/BaseApplication.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"



//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CAttack);

	bool CAttack::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("attackPower"))
			_attackPower = entityInfo->getFloatAttribute("attackPower");

		return true;
		}

	bool CAttack::activate()
	{				
		 _lightAttack=_heavyAttack=false;
		return true;
	}		

	void CAttack::deactivate(){}
	
	bool CAttack::accept(const CMessage *message)
	{//aviso de que tanto accept como process son un poco hack, pero es es solo hasta tener un componente NPCCONTROLLER
	//	return false;
		return (message->getType() == Message::CONTROL && 
			(message->getAction() == Message::LIGHT_ATTACK||
			message->getAction() == Message::HEAVY_ATTACK)) || (Message::ANIMATION_FINISHED);

	}
		
	 void CAttack::process(CMessage *message)
		 {
		switch(message->getType())
		{
		case Message::CONTROL:
			{			
				if(message->getAction() == Message::LIGHT_ATTACK)
					lightAttack();
				else if(message->getAction() == Message::HEAVY_ATTACK)
					 heavyAttack();
				break;
			}
		case Message::ANIMATION_FINISHED:
			{
				CMessageString* maux = static_cast<CMessageString*>(message);
				if (maux->getString().compare("FireKatana")==0)
				{
					
					_lightAttack=_heavyAttack=false;//stopMovement();
				}
			}
		
		case Message::ANIMATION_MOMENT:
			{
				//querré saber cual animación es, de momento se que solo puedo recibir un tipo de animación
				float punto;
				if (_entity->getSense()==Logic::LogicalPosition::RIGHT)
					punto=_entity->getDegree()-10;
				else
					punto=_entity->getDegree()+10;
					//con este metodo vemos si con la espada le estamos dando
					attackPlace(punto,_entity->getRing(),_entity->getBase(),false);
			}
		}
	 }

	void CAttack::lightAttack() 
	{
		//if (_lightAttack) return;
		_heavyAttack=false;
		_lightAttack=true;

	/*	CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setAction(Message::LIGHT_ATTACK);
		message->setString("FireAK47");
		message->setBool(false);
		_entity->emitMessage(message,this);
		*/
		//envio mensaje para averiguar sin con este ataque le he arreado a alguien o no
		/*CMessageFloat *message2 = new CMessageFloat();
		message2->setType(Message::SPACE_REQUEST);
		if (_entity->getSense()==Logic::LogicalPosition::RIGHT)
		message2->setFloat(_entity->getDegree()-10);
		else
		message2->setFloat(_entity->getDegree()+10);		
		_entity->emitMessage(message2,this);*/
		float punto;
		if (_entity->getSense()==Logic::LogicalPosition::RIGHT)
			punto=_entity->getDegree()-10;
		else
			punto=_entity->getDegree()+10;
		attackPlace(punto,_entity->getRing(),_entity->getBase(),true);
	} // turn

	void CAttack::heavyAttack() 
	{		
		if (_heavyAttack) return;
		_heavyAttack=true;
		_lightAttack=false;
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setString("FireKatana");
		message->setAction(Message::HEAVY_ATTACK);
		message->setBool(false);
		_entity->emitMessage(message,this);
	} // turn

	//este metodo devuelve null si no se está ocupando ese grado o la entidad que ocupa ese espacio
	bool CAttack::attackPlace(float grado, short ring, short base,bool soloInfo)
	{//acotar
		//averiguo el espacio que ocupo:

		
		CMap::TEntityList::const_iterator it = _entity->getMap()->getEntities().begin();
		CMap::TEntityList::const_iterator end = _entity->getMap()->getEntities().end();
		for(; it != end; ++it)
		{			
			//Si la entidad que comparo no soy yo mismo y la distancia entre las posiciones
			//de las dos es menor de la distancia de colisión (o radio)


			if(_entity != (*it) )
			{
				if (((*it)->getType().compare("Altar")!=0)&& ((*it)->getType().compare("World")!=0)&& ((*it)->getType().compare("SkyBox")!=0))
				{//lo que hay que  hacer es que no se itere sobre entidades que no tengan componente CCollider, de momento se hace esa comprobación

			
					if (base==(*it)->getBase() && ring==(*it)->getRing())	
					{								
						float limiteIzquierdo=(*it)->getDegree()-5;//-(*it)->getgetAngularBox();
						float limiteDerecho=(*it)->getDegree()+5;//+(*it)->getAngularBox();
						if (grado>limiteIzquierdo && grado<limiteDerecho )
						{
							if (!soloInfo)
							{
								CMessageString *m2 = new CMessageString();
								m2->setString("luminoso");
								m2->setType(Message::SET_SHADER);						
								(*it)->emitMessage(m2,this);

								Logic::CMessage *m = new Logic::CMessage();
								m->setType(Logic::Message::CONTROL);
								m->setAction(Logic::Message::WALK_STOP);
								(*it)->emitMessage(m);
							} else
							{
								Logic::CMessage *m = new Logic::CMessage();
								m->setType(Logic::Message::CONTROL);
								m->setAction(Logic::Message::WALK_STOP);
								(*it)->emitMessage(m);
							}
							return true;
						}
					}
				}
			}

				//Logic::TMessage m;
				//m._type = Logic::Message::CONTACT;				
				//_entity->emitMessage(m,this);				
			}		
		return false;
		}

	void CAttack::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);
			
	} //fin de CAttack:tick


} // namespace Logic