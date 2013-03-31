//---------------------------------------------------------------------------
// AltarInfo.h
//---------------------------------------------------------------------------

/**
@file AltarInfo.h

Clase que guarda información sobre el altar
@see Logic::AltarInfo

@author Jose Luis López Sánchez
*/
#pragma once
#ifndef __Logic_RingInfo_H
#define __Logic_RingInfo_H

#include <string>
#include <list>

#include "Logic\Entity\LogicalPosition.h"
namespace Logic
{
	class CEntity;
	class CPlayer;

}
namespace Logic
{
	class CAltarInfo
	{
	public:
		CAltarInfo(const unsigned short numAltars, Ring tipo);

		~CAltarInfo();
		
		/**
			 Para saber quien es el que está usando el altar
		*/
		CPlayer* getPlayer();

		/**
			 Para notificar quien es el que está usando el altar
		*/
		void setPlayer(const CPlayer* playerChanging);

		//quizá queramos poner un metodo que te devuelva quien es el dueño del altar.

		/**
			getters y setters
		*/

		
	protected:
		
		/**
			Flag de activado
		*/
		bool _activated;

		/**
			Indica si está en tránsito de activación
		*/
		bool _toActivation;
		
		
		/**
			Indica si está en tránsito de desactivación
		*/
		bool _toDesActivation;
		
		/**
			Puntero al CEntity que representa éste altar lógico.
		*/		
		Logic::CEntity* _cAltar;

	};
}

#endif //namespace Logic