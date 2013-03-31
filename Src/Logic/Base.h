//---------------------------------------------------------------------------
// GameStatus.h
//---------------------------------------------------------------------------

/**
@file GameStatus.h

Esta es la clase principal de gestión del juego. Contiene clases como son Bases, Players, Stats, etc.
@see Logic::GameStatus

@author Jose Luis López Sánchez
*/
#pragma once
#ifndef __Logic_Base_H
#define __Logic_Base_H

#include "Logic\Entity\LogicalPosition.h"
#include <string>
#include <list>
namespace Logic
{
	class CRing;
	class CPlayer;
}
namespace Logic
{
	class CBase
	{
	public:
		CBase(const unsigned short numRings);

		~CBase();
		/**
			Para obtener el número de defensores de la base.
		*/
		unsigned int getDefenders();

		/**
			Para obtener el número de atacantes en la base. Así saber que la base está calentita.
		*/
		unsigned int getAttakers();
				
		/**
			Número de altares
		*/
		const unsigned short _numAltars;

		/**
			Número de anillos
		*/
		const unsigned short _numRings;  //ya se que siempre serán 3, pero por si acaso.

		/**
			getters y setters
		*/
		unsigned short getNumActivatedAltars(){return _numActivatedAltars;}

		void setNumActivatedAltars(const unsigned short numActivatedAltars){_numActivatedAltars=numActivatedAltars;}
	
		bool getAllAltarsActivated(){return _AllAltarsActivated;}

		void setAllAltarsActivated(const bool allAltarsActivated){_AllAltarsActivated=allAltarsActivated;}
		
	protected:
		
		/**
			crea los anillos
		*/
		Logic::CRing* CBase::createRing(const unsigned short altars, Ring tipo);
		/**
			Puntero al Player dueño de la base.
		*/
		const CPlayer* _player;
				
		/**
			Número de altares activados
		*/
		unsigned short _numActivatedAltars;
		
		/**
			Base Activada (todos sus altares activados
		*/
		bool _AllAltarsActivated;

		/**
			Vida, numero de rayos que tiene, de 0 a 3
		*/
		unsigned short _life;
		
		/**
		Tipo para la lista de Anillos
		*/
		typedef std::list<CRing*> TRings;
		
		/**
		Lista de los anillos
		*/
		TRings _rings;
	};
}

#endif //namespace Logic