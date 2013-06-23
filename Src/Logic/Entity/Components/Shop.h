/***

@see Logic::CShop
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/

#ifndef __Logic_Shop_H
#define __Logic_Shop_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h"

#include "../../../../dependencies/include/cegui/CEGUIWindow.h"
#include "../../../../dependencies/include/cegui/CEGUIRect.h"

#include <sstream>     // std::ostringstream


namespace Logic
{
	class CGameStatus;
	class CMessageFloat;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
	class WindowManager;
	class FrameWindow;
	class Rect;
	class Tooltip;
}

namespace ScriptManager
{
	class Server;
}


//declaración de la clase
namespace Logic 
{
/***
	Este componente controla el Shop mediante LUA
    @ingroup logicGroup

	@author Pablo Terrado
	@date Febrero, 2013
*/
	class CShop : public IComponent
	{
		DEC_FACTORY(CShop);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CShop() : IComponent(GetAltTypeIdOf(CShop)), _time(0), numBase(0), _winShop(0), _itemsWindow(0), _criaturesWindow(0), 
		item1window(0), item2window(0), item3window(0), medusawindow(0), cerberuswindow(0), minotaurowindow(0) { }
		
		/** Destructor */
		CShop::~CShop();

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

		void displayShop();

		bool handleClose(const CEGUI::EventArgs&e);

		void createAlly(const std::string &type, const unsigned int cost);

		//Functions for create allies
		bool createCerberus(const CEGUI::EventArgs&e);
		bool createMedusa(const CEGUI::EventArgs&e);

		void deactivateControl();
		bool activateControl();
		

	protected:

		/**
		Tiempo de juego en milisegundos.
		*/
		unsigned int _time;

		/**
		Puntero al gamestatus global
		*/
		Logic::CGameStatus* _gameStatus;

		unsigned short numBase, player, pm;

		//area for limiting mouse movements
		CEGUI::Rect _area;

		CEGUI::FrameWindow* _winShop;

		CEGUI::Window* _itemsWindow;
		CEGUI::Window* _criaturesWindow;
		/*CEGUI::Window* _comboWindow;*/

		CEGUI::Window* item1window;
		CEGUI::Window* item2window;
		CEGUI::Window* item3window;

		CEGUI::Window* medusawindow;
		CEGUI::Window* cerberuswindow;
		CEGUI::Window* minotaurowindow;

		//CEGUI::Window* grados;
		//std::ostringstream gradosstr;

		 
	}; // class CShop

	REG_FACTORY(CShop);

} // namespace Logic

#endif // __Logic_Shop_H
