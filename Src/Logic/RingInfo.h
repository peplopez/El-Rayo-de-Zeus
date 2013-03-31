//---------------------------------------------------------------------------
// RingInfo.h
//---------------------------------------------------------------------------

/**
@file RingInfo.h

Esta es la clase principal de gestión del juego. Contiene clases como son Bases, Players, Stats, etc.
@see Logic::RingInfo

@author Jose Luis López Sánchez
*/
#pragma once
#ifndef __Logic_RingInfo_H
#define __Logic_RingInfo_H

#include <string>
#include <map>

#include "Logic\Entity\LogicalPosition.h"
namespace Logic
{
	class CAltarInfo;
	//class CPlayerInfo;
}
namespace Logic
{
	class CRingInfo
	{
	public:
		CRingInfo(const unsigned short numAltars, Ring tipo);

		~CRingInfo();
		/**
			Para obtener el número de defensores en el anillo
		*/
		unsigned int getDefenders();

		/**
			Para obtener el número de atacantes en el anillo. Así saber que el anillo está calentito.
		*/
		unsigned int getAttakers();

		/**
			Número de altares en el anillo
		*/
		const unsigned short _numAltars;

		/**
			getters y setters
		*/
		unsigned short getNumActivatedAltars(){return _numActivatedAltars;}

		void setNumActivatedAltars(const unsigned short numActivatedAltars){_numActivatedAltars=numActivatedAltars;}
	
		bool getAllAltarsActivated(){return _AllAltarsActivated;}

		void setAllAltarsActivated(const bool allAltarsActivated){_AllAltarsActivated=allAltarsActivated;}
	
	protected:
		
		/**
			Crea los altares
		*/
		Logic::CAltarInfo* CRingInfo::createAltar();
		
		/**
			Id enumerado que dice que tipo de anillo soy
		*/
		const Ring _tipo;
				
		/**
			Número de altares activados
		*/
		unsigned short _numActivatedAltars;
		
		/**
			Anillo Activado (todos sus altares activados)
		*/
		bool _AllAltarsActivated;

		/**
		Tipo para la lista de altares
		*/
		typedef std::map<std::string,CAltarInfo*> TAltars;
		
		/**
		Lista de los altares
		*/
		TAltars _altars;
	};
}

#endif //namespace Logic