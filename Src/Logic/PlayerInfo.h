//---------------------------------------------------------------------------
// PlayerInfo.h
//---------------------------------------------------------------------------

/**
@file PlayerInfo.h

Esta es la clase principal de gestión del juego. Contiene clases como son Bases, Players, Stats, etc.
@see Logic::PlayerInfo

@author Jose Luis López Sánchez
*/
#pragma once
#ifndef __Logic_PlayerInfo_H
#define __Logic_PlayerInfo_H

//#include "Logic\Entity\LogicalPosition.h"
#include <string>

using namespace std;
namespace Logic
{
	class CBaseInfo;
	class CPlayerInfo;
}
namespace Logic
{
	/**
		En esta clase se guardan también datos estadísticos del jugador. Esos datos pueden parecer completamente irrelevantes. 
		Pero más adelante es posible que sí que sea interesante saber cosas como quien es el que más altares lleva activados,
		quien ha matado a mas enemigos, quien es el que más viaja... ¿Desbloquear logros?...recompensas...
	*/
	class CPlayerInfo
	{
	public:
		CPlayerInfo(const CBaseInfo* miBase);	//CONSTRUIR TAMBIÉN CON EL COLOR.

		~CPlayerInfo();
		/**
			Consulta directamente a su componente CLife para obtener getLife() y getLifeMax()
		*/
		int getLife();
		int getLifeMax();

		/**
		Devuelve si está envenenado, aturdido, convertido en piedra...
		@return quizá no sea un int y sí un enumerado.
		*/
		int getHealthStatus();

		
		
	protected:
		
		/**
			crea los anillos
		*/
		Logic::CRingInfo* createRing(const unsigned short altars, Ring tipo);
		/**
			Puntero al Player dueño de la base.
		*/
		const CBaseInfo* _miBase;
		
		/**
			Número de bases que el Player ha activado
		*/
		unsigned int _basesActivated;

		/**
			Número de altares que el Player ha activado
		*/
		unsigned int _altarsActivated;

		/**
			Número de heroes que con afilado cobre hemos dado muerte
		*/
		unsigned int _heroesKilled;  

		/**
			Número de criaturas matadas
		*/
		unsigned int _creaturesKilled;  

		/**
			Puntos de mérito acumulados
		*/
		unsigned long _meritPoints;

		/**
			Número de fallecimientos
		*/
		unsigned int _myDeaths;
		
		/**
		Nombre del heroe: Hercules, Aquiles...
		*/
		string _heroName;

		/**
		El tercer combo se compraba, puede no tenerlo de inicio, aunque de momento no le impediremos que lo tenga
		de inicio.
		*/
		bool _hasThreeCombos;

		/**
		Puntos de mérito gastados en la tienda
		*/
		unsigned long _meritPointsSpent;

		/**
		Criaturas que nos han matado
		*/
		unsigned int _lostCreatures;

		/**
		Nombre del jugador, en un futuro veo más posible que esto sea un puntero a algo como CJugador.
		Un objeto con datos como email, nick, password...
		*/
		string _playerName;

	};
}

#endif //namespace Logic