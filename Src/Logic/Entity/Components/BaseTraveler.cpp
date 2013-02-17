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
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageShort.h"

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
		return CRingTraveler::accept(message)||message->getType() == Message::CONTROL;

	} // accept
	
	//---------------------------------------------------------

	void CBaseTraveler::process(CMessage *message)
	{		
		CRingTraveler::process(message);
		CMessageShort* maux = static_cast<CMessageShort*>(message);
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::CHANGE_BASE)
				CBaseTraveler::changeBase(maux->getShort());	
		}

	} // process

			
		void CBaseTraveler::changeBase(int base)
		{
			_changingBase=true;

				CMessageShort *m = new CMessageShort();	
				m->setType(Message::AVATAR_MOVE);
				m->setAction(Message::CHANGE_BASE);
				m->setShort(base);
				_entity->emitMessage(m,this);
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
					m->setType(Message::SET_SHADER);
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
							m2->setType(Message::SET_SHADER);
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
							m3->setType(Message::SET_SHADER);
							m3->setString("marine");
							_entity->emitMessage(m3,this);
						}
					}
				}
			}
		}

} // namespace Logic

