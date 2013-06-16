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

#include "Graphics/AnimatedEntity.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"
#include "Application/BaseApplication.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageInt.h"
#include "Logic/Entity/Messages/MessageAudio.h"

//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CAttack);

	bool CAttack::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("attackPower"))
			_attackPower = entityInfo->getFloatAttribute("attackPower");
		if (_entity->getName()=="locoCubriendose")
			cover();

		if (entityInfo->hasAttribute("audioCubriendose") )
			_audioCubriendose = entityInfo->getStringAttribute("audioCubriendose");
		return true;
		}

	bool CAttack::activate()
	{				
		_lightAttack=_heavyAttack=false;
				if (_entity->getType()=="OtherPlayer")
		{
			/*
			CMessageString *m = new CMessageString();	
			m->setType(Message::SET_MATERIAL);
			m->setString("marineRojo");
			_entity->emitMessage(m);
			*/
		}
		return true;
	}		
	
	bool CAttack::accept(const CMessage *message)
	{//aviso de que tanto accept como process son un poco hack, pero es es solo hasta tener un componente NPCCONTROLLER
	//	return false;
		return	message->getType() == Message::CONTROL ||
				message->getType() == Message::ANIMATION_FINISHED || 
				message->getType() == Message::ANIMATION_MOMENT ||
				message->getType() == Message::ATTACH; // HACK FRS FX Test

	}
	

	
	void CAttack::process(CMessage *message)
	{
		switch(message->getType())
		{

/////////// HACK FRS FX TEST //////////////
		case Message::ATTACH:
			_isModeBomb = message->getAction() == Message::ATTACH_TO_HAND &&
				static_cast<CMessageString*>(message)->getString() == "puPoisonOnHand2.0.mesh";				
			break;
////////////////////////////////////

		case Message::CONTROL:
			{			
				if(message->getAction() == Message::LIGHT_ATTACK)
					lightAttack();
				else if(message->getAction() == Message::HEAVY_ATTACK)
					 heavyAttack();
				else if(message->getAction() == Message::COVER)
					cover();
				break;
			}

		// TODO FRS Cuando un case se alarga tanto -> función handler especifica
		case Message::ANIMATION_FINISHED:
			{
				CMessageString* maux = static_cast<CMessageString*>(message);
				if (maux->getString().compare( Graphics::AnimNames::ATTACK1 )==0 || maux->getString().compare( Graphics::AnimNames::ATTACK2 )==0 || maux->getString().compare( Graphics::AnimNames::ATTACK3 )==0)
				{					
					_lightAttack=_heavyAttack=false;
				}
				break;
			}
		
		case Message::ANIMATION_MOMENT:
			{
				//querré saber cual animación es, de momento se que solo puedo recibir un tipo de animación
				float punto;
				if (_entity->getLogicalPosition()->getSense()==Logic::LogicalPosition::RIGHT)
					{
						punto=_entity->getLogicalPosition()->getDegree()-10;
						Logic::CMessageFloat *m = new Logic::CMessageFloat();
						m->setType(Logic::Message::AVATAR_MOVE);
						m->setAction(Logic::Message::WALK_RIGHT);
						m->setFloat(-1);
						_entity->emitMessage(m);
				}
				else
				{
					Logic::CMessageFloat *m = new Logic::CMessageFloat();
					m->setType(Logic::Message::AVATAR_MOVE);
					m->setAction(Logic::Message::WALK_LEFT);
					m->setFloat(1);
					_entity->emitMessage(m);
					punto=_entity->getLogicalPosition()->getDegree()+10;
				}	//con este metodo vemos si con la espada le estamos dando
				
					unsigned short resultadoAtaque=attackPlace(punto,_entity->getLogicalPosition()->getRing(),_entity->getLogicalPosition()->getBase(),false);
					if (resultadoAtaque==2)
					{
						CMessageBoolString *message = new CMessageBoolString();
						message->setType(Message::REWIND_ANIMATION);
						message->setString( Graphics::AnimNames::ATTACK1 ); // FRS de momento pongo la equivalente -> hay que corregir / calibrar
						message->setAction(Message::UNDEF);
						message->setBool(false);
						_entity->emitMessage(message,this);
					}					
			}
		}
	 }

	 void CAttack::cover()
	 {
		_covering=true;
			if (_entity->getType()!="Player")
			{//porque por ahora el loco no ejecuta maquina de estados
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("shieldcover");
				message->setAction(Message::UNDEF);
				message->setBool(true);
				_entity->emitMessage(message,this);
			}
	 }


	void CAttack::lightAttack() 
	{
		if (_lightAttack) return;
		_heavyAttack=false;
		_lightAttack=true;

///////////// HACK TEST FRS Para probar FX
		if(_isModeBomb) {
			CMessage *txMsg = new CMessage();	
				txMsg->setType(Message::FX_START);
				txMsg->setAction(Message::FX_BLAST);
				_entity->emitMessage(txMsg,this);
		}
////////////////////


	} // turn

	void CAttack::heavyAttack() 
	{		
		if (_heavyAttack) return;
		_heavyAttack=true;
		_lightAttack=false;

	} // turn

	//este metodo devuelve null si no se está ocupando ese grado o la entidad que ocupa ese espacio
	unsigned short CAttack::attackPlace(float grado, short ring, short base,bool soloInfo)
	{//acotar				
		CMap::TEntityList::const_iterator it = _entity->getMap()->getEntities().begin();
		CMap::TEntityList::const_iterator end = _entity->getMap()->getEntities().end();

		for(; it != end; ++it)
		{			
			//Si la entidad que comparo no soy yo mismo y la distancia entre las posiciones
			//de las dos es menor de la distancia de colisión (o radio)

			if(_entity != (*it) )
			{
				if (((*it)->getType().compare("PowerUp")!=0)&&((*it)->getType().compare("Altar")!=0)&& ((*it)->getType().compare("World")!=0)&& ((*it)->getType().compare("SkyBox")!=0))
				{//lo que hay que  hacer es que no se itere sobre entidades que no tengan componente CCollider, de momento se hace esa comprobación

			
					if (base==(*it)->getLogicalPosition()->getBase() && ring==(*it)->getLogicalPosition()->getRing())	
					{								
						//float limiteIzquierdo=(*it)->getDegree()-(*it)->getAngularBox();
						//float limiteDerecho=(*it)->getDegree()+(*it)->getAngularBox();
						float limiteIzquierdo=(*it)->getLogicalPosition()->getDegree()-5; //aquí se está asumiendo que es un angular box de 5
						float limiteDerecho=(*it)->getLogicalPosition()->getDegree()+5;
					//	if (_entity->getLogicalPosition()->getDegree()<grado) limiteIzquierdo =_entity->getLogicalPosition()->getDegree();
					//	else if (_entity->getLogicalPosition()->getDegree()>grado) limiteDerecho =_entity->getLogicalPosition()->getDegree();
						if ((*it)->getComponent<CAttack>()!=NULL)
						if (grado>limiteIzquierdo && grado<limiteDerecho 
							|| (grado>=limiteIzquierdo && grado>=limiteDerecho &&  _entity->getLogicalPosition()->getDegree()<=limiteIzquierdo && _entity->getLogicalPosition()->getDegree()<=limiteDerecho) 
							||  (grado<=limiteIzquierdo && grado<=limiteDerecho &&  _entity->getLogicalPosition()->getDegree()>=limiteIzquierdo && _entity->getLogicalPosition()->getDegree()>=limiteDerecho) 
							)
						{
							if (!soloInfo)
							{				
								if ((*it)->getComponent<CAttack>()!=NULL)
								if ((*it)->getComponent<CAttack>()->_covering==true && (*it)->getLogicalPosition()->getSense()!=_entity->getLogicalPosition()->getSense())
								{
									Logic::CMessage *m = new Logic::CMessage();
									m->setType(Logic::Message::CONTROL);
									m->setAction(Logic::Message::WALK_STOP);
									(*it)->emitMessage(m);
								
									Logic::CMessageAudio *cubiertoAudio=new Logic::CMessageAudio();		
									cubiertoAudio->setType(Message::AUDIO);			
									cubiertoAudio->setPath("media\\audio\\Sword_scrape2.wav");//ñapa
									cubiertoAudio->setId("impactoCubriendose");
									cubiertoAudio->setPosition(_entity->getPosition());
									(*it)->emitMessage(cubiertoAudio);
										
									return 2; //Impacto en el que el objetivo está cubriendose
								}
								else
								{
									CMessageInt *m2 = new CMessageInt();
									m2->setInt(-10);
									m2->setType(Message::LIFE_MODIFIER);						
									m2->setAction(Message::DAMAGE);						

									(*it)->emitMessage(m2,this);
									//veces++;
									//std::cout<<veces<<std::endl;
									Logic::CMessage *m = new Logic::CMessage();
									m->setType(Logic::Message::CONTROL);
									m->setAction(Logic::Message::WALK_STOP);
									(*it)->emitMessage(m);
									return 1; //Impacto con daño
								}
							}
						}
					}
				}
			}					
		}		
		return 0; //le has dado al aire
		}

	void CAttack::resetAttackFlags()
	{
		_heavyAttack=false;
		_lightAttack=false;

	}
	
	void CAttack::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);
			
	} //fin de CAttack:tick


} // namespace Logic