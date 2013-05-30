//---------------------------------------------------------------------------
// MenuSingleState.cpp
//---------------------------------------------------------------------------

/**
@file MenuSingleState.cpp

Contiene la implementación del estado de menú de opciones de Single Player.

@see Application::CApplicationState
@see Application::CMenuSingleState

@author Pablo Terrado
@date Mayo, 2013
*/

#include "MenuSingleState.h"
#include "BaseApplication.h"
#include "Clock.h"
#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"
//PT
#include <CEGUIDataContainer.h>
#include <CEGUIWindowRenderer.h>
#include <cegui\elements\CEGUIProgressBar.h>
//PT
#include <OGRE\Ogre.h>
#include <Ogre.h>

namespace Application {

	CMenuSingleState::~CMenuSingleState() 
	{
	} // ~CMenuSingleState

	//--------------------------------------------------------

	bool CMenuSingleState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		/*
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("MenuSingle.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("MenuSingle");
		*/

		// Cargamos la ventana que muestra el menú con LUA
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("MenuSingle");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initMenuSingle");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/Play")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuSingleState::startReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuSingleState::backReleased, this));

			//Modelo .mesh o personaje
		   _cbModel = static_cast<CEGUI::Combobox*>(CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/ModelBox"));

			// add items to the combobox list
			_cbModel->addItem(new CEGUI::ListboxTextItem("Espartano",0));
			_cbModel->addItem(new CEGUI::ListboxTextItem("Loco",1));
			_cbModel->addItem(new CEGUI::ListboxTextItem("Marine",2));
			_cbModel->addItem(new CEGUI::ListboxTextItem("Bioshock",3));
			_cbModel->addItem(new CEGUI::ListboxTextItem("Medusa",4));


			_cbModel->setReadOnly(true);

			//Color
			_cbColor = static_cast<CEGUI::Combobox*>(CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/ColorBox"));
			_cbColor->addItem(new CEGUI::ListboxTextItem("Rojo",0));
			_cbColor->addItem(new CEGUI::ListboxTextItem("Verde",1));
			_cbColor->addItem(new CEGUI::ListboxTextItem("Amarillo",2));
			_cbColor->addItem(new CEGUI::ListboxTextItem("Azul",3));

			_cbColor->setReadOnly(true);


			//_hbar = static_cast<CEGUI::ProgressBar*> 
			//	(CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/Progreso"));

			//_hbar->subscribeEvent(CEGUI::ProgressBar::EventProgressChanged, 
			//	CEGUI::Event::Subscriber(&CMenuSingleState::onProgressChanged, this));

			//_hbar->setProgress(0.0f);



		return true;

	} // init

	//--------------------------------------------------------

	void CMenuSingleState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CMenuSingleState::activate() 
	{
		CApplicationState::activate();

		CBaseApplication::getSingletonPtr()->getClock()->removeAllTimeObserver();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		/*
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();
		*/

		// Activamos la ventana que nos muestra el menú y activamos el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showMenuSingle");

	} // activate

	//--------------------------------------------------------

	void CMenuSingleState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		/*
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		*/

		// Desactivamos la ventana GUI con el menú y el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideMenuSingle");

		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuSingleState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		//PT
		//CEGUI::System::getSingleton().renderGUI();

	} // tick

	//--------------------------------------------------------

	bool CMenuSingleState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuSingleState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{

		case GUI::Key::ESCAPE:
			_app->exitRequest();
			break;

		case GUI::Key::RETURN:
			_app->setState("game");
			
			//[ƒ®§] CARGA de Blueprints, Arquetypes y Map adelantada
			// Cargamos el archivo con las definiciones de las entidades del nivel.
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
				return false;
				
			// Add - ESC
			// Cargamos el archivo con las definiciones de los archetypes
			if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt"))
				return false;
			
			// Add - JLS
			// Cargamos los anillos a partir del nombre del mapa. 
			if (!Logic::CServer::getSingletonPtr()->setRingPositions())//[ƒ®§] Esto no deberia ejecutarse como parte del loadLevel...?
				return false;

			// Cargamos el nivel a partir del nombre del mapa. 
			//if (!Logic::CServer::getSingletonPtr()->loadMap("map.txt"))
			//	return false;

			_mapsToLoad.push_back("mapRed");
			_mapsToLoad.push_back("mapBlue");
			
			//_mapsToLoad.push_back("mapGreen");
			//_mapsToLoad.push_back("mapYellow");

			if (!Logic::CServer::getSingletonPtr()->loadWorld(_mapsToLoad))
				return false;
		
			// Llamamos al método de creación del jugador. Deberemos decidir
			// si el jugador es el jugador local. Al ser el monojugador lo es.

			// TODO Deberíamos poder propocionar caracteríasticas  (nombre, modelo, etc.)... ==> Ampliar MenuSingleState...
			//Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer("Mono", true);
			Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer("Mono", true, "Mono", "spartan2.4.mesh", "SpartanBodyWounds");

			break;

		case GUI::Key::M:
			_app->setState("netmenu");
			break;

		//case GUI::Key::R:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("reloadMenuSingle");
		//	break;
			
		default:
			return false;
			
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuSingleState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuSingleState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuSingleState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
// TODO Por qué se devuelve  true o false?
// TODO Toda la carga y creación del jugador deberia encapsularse en un startGame() reusable
	bool CMenuSingleState::startReleased(const CEGUI::EventArgs& e)
	{

				//Ventana principal de MenuSingle
				//CEGUI::Window * mWindow = CEGUI::WindowManager::getSingleton().getWindow("MenuSingle");

				// Barra de progreso de MenuSingle
				CEGUI::ProgressBar *hbar = static_cast<CEGUI::ProgressBar*> (CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/Progreso"));
				//hbar->subscribeEvent(CEGUI::ProgressBar::EventProgressChanged, CEGUI::Event::Subscriber(&EventGalore::onProgressChanged, this));
				hbar->subscribeEvent(CEGUI::ProgressBar::EventProgressChanged, CEGUI::Event::Subscriber(&CMenuSingleState::onProgressChanged, this));
				//subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::SubscriberSlot(&CMenuSingleState::startReleased, this));

				//RECUPERAMOS LA INFORMACION DEL MENUSINGLESTATE (Nickname, Modelo, y Color)
				// OBTENER PLAYER INFO

				//NICKNAME
				std::string playerNick = std::string( CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/NickBox")->getText().c_str() );

				//Se eliminan los espacios (trim)
				playerNick.erase(playerNick.find_last_not_of(" \n\r\t")+1);

				if(playerNick.length() == 0)
				{
					ScriptManager::CServer::getSingletonPtr()->executeProcedureString("showError", std::string("It must be provided a not empty Nickname"));
				    return false;
				}
				//MODELO
				//int playerModelID = (int)CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/ModelBox")->getID();
				int playerModelID = 99;

				if(_cbModel->getSelectedItem() != NULL)
					playerModelID = _cbModel->getSelectedItem()->getID();

				std::string playerModel = "";
				//COLOR
				//std::string playerColor = std::string(CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/ColorBox")->getText().c_str());
				//int playercolorID = (int)CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/ColorBox")->getID();
				int playercolorID = 99;
				if(_cbColor->getSelectedItem() != NULL)
					playercolorID = _cbColor->getSelectedItem()->getID();
				std::string playerColor = "";

				//PT. se rellena el string de playerModel con su correspondiente asociacion a su ID
				switch(playerModelID)
				{
					case 0: //espartano
						playerModel = "spartan2.4.mesh";
						break;
					case 1: //loco
						playerModel = "loco.mesh";
						break;
					case 2: //marine
						playerModel = "marine.mesh";
						break;
					case 3: //biosock
						playerModel = "bioshock.mesh";
						break;
					case 4: //medusa
						playerModel = "medusa.mesh";
						break;
					default: //espartano
						playerModel = "spartan2.4.mesh";
						break;
				}

				//Tambien el color
				switch(playercolorID)
				{
					case 0: //Rojo
						playerColor = "marineRed";
						break;
					case 1: //Verde
						playerColor = "marineGreen";
						break;
					case 2: //Amarillo
						playerColor = "marineYellow";
						break;
					case 3: //Azul
						playerColor = "marineBlue";
						break;
					default: //ninguno
						//playerColor = "SpartanBodyTatoo";
						playerColor = "SpartanBodyWounds";
						break;
				}


		_app->setState("game");

		//hbar->setProgress(0.0f);
		//hbar->CEGUI::ProgressBar::setProgress(0.0f);
		//hbar->setStepSize(0.10f);
		//mWindow->update(false);
		//CEGUI::System::getSingleton().renderGUI();

		
			
		//[ƒ®§] CARGA de Blueprints, Arquetypes y Map adelantada
		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;

		hbar->setProgress(0.2f);
		//hbar->CEGUI::ProgressBar::setProgress(0.2f);
		//hbar->step();
		//hbar->step();
		//CEGUI::System::getSingleton().renderGUI();
		//hbar->update(0.0001f);
				
		// Add - ESC
		// Cargamos el archivo con las definiciones de los archetypes
		if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt"))
			return false;

		hbar->setProgress(0.4f);
		//hbar->CEGUI::ProgressBar::setProgress(0.4f);
		//hbar->step();
		//hbar->step();
		//CEGUI::System::getSingleton().renderGUI();
		//hbar->update(0.0001f);
			
		// Add - JLS
		// Cargamos los anillos a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->setRingPositions())//[ƒ®§] Esto no deberia ejecutarse como parte del loadLevel...?
			return false;

		//Inclusion de mapas a cargar a la lista de mapas _mapsToLoad
		_mapsToLoad.push_back("mapRed");
		_mapsToLoad.push_back("mapBlue");
		//_mapsToLoad.push_back("mapGreen");
		//_mapsToLoad.push_back("mapYellow");

		//Carga de mapas
		if (!Logic::CServer::getSingletonPtr()->loadWorld(_mapsToLoad))
			return false;

		hbar->setProgress(0.8f);
		/*hbar->CEGUI::ProgressBar::setProgress(0.8f);*/
		//hbar->step();
		//hbar->step();
		//hbar->step();
		//hbar->step();
		//CEGUI::System::getSingleton().renderGUI();
		//hbar->update(0.0001f);
		
		// Llamamos al método de creación del jugador. 
		// Al estar en el estado MenuSingleState el jugador es Single Player (Monojugador)


		//CMap::createPlayer(entityName, isLocalPlayer, model)
		//Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer("Mono", true);
		Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer(playerNick, true, playerNick, playerModel, playerColor);
		//PT. si se le intenta pasar otro modelo da un error en getBones porque no encuentra el hueso "paracasco"
		//para que funcione medusa, bigdday etc, hay que quitar el casco y el escudo y los accesorios al espartano
		//en el archetypes. de esa manera no da error.

		hbar->setProgress(1.0f);
		//hbar->CEGUI::ProgressBar::setProgress(1.0f);
		//hbar->step();
		//hbar->step();
		//hbar->update(false);
		//CEGUI::System::getSingleton().renderGUI();
		//hbar->swapBuffers(false);

		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CMenuSingleState::backReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("menu");
		return true;

	} // backReleased


	bool CMenuSingleState::onProgressChanged(const CEGUI::EventArgs &e)
	{
		// The progress changed.
		float progress =  static_cast<CEGUI::ProgressBar*> (CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/Progreso"))->getProgress();
		int finalProgress = static_cast<int>(progress*100);
		std::stringstream resultStr;
		resultStr << "Progress: " << finalProgress << "%";
 
		CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/TextoProgreso")->setText(resultStr.str());
		return true;
	} //onProgressChanged

} // namespace Application
