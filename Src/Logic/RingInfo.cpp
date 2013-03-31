//---------------------------------------------------------------------------
// RingInfo.cpp
//---------------------------------------------------------------------------


/**
@file RingInfo.cpp

Contiene la implementación de la clase RingInfo

@see Logic::RingInfo

@author Jose Luis López Sánchez
*/

#include "Logic\RingInfo.h"
//using namespace Logic;

namespace Logic
{
	CRingInfo::CRingInfo(const unsigned short numAltars, Ring tipo):_numAltars(numAltars),_AllAltarsActivated(false),_numActivatedAltars(0),_tipo(tipo)
	{
		//_rings=new CRingnumPlayers;
		//_numBases=numPlayers+1;
	}
	
	CRingInfo::~CRingInfo()
	{	
		//delete &_bases;	//me teneis que enseñar a destruir bien...		
		//delete &_players;
	}


	//---------------------------------------------------------
}