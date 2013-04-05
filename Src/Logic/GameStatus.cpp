//---------------------------------------------------------------------------
// GameStatus.cpp
//---------------------------------------------------------------------------


/**
@file GameStatus.cpp

Contiene la implementación de la clase que maneja las posiciones lógicas

@see Logic::GameStatus

@author Jose Luis López Sánchez
*/

#include "Logic\GameStatus.h"
#include "Logic\BaseInfo.h"
#include "Logic\PlayerInfo.h"

#include <assert.h>
//using namespace Logic;

namespace Logic 
{
	CGameStatus::CGameStatus(const unsigned short numPlayers)
	{

		_numPlayers=numPlayers;
		_numBases=numPlayers+1;//base adicional para Lobby
		
		Logic::CBaseInfo* base=createBase(3); //la base 0 es lobby
			if (base!=0)
				_bases.push_back(base);
			

		for(int i=0; i<_numPlayers; i++)//creamos las bases con 3 anillos cada una
		{
			Logic::CBaseInfo* base=createBase(3); //la base 0 es lobby
			if (base!=0)
				_bases.push_back(base);
			
			Logic::CPlayerInfo* player=createPlayer(Logic::AQUILES,base,NULL); //la base 0 es lobby
			if (player!=0) //el puntero al centity es null porque un componente se encargará de rellenarlo externamente.
				_players.push_back(player);
		}
	}
	
	CGameStatus::~CGameStatus()
	{	
		delete &_bases;	//me teneis que enseñar a destruir bien...		
		delete &_players;
	}
	CBaseInfo* CGameStatus::getBase(unsigned short base)
	{
			return _bases[base];
	}

	CPlayerInfo* CGameStatus::getPlayer(unsigned short player)
	{
			return _players[player];
	}

	//---------------------------------------------------------

	Logic::CBaseInfo* CGameStatus::createBase(const unsigned short rings)
	{		
		assert( &_bases );//&& "LOGIC::GRAPHICS>> No exite lista de bases");

		Logic::CBaseInfo* base=new Logic::CBaseInfo(3);
		if (base)
			return base;

		return 0;
	} // createBase

	Logic::CPlayerInfo* CGameStatus::createPlayer(Logic::Hero heroe, const CBaseInfo* miBase, Logic::CEntity* entity)
	{		
		assert( &_players );//&& "LOGIC::GRAPHICS>> No existe lista de players");

		Logic::CPlayerInfo* player=new Logic::CPlayerInfo(heroe,miBase,entity);
		if (player)
			return player;

		return 0;
	} // createPlayer
}