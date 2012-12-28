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

#include "Map/MapParser.h"

namespace Logic 
{
	IMP_FACTORY(CCollider);
	
	//---------------------------------------------------------

	bool CCollider::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
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
	
	bool CCollider::sentidoColision(CEntity* entidad1)
	{
		float degree1=_entity->getDegree();
		float degree2=entidad1->getDegree();
		if (degree1<degree2)
			return false;
		return true;

	}
	//---------------------------------------------------------

	bool CCollider::accept(const TMessage &message)
	{
		return false; //anulacion de mensajes
		//return message._type == Message::CONTACTO;
	} // accept
	
	//---------------------------------------------------------

	void CCollider::process(const TMessage &message)
	{
		/*switch(message._type)
		{
		case Message::CONTACTO:Contacto();
			break;
		 //PROCESAR EL CONTACTO, PARAR A LAS ENTIDADES IMPLICADAS
		
		}*/

	} // process
	
	//---------------------------------------------------------

	//---------------------------------------------------------

	void CCollider::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		if (_hit>0)
		{
			_hit++;
			if(_hit>5)
			{
				_hit=0;
     			Logic::TMessage m;
				m._string="marine";
				m._type = Logic::Message::SET_SHADER;
				_entity->emitMessage(m,this);
			}
		}

		//me gustaría que en esta lista tan sólo estuvieran entidades con componente CColider
		CMap::TEntityList::const_iterator it = _entity->getMap()->getEntities().begin();
		CMap::TEntityList::const_iterator end = _entity->getMap()->getEntities().end();
		for(; it != end; it++)
			//Si la entidad que comparo no soy yo mismo y la distancia entre las posiciones
			//de las dos es menor de la distancia de colisión (o radio)
			if(_entity != (*it) && _entity->contactoAngular((*it)) )
			{
				//Logic::TMessage m;
				//m._type = Logic::Message::CONTACTO;
				_sentidoColision=sentidoColision((*it));//devuelve true si la entidad con la que se ha colisionado está a la izquierda
				//_entity->emitMessage(m,this);
				Contacto();
			} // tick


	}

	void CCollider::Contacto()
	{
		Logic::TMessage m;
		//_Vida--; si tiene vida se le disminuye, si es un proyectil no tiene vida

		//if (_Vida<=0){	
			//_entity->getMap()->getScene()->removeEntity(_entity->_gentity);
			if (_entity->getType().compare("Player")==0)
			{
				
				m._string="walkBack";
				m._type = Logic::Message::CONTROL;
				m._bool=_sentidoColision;
				_entity->emitMessage(m,this);				
				
				_hit++;
				m._string="luminoso";
				m._type = Logic::Message::SET_SHADER;
				_entity->emitMessage(m,this);
			}		
			if (_entity->getType().compare("AnimatedEntity")==0)
			{
				_hit++;
				m._string="luminoso";
				m._type = Logic::Message::SET_SHADER;
				_entity->emitMessage(m,this);
			}
	}

} // namespace Logic

