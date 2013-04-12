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
	} // ~CGRingTraveler
	
	void CRingTraveler::timeArrived()
	{
		if (_changingRing /*&& !_desencogiendo*/)
		{
			CMessageChar *m0 = new CMessageChar();	
			m0->setType(Message::AVATAR_MOVE);
			m0->setAction(Message::CHANGE_RING);		
			m0->setChar(_toUp);
			_entity->emitMessage(m0,this);	
			LOG("Change Ring: " << (int) m0->getChar() );
							
			_changingRing=false;
			_desencogiendo=true;
			_changingRingTime=0;
		}else
		{
			/*
			CMessageString *m3 = new CMessageString();	
			m3->setType(Message::SET_MATERIAL);
			m3->setString("marine");
			_entity->emitMessage(m3,this);
			*/
			_desencogiendo=false;

		
		}
	}

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
		if (_entity->getLogicalPosition()->getRing()==Logic::LogicalPosition::LOWER_RING && message->getAction()==Message::GO_DOWN)
			return false;
		if (_entity->getLogicalPosition()->getRing()==Logic::LogicalPosition::UPPER_RING && message->getAction()==Message::GO_UP)
			return false;

		if (_desencogiendo) return false;
		return (!_changingRing && isAwake() && message->getType() == Message::CONTROL && (
						message->getAction() == Message::GO_DOWN || 
						message->getAction() == Message::GO_UP));
	} // accept
	
	//---------------------------------------------------------

	void CRingTraveler::process(CMessage *message)
	{
		if(message->getAction() != Message::GO_DOWN && message->getAction() != Message::GO_UP ) 
			return;
		if (_changingRing || !isAwake() ) return;
		_changingRing=true; //TODO Pablo. Sólo si no esta saltandose puede realizar la accion de cambio de anillo.
		message->getAction() == Message::GO_UP ? _toUp = 1 : _toUp = -1;  // ƒ®§ GO_UP (+1) vs GO_DOWN (-1)  son las únicas dos opciones que pasan el filtro del accept
 	} // process

	void CRingTraveler::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);
	}

} // namespace Logic

