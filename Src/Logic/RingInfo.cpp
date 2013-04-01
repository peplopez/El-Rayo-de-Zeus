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
	{
		for(unsigned short i=0; i<_numAltars; i++)//creamos las bases con 3 anillos cada una
		{
			unsigned short altars=5;
			Logic::Ring tipo=Logic::LogicalPosition::CENTRAL_RING;
			/*if (i==0)
			{
				altars=3;
				tipo=Logic::LogicalPosition::LOWER_RING;
			}
			if (i==2)
			{
				altars=3;
				tipo=tipo=LogicalPosition::UPPER_RING;			
			}*/
			Logic::CAltarInfo* altar=createAltar();
			if (altar!=0)//los altares Reales deben estar ya creados y con su identificador para crear estos.
				_altars[""]=altar;
		}
	}
	
	CRingInfo::~CRingInfo()
	{	
		
	}


	Logic::CAltarInfo* CRingInfo::createAltar()
	{
		//assert
		Logic::CAltarInfo* altar=new CAltarInfo();
		if (altar)
			return altar;

		return 0;

	}
		
	//---------------------------------------------------------
}