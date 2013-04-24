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
#include "../../../Application/OgreClock.h" 
#include "../../../Application/BaseApplication.h" 

#include "Logic/Entity/Components/BaseTraveler.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageString.h"

#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"


/*para tener un acceso directo al gamestatus*/
#include "Logic/GameStatus.h"
#include "Logic/RingInfo.h"
#include "Logic/BaseInfo.h"
#include "Logic/PlayerInfo.h"

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
	{	} // ~CBaseTraveler
	
	//---------------------------------------------------------

	bool CBaseTraveler::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
				//_scene=_entity->getMap()->getGraphicScene();
		//lo hago aquí mismo, en algún componente hay que hacerlo y en principio solo los personajes
		//player  (ya sea humano o bot) pueden viajar entre bases.
		
		//_gameStatus=Application::CBaseApplication::getSingletonPtr()->getGameState()->getGameStatus();
			
		_gameStatus=CGameStatus::getSingletonPtr();
		//creamos un altar pasandole la entidad propietaria del presente compontente.
		//IMPORTANTE: Hay que tener en cuenta que una unidad pertenece SIEMPRE a la base en la que nace.
		//también es imporante tener en cuenta, que los arrays _base y _players tienen una corresponendencia de índice
		//es decir, _bases[1] es del jugador 1 que está guardado en _players[1]
		_gameStatus->getPlayer(entity->getLogicalPosition()->getBase())->attachCEntity(_entity);
		
		if(entityInfo->hasAttribute("hero"))
			_gameStatus->getPlayer(entity->getLogicalPosition()->getBase())->setHeroName( entityInfo->getStringAttribute("hero"));		
		
		return true;
	} // spawn
	
	//---------------------------------------------------------

	
	bool CBaseTraveler::accept(const CMessage *message)
	{
		return (isAwake() && !_changingBase && (CRingTraveler::accept(message) || 
					(message->getType() == Message::CONTROL && message->getAction() == Message::SHOW_BASE) || 
					  (message->getType() == Message::CONTROL && message->getAction() == Message::GOBACK_TO_BASE)  || 
					   (message->getType() == Message::CONTROL && message->getAction() == Message::CHANGE_BASE)));
	} // accept
	
	//---------------------------------------------------------

	void CBaseTraveler::process(CMessage *message)
	{
		CRingTraveler::process(message);
		 // TODO FRS esto podría ser char...
		
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::SHOW_BASE)
			{
				CMessageUShort *maux = static_cast<CMessageUShort*>(message);
				
				if (_gameStatus->getNumBases() > maux->getUShort())
					showBase(maux->getUShort());	
				
			}
			if(message->getAction() == Message::GOBACK_TO_BASE)
			{
				CBaseTraveler::returnToPlayerBase();
			}
			if(message->getAction() == Message::CHANGE_BASE)// lo de _changeAllowed no funciona con los bots
			{
				if (_changeAllowed)
				{
					_changingBase=true;
					_entity->getMap()->setVisible(); 
				}
				if (_entity->getName()=="GemeloGreen" || _entity->getName()=="GemeloYellow" /*|| _entity->getName()=="GemeloBlue"*/)
				{
					_changingBase=true;
					CMessageUShort *maux = static_cast<CMessageUShort*>(message);
					_baseToGo = maux->getUShort();

				}
			}
		}
	}

	//---------------------------------------------------------

	void CBaseTraveler::changeBase()
	{
	
		if (_changingBase && !isChangingRing())
		{	
			jumpToBase();
			LOG("EXITO");

			CMessageChar *m = new CMessageChar();	
			m->setType(Message::AVATAR_MOVE);
			m->setAction(Message::CHANGE_BASE);
			m->setChar( _baseToGo - (int) _entity->getLogicalPosition()->getBase() ); // ƒ®§ Enviamos diferencial de base (AVATAR_MOVE es movimiento diferencial)
			_entity->emitMessage(m,this);
			
			LOG("Change Base from " << _entity->getLogicalPosition()->getBase() << " to " << _baseToGo );


			
			CMessageString *m2 = new CMessageString();	
			m2->setType(Message::SET_MATERIAL);
			m2->setString(_entity->getInitialMaterial());
			_entity->emitMessage(m2,this);
		}
		_changingBase=false;
		_changingBaseTime=0;
	}

	
	void CBaseTraveler::changeRing()
	{
		CRingTraveler::timeArrived();
		_changingRingTime=0;
	}

	//---------------------------------------------------------

	void CBaseTraveler::showBase(unsigned short base)
	{
		_changeAllowed = true;
		_baseToGo = base;
	
		Logic::CServer* srv = Logic::CServer::getSingletonPtr();
		srv->activateBaseCam(base);
		LOG("Showing Base " <<  base );
	}

	//---------------------------------------------------------
	void CBaseTraveler::returnToPlayerBase()
	{
		if (_changeAllowed)
		{
			_changeAllowed = false;		
			
			Logic::CServer* srv = Logic::CServer::getSingletonPtr();
			srv->activatePlayerCam();
		}
	}

	//---------------------------------------------------------
	void CBaseTraveler::jumpToBase()
	{
		if (_changeAllowed || !_entity->isPlayer())
		{
			_changeAllowed = false;
			_changingBase=true;
			
			Logic::CServer* srv = Logic::CServer::getSingletonPtr();
			srv->deferredMoveEntity(_entity, _baseToGo);

		}
	}

	//---------------------------------------------------------
			
	void CBaseTraveler::tick(unsigned int msecs)
	{
			CRingTraveler::tick(msecs);
			IComponent::tick(msecs);			
	}

} // namespace Logic

