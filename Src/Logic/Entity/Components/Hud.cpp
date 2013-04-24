/***
@file HudCegui.cpp

Contiene la implementación del componente que controla el HUD mediante CEGUI.
 
@see Logic::CHud
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/

#include "Hud.h"

//Inclusion de los mensajes
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageBool.h"
#include "Logic/Entity/Messages/MessageInt.h"

#include "Map/MapEntity.h"

#include "Logic/GameStatus.h"
#include "Logic/BaseInfo.h"
#include "Logic/RingInfo.h"
#include "Logic/PlayerInfo.h"

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"


namespace Logic 
{
	IMP_FACTORY(CHud);
	
	//---------------------------------------------------------
		
	CHud::~CHud() {	 	
	} // destructor

	//---------------------------------------------------------
	
	bool CHud::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		numBase = entity->getOriginBase();

		player = numBase;

		//Get CGameStatus Singleton to recover info to show in the HUD
		_gameStatus = Logic::CGameStatus::getSingletonPtr();

		//recover the maximun number of altars of the base. Its better than make the call
		// in tick method
		numMaxAltares = _gameStatus->getBase(numBase)->_numAltars;

		//init HUD Layout and functions
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("Hud");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initHud");

		return true;

	} // spawn


	//---------------------------------------------

	bool CHud::activate()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showHud");
		return true;
	} //activate

	//---------------------------------------------

	void CHud::deactivate()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideHud");
	}//deactivate
	
	//---------------------------------------------------------

	bool CHud::accept(const CMessage *message)
	{
		return message->getType() == Message::HUD || message->getType() == Message::CONTROL || message->getType() == Message::AVATAR_MOVE;
		//return true;
	} // accept
	
	//---------------------------------------------------------

	void CHud::process(CMessage *message)
	{

			switch(message->getType())
			{
				/*case Message::CONTACT:
						hudLife(message._float);*/
				case Message::HUD:
					//CMessageFloat *maux = static_cast<CMessageFloat*>(message);
					if(message->getAction() == Message::DISPLAY_HUD)
						displayHud();
					/*if(!message._string.compare("addPlayerToBase"))
						hudPlayers(1);
					if(!message._string.compare("minusPlayerToBase"))
						hudPlayers(-1);
						*/
					break;
				/*case Message::AVATAR_MOVE:
					if(message->getAction() == Message::CHANGE_BASE)
						if(static_cast<CMessageChar*>(message)->getChar()==numBase)
						{
							ScriptManager::CServer::getSingletonPtr()->executeProcedure("incrementEnemiesInBase");
						}
						else
						{
							ScriptManager::CServer::getSingletonPtr()->executeProcedure("decrementEnemiesInBase");
						}
					break;
					*/
			}

	} // process

	void CHud::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		_time += msecs;

		//se recupera la informacion de Logic::CGameStatus cada medio segundo para actualizar el HUD
		if(_time >= 500)
		{
			_time = 0;
			//parametros: 
			//primero: la cadena con el procedimiento de LUA a ejecutar : showAltarsActivated
			//segundo: el numero de altares activados
			//tercero: el numero maximo de altares
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showAltarsActivated",_gameStatus->getBase(numBase)->getNumActivatedAltars(),numMaxAltares);

			//parametros: 
			//primero: la cadena con el procedimiento de LUA a ejecutar : showAltarsActivated
			//segundo: las vidas de la base
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showBaseLife",_gameStatus->getBase(numBase)->getBaseLife());

			//Enemies number / players that are not me in my origin base

			//ScriptManager::CServer::getSingletonPtr()->executeProcedure("showEnemiesInBase",_gameStatus->getBase(numBase)->getAttakers());
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showEnemiesInBase",_gameStatus->getEnemiesInBase(numBase));
			//ScriptManager::CServer::getSingletonPtr()->executeProcedure("showEnemiesInBase",ScriptManager::CServer::getSingletonPtr()->getGlobal("numberEnemiesInBase",0));

			//Merite Points
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showPM",_gameStatus->getPlayer(player)->getMeritPoints());
		}
		
	}//tick

	void CHud::displayHud()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("changeHudVisibility");
	}

} // namespace Logic



