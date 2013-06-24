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
		CShop() : IComponent(GetAltTypeIdOf(CShop)), _time(0), numBase(0), _winShop(0), _upgradesWindow(0), _alliesWindow(0), 
		tercerComboWindow(0), rezoHadesWindow(0), rezoTiqueWindow(0), rezoArtemisaWindow(0), rezoZeusWindow(0), 
		rezoCronosWindow(0), rezoAresWindow(0), rezoAteneaWindow(0), fortalecerBaseWindow(0), debilitarBasesWindow(0),
		sacrificioWindow(0), sacrificioDefinitivoWindow(0),
		gorgonawindow(0), helenatroyawindow(0), centaurowindow(0), minotaurowindow(0), cerberuswindow(0), ciclopewindow(0) { }
		
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
		
		void deactivateControl();

		bool activateControl();


		//Functions for create allies
		void createAlly(const std::string &type, const unsigned int cost);
		bool createGorgona(const CEGUI::EventArgs&e);
		bool createHelenaTroya(const CEGUI::EventArgs&e);
		bool createCentauro(const CEGUI::EventArgs&e);
		bool createMinotauro(const CEGUI::EventArgs&e);
		bool createCerberus(const CEGUI::EventArgs&e);
		bool createCiclope(const CEGUI::EventArgs&e);

		//Functions for upgrades
		bool activateThirdCombo(const CEGUI::EventArgs&e);
		bool rezoHades(const CEGUI::EventArgs&e);
		bool rezoTique(const CEGUI::EventArgs&e);
		bool rezoArtemisa(const CEGUI::EventArgs&e);
		bool rezoZeus(const CEGUI::EventArgs&e);
		bool rezoCronos(const CEGUI::EventArgs&e);
		bool rezoAres(const CEGUI::EventArgs&e);
		bool rezoAtenea(const CEGUI::EventArgs&e);
		bool fortalecerBase(const CEGUI::EventArgs&e);
		bool debilitarBases(const CEGUI::EventArgs&e);
		bool sacrificio(const CEGUI::EventArgs&e);
		bool sacrificioDefinitivo(const CEGUI::EventArgs&e);

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

		//MAIN SHOP WINDOW and TAB WINDOWS INSIDE
		CEGUI::FrameWindow* _winShop;
		CEGUI::Window* _upgradesWindow;
		CEGUI::Window* _alliesWindow;

		//UPGRADES
		CEGUI::Window* tercerComboWindow;
		CEGUI::Window* rezoHadesWindow;
		CEGUI::Window* rezoTiqueWindow;
		CEGUI::Window* rezoArtemisaWindow;
		CEGUI::Window* rezoZeusWindow;
		CEGUI::Window* rezoCronosWindow;
		CEGUI::Window* rezoAresWindow;
		CEGUI::Window* rezoAteneaWindow;
		CEGUI::Window* fortalecerBaseWindow;
		CEGUI::Window* debilitarBasesWindow;
		CEGUI::Window* sacrificioWindow;
		CEGUI::Window* sacrificioDefinitivoWindow;

		//ALLIES AND CRIATURES
		CEGUI::Window* gorgonawindow;
		CEGUI::Window* helenatroyawindow;
		CEGUI::Window* centaurowindow;
		CEGUI::Window* minotaurowindow;
		CEGUI::Window* cerberuswindow;
		CEGUI::Window* ciclopewindow;

		//CEGUI::Window* grados;
		//std::ostringstream gradosstr;

		 
	}; // class CShop

	REG_FACTORY(CShop);

} // namespace Logic

#endif // __Logic_Shop_H
