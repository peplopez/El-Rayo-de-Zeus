//---------------------------------------------------------------------------
// AltarInfo.cpp
//---------------------------------------------------------------------------

/**
@file AltarInfo.cpp

Contiene la implementación de la clase AltarInfo

@see Logic::AltarInfo

@author Jose Luis López Sánchez
*/

#include "Logic\AltarInfo.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\Altar.h"
namespace Logic
{
	CAltarInfo::CAltarInfo(Logic::CEntity* entity):_altar(entity),_activated(false),_toActivation(false),_toDesActivation(false)// Esto es así hasta saber como será la escena que emilio está haciendo.
	{		
	}

	bool CAltarInfo::isActivated()
	{
		if (_altar) //si hay un puntero valido a un altar
			if (_altar->getComponent<CAltar>()!=NULL)
				_activated=_altar->getComponent<CAltar>()->isActivated();
		return _activated;
	}	
}