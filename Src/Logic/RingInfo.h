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
#include "Logic\Maps\EntityID.h"
namespace Logic
{
	class CAltarInfo;
	class CEntity;
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
		unsigned short getNumActivatedAltars();

		void setNumActivatedAltars(const unsigned short numActivatedAltars){_numActivatedAltars=numActivatedAltars;}
	
		bool areAllAltarsActivated();

		void setAllAltarsActivated(const bool allAltarsActivated){_AllAltarsActivated=allAltarsActivated;}
	
		/*CAltarInfo* getAltar(string altar)
		{
			return _altars[altar];
		}*/
	protected:
		
		friend class CAltar;
		/**
			Crea los altares
		*/
		Logic::CAltarInfo* CRingInfo::createAltar(Logic::CEntity* entity);
		
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
		typedef std::map<unsigned int,CAltarInfo*> TAltars;
		
		/**
		Lista de los altares
		*/
		TAltars _altars;
	};
}

#endif //namespace Logic