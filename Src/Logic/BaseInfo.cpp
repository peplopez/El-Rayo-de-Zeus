//---------------------------------------------------------------------------
// BaseInfoInfo.cpp
//---------------------------------------------------------------------------


/**
@file BaseInfo.cpp

Contiene la implementación de la clase que maneja las posiciones lógicas

@see Logic::BaseInfo

@author Jose Luis López Sánchez
*/

#include "Logic\BaseInfo.h"
#include "Logic\RingInfo.h"

namespace Logic
{
	CBaseInfo::CBaseInfo(const unsigned short numRings):_numRings(numRings),_AllAltarsActivated(false),_numActivatedAltars(0),_life(1),_numAltars(11)
	{		
		//_rings=new CRingnumPlayers;
		//_numBases=numPlayers+1;

		for(unsigned short i=0; i<_numRings; i++)//creamos las bases con 3 anillos cada una
		{
			unsigned short altars =5;
			Logic::Ring tipo=Logic::LogicalPosition::CENTRAL_RING;
			if (i==0)
			{
				altars=3;
				tipo=Logic::LogicalPosition::LOWER_RING;
			}
			if (i==2)
			{
				altars=3;
				tipo=tipo=LogicalPosition::UPPER_RING;			
			}
			Logic::CRingInfo* ring=createRing(altars,tipo);
			if (ring!=0)	
				_rings.push_back(ring);
		}

	}
	
	CBaseInfo::~CBaseInfo()
	{	
		//delete &_bases;	//me teneis que enseñar a destruir bien...		
		//delete &_players;
	}

	//---------------------------------------------------------

	CRingInfo* CBaseInfo::getRing(unsigned short ring)
		{
			return _rings[ring];
		}

	Logic::CRingInfo* CBaseInfo::createRing(const unsigned short numAltars, Ring tipo)
	{		
		//assert( &_bases );//&& "LOGIC::GRAPHICS>> No existe escena gráfica!");

		Logic::CRingInfo* ring=new CRingInfo(numAltars, tipo);
		if (ring)
			return ring;

		return 0;
	} // createBase

}