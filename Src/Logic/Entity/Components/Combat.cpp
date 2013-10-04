/**
@file Combat.cpp

Contiene la declaración del componente que controla el ataque 
angular de entidades.

@see Logic::CCombat
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/

#include "Combat.h"

#include <Application/BaseApplication.h> // HACK FRS Que tendremos que evitar en algún momento
#include <Logic/Entity/Entity.h>
#include <Logic/Entity/Messages/Message.h>
#include <Logic/Entity/Messages/MessageAudio.h>
#include <Logic/Entity/Messages/MessageFloat.h>
#include <Logic/Entity/Messages/MessageBoolUShort.h>
#include <Logic/Entity/Messages/MessageUShort.h>
#include <Logic/Entity/Messages/MessageUInt.h>
#include <Logic/Entity/Messages/MessageString.h>
#include <Logic/Maps/Map.h>
#include <Map/Entity.h>

#include "AnimatedGraphics.h"
#include "logic\server.h"


//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CCombat);

	bool CCombat::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("attackPower"))
			_attackPower = entityInfo->getFloatAttribute("attackPower");

		if (_entity->getName()=="locoCubriendose")
			cover();

		if (entityInfo->hasAttribute("audioCubriendose") )
			_audioCubriendose = entityInfo->getStringAttribute("audioCubriendose");

		//PT Modificadores de daño de ataque (ligero y pesado)
		//assert( entityInfo->hasAttribute("lifeModifierLightAttack") );
		if(entityInfo->hasAttribute("lifeModifierLightAttack"))
			_lifeModifierLightAttack = entityInfo->getIntAttribute("lifeModifierLightAttack");

		//assert( entityInfo->hasAttribute("lifeModifierHeavyAttack") );
		if(entityInfo->hasAttribute("lifeModifierHeavyAttack"))
			_lifeModifierHeavyAttack = entityInfo->getIntAttribute("lifeModifierHeavyAttack");
		
		if(entityInfo->hasAttribute("arrowFighter"))
			_arrowFighter = entityInfo->getBoolAttribute("arrowFighter");

		return true;
		}

	bool CCombat::activate()
	{				
		//_lightAttack=_heavyAttack=false;

		return true;
	}		
	
	bool CCombat::accept(const CMessage *message)
	{//aviso de que tanto accept como process son un poco hack, pero es es solo hasta tener un componente NPCCONTROLLER
	//	return false;
		return (message->getType() == Message::CONTROL && 
				(message->getAction() == Message::LIGHT_ATTACK || 
				message->getAction() == Message::HEAVY_ATTACK || 
				message->getAction() == Message::COVER) || 
				(message->getType()==Message::ANIMATION_FINISHED || message->getType()==Message::DAMAGE_MOMENT)) ||
				message->getType() == Message::ATTACH; // HACK FRS FX Test
	}
	
	void CCombat::process(CMessage *message)
	{
		switch(message->getType())
		{

/////////// HACK FRS FX TEST //////////////
		case Message::ATTACH:
			_isModeBomb = message->getAction() == Message::ATTACH_TO_HAND &&
				static_cast<CMessageString*>(message)->getString().substr(0,14) == "puPoisonOnHand";				
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
				CMessageUShort* maux = static_cast<CMessageUShort*>(message);
				if (maux->getUShort()==Logic::ATTACK1 || maux->getUShort()==Logic::ATTACK2 || maux->getUShort()==Logic::ATTACK3)
				{					
					//_lightAttack=_heavyAttack=false;
				}
				break;
			}
		
		case Message::DAMAGE_MOMENT:
			{
				//querré saber cual animación es, de momento se que solo puedo recibir un tipo de animación
				float punto;
				if (_entity->getLogicalPosition()->getSense()==Logic::LogicalPosition::RIGHT)
					punto=_entity->getLogicalPosition()->getDegree()-10;
				else
					punto=_entity->getLogicalPosition()->getDegree()+10;
					//con este metodo vemos si con la espada le estamos dando
				
				if (_arrowFighter)
					 fireArrow();
					unsigned short resultadoAtaque=attackToPlace(punto,_entity->getLogicalPosition()->getRing(),_entity->getLogicalPosition()->getBase(),false);
					if (resultadoAtaque==2)
					{
						CMessageBoolUShort *message = new CMessageBoolUShort();
						message->setType(Message::REWIND_ANIMATION);
						message->setUShort(Logic::ATTACK1); // FRS de momento pongo la equivalente -> hay que corregir / calibrar
						message->setBool(false);
						_entity->emitMessage(message,this);
					}					
			}
		}
	 }

	 void CCombat::cover()
	 {
		_covering=true;
			if (_entity->getType()!="Player")
			{//porque por ahora el loco no ejecuta maquina de estados
				CMessageBoolUShort *message = new CMessageBoolUShort();
				message->setType(Message::SET_ANIMATION);
				message->setUShort(Logic::COVER_WITH_SHIELD);
				message->setBool(true);
				_entity->emitMessage(message,this);

			
			}
	 }

	 void CCombat::fireArrow()
	 {
		 
		 Logic::Ring ring = _entity->getLogicalPosition()->getRing();
		unsigned short int degree = (unsigned short int)_entity->getLogicalPosition()->getDegree();
		Logic::Sense sense =_entity->getLogicalPosition()->getSense();
		unsigned short numBase =_entity->getLogicalPosition()->getBase();

		short int newdegree = degree;

		if(sense==LogicalPosition::RIGHT) //seeing right
			newdegree-= 10;
		else if(LogicalPosition::LEFT) //seeing left
			newdegree+= 10;

		//correction in the degrees where ally appears
		/*if(newdegree<0)
			newdegree = 360 + newdegree;
		if(newdegree > 360)
			newdegree = newdegree - 360;
			*/
		const std::string type = "Arrow";
		
		if(sense!=LogicalPosition::Sense::LEFT && sense!=LogicalPosition::Sense::RIGHT)
		sense = LogicalPosition::Sense::LEFT;

		CLogicalPosition pos;
		pos.setDegree(_entity->getLogicalPosition()->getDegree());
		pos.setRing(ring);
		pos.setBase(numBase);
		pos.setSense(sense);

		_entity->getMap()->createProjectile(type,pos,_entity);
	 }

	void CCombat::lightAttack() 
	{
		//if (_lightAttack) return;
		//_heavyAttack=false;
		//_lightAttack=true;

		////Emito mensaje al componente Life para modificarle la vida
		 //CMessageUInt *muint = new CMessageUInt();
		 //muint->setType(Message::LIFE_MODIFIER);
		 //muint->setAction(Message::DAMAGE);
		 //muint->setUInt(_lifeModifierLightAttack);
		 //_entity->emitMessage(muint);
///////////// HACK TEST FRS Para probar FX
		//if(_isModeBomb) {
		//	CMessage *txMsg = new CMessage();	
		//		txMsg->setType(Message::FX_START);
		//		txMsg->setAction(Message::FX_BLAST);
		//		_entity->emitMessage(txMsg,this);
		//}
////////////////////

	//	if (_arrowFighter)
	//	    fireArrow();


		_attackPower = _lifeModifierLightAttack;
	} // lightAttack

	void CCombat::heavyAttack() 
	{		
		//if (_heavyAttack) return;
		//_heavyAttack=true;
		//_lightAttack=false;

		////Emito mensaje al componente Life para modificarle la vida
		// CMessageUInt *muint = new CMessageUInt();
		// muint->setType(Message::LIFE_MODIFIER);
		// muint->setAction(Message::DAMAGE);
		// muint->setUInt(_lifeModifierHeavyAttack);
		// _entity->emitMessage(muint);

		_attackPower = _lifeModifierHeavyAttack;
		

	} // heavyAttack

	//este metodo devuelve null si no se está ocupando ese grado o la entidad que ocupa ese espacio
	unsigned short CCombat::attackToPlace(float grado, short ring, short base,bool soloInfo)
	{//acotar				
		CMap::TEntityList::const_iterator it = _entity->getMap()->getEntities().begin();
		CMap::TEntityList::const_iterator end = _entity->getMap()->getEntities().end();

		for(; it != end; ++it)
		{			
			//Si la entidad que comparo no soy yo mismo y la distancia entre las posiciones
			//de las dos es menor de la distancia de colisión (o radio)

			if(_entity != (*it) )
			{
				if (((*it)->getType().compare("PowerUp")!=0)&&((*it)->getType().compare("Altar")!=0)&& ((*it)->getType().compare("World")!=0)&& ((*it)->getType().compare("Sky")!=0)&&((*it)->getType().compare("Arrow")!=0))
				{//lo que hay que  hacer es que no se itere sobre entidades que no tengan componente CCollider, de momento se hace esa comprobación

			
					if (base==(*it)->getLogicalPosition()->getBase() && ring==(*it)->getLogicalPosition()->getRing())	
					{								
						//float limiteIzquierdo=(*it)->getDegree()-(*it)->getAngularBox();
						//float limiteDerecho=(*it)->getDegree()+(*it)->getAngularBox();
						float limiteIzquierdo=(*it)->getLogicalPosition()->getDegree()-5; //aquí se está asumiendo que es un angular box de 5
						float limiteDerecho=(*it)->getLogicalPosition()->getDegree()+5;
					//	if (_entity->getLogicalPosition()->getDegree()<grado) limiteIzquierdo =_entity->getLogicalPosition()->getDegree();
					//	else if (_entity->getLogicalPosition()->getDegree()>grado) limiteDerecho =_entity->getLogicalPosition()->getDegree();
						if ((*it)->hasComponent<CCombat>() && (*it)->getComponent<CCombat>()->isVulnerable())
						if (grado>limiteIzquierdo && grado<limiteDerecho 
							|| (grado>=limiteIzquierdo && grado>=limiteDerecho &&  _entity->getLogicalPosition()->getDegree()<=limiteIzquierdo && _entity->getLogicalPosition()->getDegree()<=limiteDerecho) 
							||  (grado<=limiteIzquierdo && grado<=limiteDerecho &&  _entity->getLogicalPosition()->getDegree()>=limiteIzquierdo && _entity->getLogicalPosition()->getDegree()>=limiteDerecho) 
							)
						{
							if (!soloInfo)
							{				
								if ((*it)->hasComponent<CCombat>())
								{
								if ((*it)->getComponent<CCombat>()->_covering==true && (*it)->getLogicalPosition()->getSense()!=_entity->getLogicalPosition()->getSense())
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
									cubiertoAudio->setNotIfPlay(false);
									cubiertoAudio->setIsPlayer(false);
									(*it)->emitMessage(cubiertoAudio);
										
									return 2; //Impacto en el que el objetivo está cubriendose
								}
								else
								{
									CMessageUInt *m2 = new CMessageUInt();
									//m2->setUInt(-10);
									m2->setUInt(_attackPower);
									m2->setType(Message::LIFE_MODIFIER);						
									m2->setAction(Message::DAMAGE);						
									(*it)->emitMessage(m2);

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
		}		
		return 0; //le has dado al aire
	}

	void CCombat::resetAttackFlags()
	{
		//_heavyAttack=false;
		//_lightAttack=false;

	}
	
	void CCombat::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);
			
	} //fin de CCombat:tick


} // namespace Logic