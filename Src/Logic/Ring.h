//---------------------------------------------------------------------------
// Ring.h
//---------------------------------------------------------------------------

/**
@file Ring.h

Esta es la clase principal de gestión del juego. Contiene clases como son Bases, Players, Stats, etc.
@see Logic::Ring

@author Jose Luis López Sánchez
*/
#pragma once
#ifndef __Logic_Ring_H
#define __Logic_Ring_H

#include <string>
#include <list>

#include "Logic\Entity\LogicalPosition.h"
namespace Logic
{
	class CAltar;
	//class CPlayer;
}
namespace Logic
{
	class CRing
	{
	public:
		CRing(const unsigned short numAltars, Ring tipo);

		~CRing();
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
		Tipo para la lista de Anillos
		*/
		typedef std::list<CAltar*> TAltars;
		
		/**
		Lista de los anillos
		*/
		TAltars _altars;
	};
}

#endif //namespace Logic