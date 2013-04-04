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


namespace Logic
{
	CPlayerInfo::CPlayerInfo(Hero heroe,const CBaseInfo* miBase,Logic::CEntity* entity):_player(entity),_miBase(miBase),_basesActivated(0),
		_altarsActivated(0),_heroesKilled(0),_creaturesKilled(0),_meritPoints(0),_myDeaths(0),
		_hasThreeCombos(true),_meritPointsSpent(0),_lostCreatures(0),_hero(heroe)
	{
		_playerName="Hercules"; //dependerá del enumerado Hero
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


}