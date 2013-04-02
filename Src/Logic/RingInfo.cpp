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
#include "Logic\AltarInfo.h"
//using namespace Logic;

namespace Logic
{
	CRingInfo::CRingInfo(const unsigned short numAltars, Ring tipo):_numAltars(numAltars),_AllAltarsActivated(false),_numActivatedAltars(0),_tipo(tipo)
	{/*	
		for(unsigned short i=0; i<_numAltars; i++)//creamos las bases con 3 anillos cada una
		{
		unsigned short altars=5;
			Logic::Ring tipo=Logic::LogicalPosition::CENTRAL_RING;
		
			Logic::CAltarInfo* altar=createAltar();
			if (altar!=0)//los altares Reales deben estar ya creados y con su identificador para crear estos.
				_altars[""]=altar;
		}*/
	}
	
	CRingInfo::~CRingInfo()
	{	
		
	}


	Logic::CAltarInfo* CRingInfo::createAltar(std::string id)
	{
		//assert
		Logic::CAltarInfo* altar=new CAltarInfo(id);
		if (altar)
		{

			return altar;
		}
		return 0;

	}
		
	//---------------------------------------------------------
}