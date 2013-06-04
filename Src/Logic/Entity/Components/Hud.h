/***

@see Logic::CHud
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/

#ifndef __Logic_Hud_H
#define __Logic_Hud_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h"

namespace Logic
{
	class CGameStatus;
	class CMessageFloat;
}

namespace ScriptManager
{
	class Server;
}


//declaración de la clase
namespace Logic 
{
/***
	Este componente controla el hud mediante LUA
    @ingroup logicGroup

	@author Pablo Terrado
	@date Febrero, 2013
*/
	class CHud : public IComponent
	{
		DEC_FACTORY(CHud);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CHud() : IComponent(GetAltTypeIdOf(CHud)), _time(0), numMaxAltares(0), numBase(0), _ratioLife(1.0f) { }
		
		/** Destructor */
		CHud::~CHud();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool accept(const CMessage *message);

		void process(CMessage *message);	

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		*/
		void deactivate();

		void tick(unsigned int msecs);

		void displayHud();

		void updateHudLife(float ratioLife);
		

	protected:

		/**
		Tiempo de juego en milisegundos.
		*/
		unsigned int _time;

		/**
		Puntero al gamestatus global
		*/
		Logic::CGameStatus* _gameStatus;

		unsigned short numMaxAltares;
		unsigned short numBase;
		unsigned short player;

		float _ratioLife;

		 
	}; // class CHud

	REG_FACTORY(CHud);

} // namespace Logic

#endif // __Logic_Hud_H
