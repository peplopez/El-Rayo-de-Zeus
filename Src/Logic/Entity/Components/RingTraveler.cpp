/**
@file RingTraveler.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CRingTraveler
@see Logic::IComponent

@author Jose Luis López Sánchez
@date Febrero, 2013
*/
#include "Logic/Entity/Entity.h"
//#include "Logic/Maps/Map.h"
//#include "Map/MapEntity.h"
#include "Logic/Entity/Components/RingTraveler.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageString.h"

namespace Logic 
{
	IMP_FACTORY(CRingTraveler);
	
	//---------------------------------------------------------

	CRingTraveler::~CRingTraveler() 
	{


	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CRingTraveler::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		

		return true;

	} // spawn
	
	//---------------------------------------------------------

	
	bool CRingTraveler::accept(const CMessage *message)
	{//que no os confunda el nombre de mensaje CHANGE_RING es tanto para cambiar de base como de anillo dentro de la base. Apreciad que en cualquier caso siempre es un cambio de anillo, de ahí el nombre
		return message->getType() == Message::CHANGE_RING;

	} // accept
	
	//---------------------------------------------------------

	void CRingTraveler::process(CMessage *message)
	{
		CMessage *maux = static_cast<CMessage*>(message);
		switch(message->getType())
		{
		case Message::CHANGE_RING:
			if(message->getAction() == Message::GO_UP)
				goUp();
		else if(message->getAction() == Message::GO_DOWN)
				goDown();
		}

	} // process

	
		void CRingTraveler::goDown()
		{
			_changingRing=true;
			//Pablo. Sólo si no esta saltandose puede realizar la accion de cambio de anillo.
			if(_entity->getJumping()==false)
			{
				_changingRing=true;
				if (_entity->getRing()==Ring::ANILLO_CENTRAL)
				{
					_entity->setRing(Ring::ANILLO_INFERIOR);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
				if (_entity->getRing()==Ring::ANILLO_SUPERIOR)
				{
					_entity->setRing(Ring::ANILLO_CENTRAL);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
			}			
		}
		
		void CRingTraveler::goUp()
		{
			_changingRing=true;
			//Pablo. Sólo si no esta saltandose puede realizar la accion de cambio de anillo.
			if(_entity->getJumping()==false)
			{
				_changingRing=true;	
				if (_entity->getRing()==Ring::ANILLO_CENTRAL)
				{
					_entity->setRing(Ring::ANILLO_SUPERIOR);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
				if (_entity->getRing()==Ring::ANILLO_INFERIOR)
				{
					_entity->setRing(Ring::ANILLO_CENTRAL);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
			}			
		}
		void CRingTraveler::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);

		if (_changingRing)
			{
				if(_entity->isPlayer())
				{
					CMessageString *m = new CMessageString();	
					m->setType(Message::SET_SHADER);
					m->setString("transito");
					_entity->emitMessage(m,this);
					
					if (_changingRing)
						{
						_changingRingTime+=msecs;
						if (_changingRingTime>_maxChangingRingTime)
						{
							_changingRing=false;
							_changingRingTime=0;
							CMessageString *m3 = new CMessageString();	
							m3->setType(Message::SET_SHADER);
							m3->setString("marine");
							_entity->emitMessage(m3,this);
						}
					}
				}
			}
		}

} // namespace Logic

