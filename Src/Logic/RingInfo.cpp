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
#include "Logic\Entity\Entity.h"

namespace Logic
{
	class CEntity;
}

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
		_altars.clear();
	//	delete &_altars;
	}

	void CRingInfo::updateAllAltarsInfo()
	{
		 TAltars::const_iterator itr;
		 _numActivatedAltars=0;
		 for(itr = _altars.begin(); itr != _altars.end(); ++itr){
			 if ((*itr).second->isActivated())
				 _numActivatedAltars++;	
		 }
		 _numActivatedAltars >= _numAltars ? true : false;
	}

	unsigned short CRingInfo::getNumActivatedAltars()
	{
		 return _numActivatedAltars;
	}

	bool CRingInfo::areAllAltarsActivated()
	{
		return _AllAltarsActivated;
	}

	Logic::CAltarInfo* CRingInfo::createAltar(Logic::CEntity* entity)
	{
		Logic::CAltarInfo* altar=new CAltarInfo(entity);
		if (altar)
		{
			_altars[altar->getEntity()->getEntityID()]=altar;
			return altar;
		}
		return 0;
	}
}