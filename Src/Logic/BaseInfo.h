//---------------------------------------------------------------------------
// BaseInfo.h
//---------------------------------------------------------------------------

/**
@file BaseInfo.h

Esta es la clase que representa una base y que contiene anillos. Contiene entre otras
cosas un puntero al dueño de la base (CPlayerInfo, lista de CRingInfo).
@see Logic::GameStatus

@author Jose Luis López Sánchez
*/
#pragma once
#ifndef __Logic_BaseInfo_H
#define __Logic_BaseInfo_H

#include "Logic\Entity\LogicalPosition.h"
#include <string>
#include <vector>
namespace Logic
{
	class CRingInfo;
	class CPlayerInfo;
}
namespace Logic
{
	/**
		Cada base debe tener un color asociado. Esto no quiere decir que la base vaya a ser de ese color. 
		Simplemente que sus altares, su iluminación emite ese color. Y quizá, que los heroes tien shaders
		de ese color. Por ejemplo si dos jugadores decien cogerse el mismo personaje cada uno será del color
		asociado a la base.
	*/
	enum Color : unsigned short
		{	
			WHITE	=0,
			RED		=1,
			BLUE	=2,
			GREEN	=3,
			YELLOW	=4,
			BLACK	=5, //Suponiendo que sea un color válido, ahora por el fondo no lo es
			ORANGE	=6,
			VIOLET	=7,
			BROWN	=8
		};

	class CBaseInfo
	{
	public:
		CBaseInfo(const unsigned short numRings);	//CONSTRUIR TAMBIÉN CON EL COLOR.

		~CBaseInfo();
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
	
		bool getAllAltarsActivated();

		//void setAllAltarsActivated(const bool allAltarsActivated){_AllAltarsActivated=allAltarsActivated;}
		void updateAllAltarsActivated();
		CRingInfo* getRing(unsigned short ring);

	protected:
		
		/**
			crea los anillos
		*/
		Logic::CRingInfo* createRing(const unsigned short altars, Ring tipo);
				
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
		typedef std::vector<CRingInfo*> TRings;
		
		/**
		Lista de los anillos
		*/
		TRings _rings;
	};
}

#endif //namespace Logic