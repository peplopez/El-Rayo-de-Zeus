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
#ifndef __Logic_AltarInfo_H
#define __Logic_AltarInfo_H

#include <string>
#include <list>

#include "Logic\Entity\LogicalPosition.h"
namespace Logic
{
	class CEntity;
	class CPlayerInfo;

}
namespace Logic
{
	class CAltarInfo
	{
	public:
		CAltarInfo(std::string id):_altar(NULL)// Esto es así hasta saber como será la escena que emilio está haciendo.
		{}

		~CAltarInfo(){}
		
		/**
			 Para saber quien es el que está usando el altar
		*/
		CPlayerInfo* getPlayer();

		/**
			 Para notificar quien es el que está usando el altar
		*/
		void setPlayer(const CPlayerInfo* playerChanging);

		//quizá queramos poner un metodo que te devuelva quien es el dueño del altar.

		//void setEntity();

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
		Logic::CEntity* _altar;

	};
}

#endif //namespace Logic