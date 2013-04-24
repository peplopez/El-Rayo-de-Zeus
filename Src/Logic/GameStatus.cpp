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
	CGameStatus* CGameStatus::_instance = 0;

	CGameStatus::CGameStatus(const unsigned short numPlayers)
	{
		_instance=this;
		_numPlayers=numPlayers;
		_numBases=numPlayers+1;//base adicional para Lobby
		
		for(int i=0; i<=_numPlayers; i++)//creamos las bases con 3 anillos cada una
		{
			Logic::CBaseInfo* base=createBase(3); //la base 0 es lobby
			if (base!=0)
				_bases.push_back(base);
			
			Logic::CPlayerInfo* player=createPlayer(base,NULL); //la base 0 es lobby
			if (player!=0) //el puntero al centity es null porque un componente se encargará de rellenarlo externamente.
				_players.push_back(player);
		}
	}
	
	CGameStatus::~CGameStatus()
	{	
		_instance=0;
		_bases.clear();
		_players.clear();
	}

	bool CGameStatus::Init(const unsigned short numPlayers)
	{
		assert(!_instance && "Segunda inicialización de Logic::CGameStatus no permitida!");

		new CGameStatus(numPlayers);

		return true;
	} // Init

	//--------------------------------------------------------

	void CGameStatus::Release()
	{
		assert(_instance && "Logic::CGameStatus no está inicializado!");

		if(_instance)
			delete _instance;

	} // Release

	CBaseInfo* CGameStatus::getBase(unsigned short base)
	{
		
		if (_bases.empty()) return NULL;
		return _bases[base];
	}

	CPlayerInfo* CGameStatus::getPlayer(unsigned short player)
	{
		if (_players.empty()) return NULL;
		return _players[player];
	}
	
	Logic::CBaseInfo* CGameStatus::createBase(const unsigned short rings)
	{		
		assert( &_bases );//&& "LOGIC::GRAPHICS>> No exite lista de bases");

		Logic::CBaseInfo* base=new Logic::CBaseInfo(3);
		if (base)
			return base;

		return 0;
	} // createBase

	Logic::CPlayerInfo* CGameStatus::createPlayer(const CBaseInfo* miBase, Logic::CEntity* entity)
	{		
		assert( &_players );//&& "LOGIC::GRAPHICS>> No existe lista de players");

		Logic::CPlayerInfo* player=new Logic::CPlayerInfo(miBase,entity);
		if (player)
			return player;

		return 0;
	} // createPlayer

	//PT
	unsigned short CGameStatus::getEnemiesInBase(unsigned short base)
	{
		unsigned short enemies = 0;
		for(std::vector<CPlayerInfo*>::iterator itPlayers = _players.begin();
			itPlayers != _players.end(); itPlayers++)
		{
			CPlayerInfo* player = (*itPlayers);

			//para evitar el primer player que no es nadie
			if(player->getPlayer() != NULL)
			{
				//Si el player no soy yo, y el player esta en mi base aumento el numero de players enemigos
				if( (!player->inMyBase()) &&  (player->inBase(base)) )
					enemies++;
			}

		}

		return enemies;
	}
}