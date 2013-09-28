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

	CGameStatus::CGameStatus(unsigned int numPlayers) : _numPlayers(numPlayers)
	{
		_instance=this;

		_numBases = _numPlayers+1;//base adicional para Lobby
		
		for(int i=0; i<=_numPlayers; ++i)//creamos las bases con 3 anillos cada una
		{
			Logic::CBaseInfo* base=createBase(3,3,0); //la base 0 es lobby (rings, lifeBase, colorBase)
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


	bool CGameStatus::Init(unsigned int numPlayers)
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
	
	Logic::CBaseInfo* CGameStatus::createBase(const unsigned short rings, const unsigned short lifeBase, const unsigned short colorBase)
	{		
		assert( &_bases );//&& "LOGIC::GRAPHICS>> No exite lista de bases");

		//PT
		//Logic::CBaseInfo* base=new Logic::CBaseInfo(3);
		Logic::CBaseInfo* base=new Logic::CBaseInfo(rings, lifeBase, colorBase);
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
		//set enemies number 0 before calculating it
		_enemies = 0;

		for(std::vector<CPlayerInfo*>::const_iterator itPlayers = _players.cbegin();
			itPlayers != _players.cend(); itPlayers++)
		{
			CPlayerInfo* player = (*itPlayers);

			//para evitar el primer player que no es nadie
			if(player->getPlayer() != NULL)
			{
				// If player its not me and player is in my base
				//and that player is not dead, it is added enemies number
				if( (!player->inMyBase()) &&  (player->inBase(base)) && (player->getLife() > 0) )
					_enemies++;

			}

		}

		return _enemies;
	}
}