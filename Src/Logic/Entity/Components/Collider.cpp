/**
@file Collider.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CCollider
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Collider.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Component.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageBool.h"


#include "Map/MapParser.h"



namespace Logic 
{
	IMP_FACTORY(CCollider);
	
	//---------------------------------------------------------

	bool CCollider::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		_excluido=NULL;
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CCollider::activate()
	{
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CCollider::deactivate()
	{

	} // deactivate
	
	
	//---------------------------------------------------------

	bool CCollider::accept(const CMessage *message)
	{
		
		return message->getType() == Message::CONTACTO;
	} // accept
	
	//---------------------------------------------------------

	void CCollider::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTACTO:  //[ƒ®§] Pero..?
			//Contacto();
			break;
		 //PROCESAR EL CONTACTO, PARAR A LAS ENTIDADES IMPLICADAS
		
		}

	} // process
	
	//---------------------------------------------------------

	//---------------------------------------------------------

	bool CCollider::contactoAngular( CEntity* entidad1, CEntity* entidad2)
	{
		if (entidad1==entidad2)
			return false;
		if (entidad1->getBase()!=entidad2->getBase()) 
			return false;
		if (entidad1->getRing()!=entidad2->getRing()) 
			return false;
	

		if (entidad1->getJumping()==true || entidad2->getJumping()==true)
			return false;


		if (entidad2->getAngularBox()==0)
			return false;
		float logicalCenterDistance=abs(entidad1->getDegree()-entidad2->getDegree());//distancia entre los centros de las entidades
		if (logicalCenterDistance>180) //
			logicalCenterDistance=360-logicalCenterDistance;

		float angularBoxAmount=entidad1->getAngularBox()+entidad2->getAngularBox();
		//if (this->getType().compare("AnimatedEntity")==0)
		//	int i=0;
		if (logicalCenterDistance<=angularBoxAmount) //si la distancia de los centros es menor que la suma de los radios hay contacto
		{	
			return true;		
		}
			return false;
	}

	bool CCollider::contactoExtremo( CEntity* entidad1, CEntity* entidad2)
	{		
		if (entidad2->getAngularBox()==0)
			return false;
		float logicalCenterDistance=abs(entidad1->getDegree()-entidad2->getDegree());//distancia entre los centros de las entidades
		if (logicalCenterDistance>180) //
			logicalCenterDistance=360-logicalCenterDistance;

		if (logicalCenterDistance<((entidad1->getAngularBox()+entidad2->getAngularBox()/2)))
			return true;

		return false;

		float angularBoxAmount=entidad1->getAngularBox()+entidad2->getAngularBox();
		//if (this->getType().compare("AnimatedEntity")==0)
		//	int i=0;
		if (logicalCenterDistance<=angularBoxAmount) //si la distancia de los centros es menor que la suma de los radios hay contacto
		{	
			return true;		
		}
			return false;
	}


	bool CCollider::contacto( CEntity* entidad1, CEntity* entidad2)
	{
		CMessageFloat *m = new CMessageFloat();


		//_Vida--; si tiene vida se le disminuye, si es un proyectil no tiene vida
		// Pablo 28-01-2013
		m->setType(Message::CONTACTO);
		m->setAction(Message::UPDATE_LIFE);
		m->setFloat(10.0f);
		entidad1->emitMessage(m,this);
		//entidad2->emitMessage(m,this);
		//fin emision mensaje de vida

		CMessageString *m2 = new CMessageString();
		m2->setString("luminoso");
		m2->setType(Message::SET_SHADER);
		entidad1->emitMessage(m2,this);
		entidad2->emitMessage(m2,this);

		CMessageBoolFloat *m3 = new CMessageBoolFloat();
		CMessageBoolFloat *m4 = new CMessageBoolFloat();
		if (entidad1->getType().compare("Player")==0)
			m3->setType(Message::CONTROL);
		if (entidad1->getType().compare("AnimatedEntity")==0)
			m3->setType(Message::NPC_CONTROL);


	//	_sentidoColision=sentidoColision(entidad1,entidad2);
        m3->setBool(sentidoColision(entidad1,entidad2));
		m4->setBool(!m3->getBool());
//		m._bool=_sentidoColision;

		//corrección para cuerpos superspuestos
		if (contactoExtremo(entidad1,entidad2))
		{
			m3->setAction(Message::WALK_BACK);
			m4->setAction(Message::WALK_BACK);
			m3->setFloat(((entidad1->getAngularBox()+entidad2->getAngularBox())/4));
			m4->setFloat(((entidad1->getAngularBox()+entidad2->getAngularBox())/4));
			entidad1->emitMessage(m3,this);	

			if (entidad2->getType().compare("Player")==0)
				m4->setType(Logic::Message::CONTROL);
			if (entidad2->getType().compare("AnimatedEntity")==0)
				m4->setType(Logic::Message::NPC_CONTROL);


			entidad2->emitMessage(m4,this);	
		}
		else    // HACK [ƒ®§] Si este componente está haciendo las veces de CPhysic, nunca jamás
		{	// debería actuar sobre las animaciones (de ahi lo de separar el CAngularMovement 
			// de animaciones mediante el CAvatarController).			
			// Lo que suele hacer el CPhysic para detener un movimiento en colision
			// en realidad es filtrar: el CPhysic se coloca antes de los receptores del SET_TRANSFORM, 
			// recoge los SET_TRANS enviados, los filtra/capa y los reenvia a los gráficos. Luego
			// tambien tiene un AVATAR_WALK que intenta ejecutar un movimiento y retransmite el SET_TRANSFORM 
			// del resultado de ese movimiento. Eso es lo que crea el efecto de tio corriendo contra 
			// una pared sin moverse. De momento, como me conviene arreglarlo rápido para que salgan
			// animaciones de daños y muerte en el HITO 2, lo quito.
			CMessage *m5 = new CMessage();
			CMessage *m6 = new CMessage();
						if (entidad1->getType().compare("Player")==0)
				m5->setType(Message::CONTROL);
			if (entidad1->getType().compare("AnimatedEntity")==0)
				m5->setType(Message::NPC_CONTROL);
			m5->setAction(Message::WALK_STOP);
			entidad1->emitMessage(m5,this);				
			m6->setAction(Message::WALK_STOP);

			if (entidad2->getType().compare("Player")==0)
				m6->setType(Logic::Message::CONTROL);
			if (entidad2->getType().compare("AnimatedEntity")==0)
				m6->setType(Logic::Message::NPC_CONTROL);
			


			entidad2->emitMessage(m6,this);	
			//entidad1->emitMessage(m,this);	
			//m._bool=!m._bool;
			//if (entidad2->getType().compare("Player")==0)
			//	m._type = Logic::Message::CONTROL;
			//if (entidad2->getType().compare("AnimatedEntity")==0)
			//	m._type = Logic::Message::NPC_CONTROL;
			//entidad2->emitMessage(m,this);	
		}
		CMessageBool *m7 = new CMessageBool();
		CMessageBool *m8 = new CMessageBool();
		m7->setAction(Message::CHANGE_DIRECTION);
		m8->setAction(Message::CHANGE_DIRECTION);
			
		m7->setBool(sentidoColision(entidad1,entidad2));
		m8->setBool(!m7->getBool());
			if (entidad1->getType().compare("Player")==0)
				m7->setType(Message::CONTROL);
			if (entidad1->getType().compare("AnimatedEntity")==0)
				m7->setType(Message::NPC_CONTROL);

		entidad1->emitMessage(m7,this);	
			if (entidad2->getType().compare("Player")==0)
				m8->setType(Message::CONTROL);
			if (entidad2->getType().compare("AnimatedEntity")==0)
				m8->setType(Message::NPC_CONTROL);

		entidad2->emitMessage(m8,this);	

		// HACK [ƒ®§0207] No sé lo que  viene encima y miedo me da meterme ^^", ¿pero lo suyo no sería enviar un mensaje de daño y que lo reciba quien pueda ser dañado?
		// Aunque para independizar este detector de físicas de daños, debería ser el componente dañino del entity2 el que, tras ser avisado de que ha contactado
		// con el entity1, le enviara un mensaje DAMAGED con el CDamage::_DAMAGE que sólo él conoce (el CDamage de entity2)		
		//TMessage msg;
		//	msg._type = TMessageType::DAMAGED;
		//	msg._int = 10; // los hp suelen ser enteros
		//entidad1->emitMessage(msg, this);	

		return false;
		}

		bool CCollider::sentidoColision(const CEntity* entidad1,const CEntity* entidad2)
	{
		float degree1=entidad1->getDegree();
		float degree2=entidad2->getDegree();
		
		if (abs(abs(degree1)-abs(degree2))>200)
		
			if (degree1<degree2) //esta comprobación es para los casos en el extremo (grado 0 o 360) 
				degree1+=360;
			else
				degree2+=360;

		if (degree1<degree2)
			return false;
		else 
			return true;

	}

	void CCollider::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);		

		//me gustaría que en esta lista tan sólo estuvieran entidades con componente CColider
		//if (_entity->getType().compare("Player")==0)
		//{
		CMap::TEntityList::const_iterator it = _entity->getMap()->getEntities().begin();
		CMap::TEntityList::const_iterator end = _entity->getMap()->getEntities().end();
		_excluido=NULL;
		_comprobando=!_comprobando;
		//if (_comprobando)
		for(; it != end; ++it)
		{
		
			//Si la entidad que comparo no soy yo mismo y la distancia entre las posiciones
			//de las dos es menor de la distancia de colisión (o radio)
			
			if(_entity != (*it) && contactoAngular(_entity,(*it)) )
			{
				if (_excluido==NULL && ((*it)->getType().compare("Altar")!=0)&& ((*it)->getType().compare("World")!=0))
				{//lo que hay que  hacer es que no se itere sobre entidades que no tengan componente CCollider, de momento se hace esa comprobación
					contacto(_entity,(*it));
 					//_excluido=(*it);

					break;
				}
				//Logic::TMessage m;
				//m._type = Logic::Message::CONTACTO;				
				//_entity->emitMessage(m,this);				
			}
			} // tick
		}
//	}
		


} // namespace Logic

