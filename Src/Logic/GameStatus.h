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
#ifndef __Logic_GameStatus_H
#define __Logic_GameStatus_H

#include <list>
namespace Logic
{
	class CBaseInfo;
	class CPlayerInfo;
}
namespace Logic
{
	class CGameStatus
	{
	public:
		CGameStatus(const unsigned short numPlayers);

		~CGameStatus();

		//opciones que van a ir a Ranking
		/**
			Tiempo programado como necesario en la partida creada
			para que el altar cambie desde activado a desactivado
			y viceversa.
			Se asume que es el mismo para todos los altares de la 
			partida y que este tiempo jamás cambia.
		*/
		//const unsigned long _activationTime;

		/**
			Puntos de mérito que supone activar un altar.
		*/
		//const unsigned int _scoreAltar;
		
	protected:
	
		/**
			crea las bases
		*/
		Logic::CBaseInfo* createBase(const unsigned short rings);

		/**
			Número de jugadores
		*/
		unsigned short _numPlayers;
		/**
			Número de bases, pueden ser diferentes, caso de la base Lobby
		*/
		unsigned short _numBases; 

		/**
		Tipo para la lista de Bases
		*/
		typedef std::list<CPlayerInfo*> TPlayers;

		/**
		Lista de las Bases
		*/
		TPlayers _players;

		/**
		Tipo para la lista de Bases
		*/
		typedef std::list<CBaseInfo*> TBases;

		/**
		Lista de las Bases
		*/
		TBases _bases;
	};
//TBases _bases;
}

#endif //namespace Logic