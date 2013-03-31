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
#include "Logic\Ring.h"

//using namespace Logic;

namespace Logic
{
	CBase::CBase(const unsigned short numRings):_numRings(numRings),_AllAltarsActivated(false),_numActivatedAltars(0),_life(1),_numAltars(11)
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
			Logic::CRing* ring=createRing(altars,tipo);
			if (ring!=0)	
				_rings.push_back(ring);
		}

	}
	
	CBase::~CBase()
	{	
		//delete &_bases;	//me teneis que enseñar a destruir bien...		
		//delete &_players;
	}

	//---------------------------------------------------------

	
	Logic::CRing* CBase::createRing(const unsigned short numAltars, Ring tipo)
	{		
		//assert( &_bases );//&& "LOGIC::GRAPHICS>> No existe escena gráfica!");

		Logic::CRing* ring=new CRing(numAltars, tipo);
		if (ring)
			return ring;

		return 0;
	} // createBase

}