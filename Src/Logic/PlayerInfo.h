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
#include <string>

using namespace std;

namespace Logic
{
	class CBaseInfo;
	class CPlayerInfo;
	class CEntity;
}
namespace Logic
{
	/**
		En esta clase se guardan también datos estadísticos del jugador. Esos datos pueden parecer completamente irrelevantes. 
		Pero más adelante es posible que sí que sea interesante saber cosas como quien es el que más altares lleva activados,
		quien ha matado a mas enemigos, quien es el que más viaja... ¿Desbloquear logros?...recompensas...
	*/
	enum Hero : unsigned short
		{	
			AQUILES	=0,
			ATALANTA=1,
			HECTOR	=2,
			HERCULES=3,
			HIPOLITA=4,
			PERSEO	=5,
			TESEO	=6, 
			ULISES	=7
		};

	class CPlayerInfo
	{
	public:
		CPlayerInfo(Hero heroe,const CBaseInfo* miBase,Logic::CEntity* entity);	//CONSTRUIR TAMBIÉN CON EL COLOR.

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
		
		/**
		getters y setters
		*/
		unsigned int getBasesActivated()
		{
			return _basesActivated;
		}
		
		unsigned int getAltarsActivated()
		{
			return _altarsActivated;
		}

		
		unsigned int getHeroesKilled()
		{
			return _heroesKilled;
		}

		unsigned int getCreaturesKilled()
		{
			return _creaturesKilled;
		}

		unsigned int getMeritPoints()
		{
			return _meritPoints;
		}

		unsigned int getMyDeaths()
		{
			return _myDeaths;
		}

		bool hasThreeCombos()
		{
			return _hasThreeCombos;
		}

		unsigned int getMeritPointsSpent()
		{
			return _meritPointsSpent;
		}

		unsigned int getLostCreatures() //habrá que ampliarlo despues con los escuderos
		{
			return _lostCreatures;
		}

	protected:
		
		Hero _hero ;
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

		/**
			Puntero al CEntity que representa éste player/bot lógico
		*/		
		Logic::CEntity* _player;
	};
}

#endif //namespace Logic