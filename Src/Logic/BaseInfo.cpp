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
		_rings.clear();
	}

	//---------------------------------------------------------
	bool CBaseInfo::getAllAltarsActivated()
	{
		return _AllAltarsActivated;		
	}

	void CBaseInfo::updateAllAltarsActivated()
	{
		_AllAltarsActivated=false;
		unsigned int counter=0;
		if(this->getRing(Logic::LogicalPosition::LOWER_RING)->getAllAltarsActivated())
		{
			std::cout<<"APPLICATION::GAMESTATE::RAYAZO EN ANILLO 0"<<std::endl;
			counter++;
		}	
		if(this->getRing(Logic::LogicalPosition::CENTRAL_RING)->getAllAltarsActivated())
		{
			std::cout<<"APPLICATION::GAMESTATE::RAYAZO EN ANILLO 1"<<std::endl;
			counter++;
		}
		if(this->getRing(Logic::LogicalPosition::UPPER_RING)->getAllAltarsActivated())
		{
			std::cout<<"APPLICATION::GAMESTATE::RAYAZO EN ANILLO 2"<<std::endl;
			counter++;
		}
		if (counter==3)
			_AllAltarsActivated=true;
	}

	unsigned short CBaseInfo::getNumActivatedAltars()
	{
		_numActivatedAltars=0;
		_numActivatedAltars=getRing(0)->getNumActivatedAltars()
										+getRing(1)->getNumActivatedAltars()
										+getRing(2)->getNumActivatedAltars();
		return _numActivatedAltars;
	}


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