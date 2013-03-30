//---------------------------------------------------------------------------
// GameStatus.cpp
//---------------------------------------------------------------------------


/**
@file GameStatus.cpp

Contiene la implementación de la clase que maneja las posiciones lógicas

@see Logic::LogicalPosition

@author Jose Luis López Sánchez
*/

#include "Logic\GameStatus.h"
//using namespace Logic;

namespace Logic 
{
	CGameStatus::CGameStatus(const unsigned short numPlayers)
	{		
		_numPlayers=numPlayers;
		_numBases=numPlayers+1;
		//_bases=new Bases(_numBases);
		//_players=new Players(_numPlayers);
	}
	
	CGameStatus::~CGameStatus()
	{	
		delete &_bases;	//me teneis que enseñar a destruir bien...		
		delete &_players;
	}

	//---------------------------------------------------------

	


}