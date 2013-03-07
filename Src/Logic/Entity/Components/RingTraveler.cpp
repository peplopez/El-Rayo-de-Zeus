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

#include "Logic/Entity/Components/RingTraveler.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageChar.h"


#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::RING_TRAVELER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


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
	{//que no os confunda el nombre de mensaje CHANGE_PLANE es tanto para cambiar de base como de anillo dentro de la base. Apreciad que en cualquier caso siempre es un cambio de anillo, de ahí el nombre
		return message->getType() == Message::CONTROL && (
						message->getAction() == Message::GO_DOWN || 
						message->getAction() == Message::GO_UP
				);
	} // accept
	
	//---------------------------------------------------------

	void CRingTraveler::process(CMessage *message)
	{
		if(message->getAction() != Message::GO_DOWN && message->getAction() != Message::GO_UP ) 
			return;

		_changingRing=true; //TODO Pablo. Sólo si no esta saltandose puede realizar la accion de cambio de anillo.
		
		CMessageChar *m = new CMessageChar();	
			m->setType(Message::AVATAR_MOVE);
			m->setAction(Message::CHANGE_RING);		
			m->setChar( 
				message->getAction() == Message::GO_UP ? 
				1 : -1  // ƒ®§ GO_UP (+1) vs GO_DOWN (-1)  son las únicas dos opciones que pasan el filtro del accept
			);
		_entity->emitMessage(m,this);	

		LOG("Change Ring: " << (int) m->getChar() );
 	} // process

	
	// UNDONE ƒ®§
	//void CRingTraveler::goDown()
	//{
	//	//Pablo. Sólo si no esta saltandose puede realizar la accion de cambio de anillo.
	//	_changingRing=true;

	//	CMessageInt *m = new CMessageInt();	
	//		m->setType(Message::AVATAR_MOVE);
	//		m->setAction(Message::CHANGE_RING);		
	//		m->setInt(-1); // -1 Anillo
	//	_entity->emitMessage(m,this);	
	//}
	//	
	//void CRingTraveler::goUp()
	//{
	//	_changingRing=true;

	//	CMessageInt *m = new CMessageInt();	
	//		m->setType(Message::AVATAR_MOVE);
	//		m->setAction(Message::CHANGE_RING);		
	//		m->setInt(1); // +1 Anillo
	//	_entity->emitMessage(m,this);		
	//}

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

