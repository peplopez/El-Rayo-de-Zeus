//---------------------------------------------------------------------------
// Base.cpp
//---------------------------------------------------------------------------


/**
@file Base.cpp

Contiene la implementación de la clase que maneja las posiciones lógicas

@see Logic::Base

@author Jose Luis López Sánchez
*/

#include "Logic\Base.h"
//using namespace Logic;

namespace Logic
{
	CBase::CBase(const unsigned short numRings):_numRings(numRings),_AllAltarsActivated(false),_numActivatedAltars(0),_life(1),_numAltars(11)
	{		
		//_rings=new CRingnumPlayers;
		//_numBases=numPlayers+1;
	}
	
	CBase::~CBase()
	{	
		//delete &_bases;	//me teneis que enseñar a destruir bien...		
		//delete &_players;
	}

	//---------------------------------------------------------

	


}