//---------------------------------------------------------------------------
// PlayerInfo.cpp
//---------------------------------------------------------------------------


/**
@file PlayerInfo.cpp

Contiene la implementación de la clase que conoce los datos relativos a las bases.
Datos en cuanto a IA, percepción y gestión del juego.

@see Logic::PlayerInfo

@author Jose Luis López Sánchez
*/

#include "Logic\PlayerInfo.h"
#include "Logic\BaseInfo.h"
#include "Logic\Entity\Components\Life.h"
#include "Logic\Entity\Entity.h"

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"


namespace Logic
{
	CPlayerInfo::CPlayerInfo(const CBaseInfo* miBase, Logic::CEntity* entity):_player(entity),_miBase(miBase),_basesActivated(0),
		_altarsActivated(0),_heroesKilled(0),_creaturesKilled(0),_meritPoints(0),_myDeaths(0),
		_hasThreeCombos(true),_meritPointsSpent(0),_lostCreatures(0),_playerName("")
	{

	}

	CPlayerInfo::~CPlayerInfo()
	{	
		delete this;	//me teneis que enseñar a destruir bien...		
	}

	//---------------------------------------------------------
	int CPlayerInfo::getLife()
	{
		if (_player)
			if (_player->getComponent<CLife>()!=NULL)
				return _player->getComponent<CLife>()->getLife();
		return NULL;
	}

	int CPlayerInfo::getLifeMax()
	{
		if (_player)
			if (_player->getComponent<CLife>()!=NULL)
				return _player->getComponent<CLife>()->getLifeMax();
		return NULL;
	}
	
	bool CPlayerInfo::inMyBase()
	{//quiero que dependiendo de si el bot está en su propia base o en otra tenga diferente comportamiento.
		if(_player)
		{
			if (_player->getLogicalPosition()->getBase()==_player->getOriginBase())
				setInMyBase(true);
			else
				setInMyBase(false);
			return _inMyBase;
		}
		else
		{
			return false;
		}
			
	}

	//PT
	bool CPlayerInfo::inBase(unsigned short base)
	{
		if (_player->getLogicalPosition()->getBase()==base)
			return true;
		else
			return false;
	}

	void CPlayerInfo::increaseAltarsActivated()
	{
		_altarsActivated++;
		//incPM es una variable global de LUA, para que el incremento de puntos de merito
		//este scriptado y no sea una variable de C++ que haya que compilar de nuevo
		_meritPoints+=ScriptManager::CServer::getSingletonPtr()->getGlobal("incPM",100);
	}

	void CPlayerInfo::decreaseAltarsActivated()
	{
		_altarsActivated--;
		//incPM es una variable global de LUA, para que el incremento de puntos de merito
		//este scriptado y no sea una variable de C++ que haya que compilar de nuevo
		_meritPoints+=ScriptManager::CServer::getSingletonPtr()->getGlobal("incPM",100);
	}

}