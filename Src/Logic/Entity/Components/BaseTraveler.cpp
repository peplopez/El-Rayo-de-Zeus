/**
@file BaseTraveler.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CBaseTraveler
@see Logic::IComponent

@author Jose Luis López Sánchez
@date Febrero, 2013
*/

#include "Logic/Entity/Entity.h"

#include "Logic/Entity/Components/BaseTraveler.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageString.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::BASE_TRAVELER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic 
{
	IMP_FACTORY(CBaseTraveler);
	
	//---------------------------------------------------------

	CBaseTraveler::~CBaseTraveler() 
	{


	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CBaseTraveler::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		return true;

	} // spawn
	
	//---------------------------------------------------------

	
	bool CBaseTraveler::accept(const CMessage *message)
	{//que no os confunda el nombre de mensaje CHANGE_PLANE es tanto para cambiar de base como de anillo dentro de la base. Apreciad que en cualquier caso siempre es un cambio de anillo, de ahí el nombre
		return (CRingTraveler::accept(message) || 
					(message->getType() == Message::CONTROL &&
					message->getAction() == Message::CHANGE_BASE));

	} // accept
	
	//---------------------------------------------------------

	void CBaseTraveler::process(CMessage *message)
	{
		CRingTraveler::process(message);
		CMessageUShort *maux = static_cast<CMessageUShort*>(message); // TODO FRS esto podría ser char...
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::CHANGE_BASE)
				CBaseTraveler::changeBase(maux->getUShort());		
		}

	} // process

			
	void CBaseTraveler::changeBase(int base)
	{
		_changingBase=true;

			CMessageChar *m = new CMessageChar();	
				m->setType(Message::AVATAR_MOVE);
				m->setAction(Message::CHANGE_BASE);
				m->setChar( base - (int) _entity->getBase() ); // ƒ®§ Enviamos diferencial de base (AVATAR_MOVE es movimiento diferencial)
			_entity->emitMessage(m,this);

		LOG("Change Base from " << _entity->getBase() << " to " << base );

			// UNDONE
		/*	if (_entity->getRing()==Ring::UPPER_RING)
			{
				_entity->setBase(base);
				Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getHeight(),_entity->getBase(),_entity->getRing());
				_entity->setPosition(newPosition);
			}
			if (_entity->getRing()==Ring::CENTRAL_RING)
			{
				_entity->setBase(base);
				Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getHeight(),_entity->getBase(),_entity->getRing());
				_entity->setPosition(newPosition);
			}
			if (_entity->getRing()==Ring::LOWER_RING)
			{
				_entity->setBase(base);
				Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getHeight(),_entity->getBase(),_entity->getRing());
				_entity->setPosition(newPosition);
			}*/

	}


	void CBaseTraveler::tick(unsigned int msecs)
	{
			//CRingTraveler::tick(msecs);
			IComponent::tick(msecs);
			
			if (_changingRing || _changingBase)
			{
				if(_entity->isPlayer())
				{
					CMessageString *m = new CMessageString();	
					m->setType(Message::SET_MATERIAL);
					m->setString("transito");
					_entity->emitMessage(m,this);

					if (_changingBase)
					{
						_changingBaseTime+=msecs;
						if (_changingBaseTime>_maxChangingBaseTime)
						{
							_changingBase=false;
							_changingBaseTime=0;
							CMessageString *m2 = new CMessageString();	
							m2->setType(Message::SET_MATERIAL);
							m2->setString("marine");
							_entity->emitMessage(m2,this);
						}
					}
					if (_changingRing)
						{
						_changingRingTime+=msecs;
						if (_changingRingTime>_maxChangingRingTime)
						{
							_changingRing=false;
							_changingRingTime=0;
							CMessageString *m3 = new CMessageString();	
							m3->setType(Message::SET_MATERIAL);
							m3->setString("marine");
							_entity->emitMessage(m3,this);
						}
					}
				}
			}
		}

} // namespace Logic

