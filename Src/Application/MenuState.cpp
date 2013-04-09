//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llansó
@date Agosto, 2010
*/

#include "MenuState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CMenuState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Menu.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("Menu");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Single")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::startReleased, this));

		CEGUI::WindowManager::getSingleton().getWindow("Menu/Multi")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::multiplayerReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Exit")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::exitReleased, this));

		return true;

	} // init

	//--------------------------------------------------------

	void CMenuState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CMenuState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CMenuState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(GUI::TKey key)
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

			_mapsToLoad.push_back("mapPlayer");
			_mapsToLoad.push_back("mapNPC");

			if (!Logic::CServer::getSingletonPtr()->loadWorld(_mapsToLoad))
				return false;
		
			// Llamamos al método de creación del jugador. Deberemos decidir
			// si el jugador es el jugador local. Al ser el monojugador lo es.
			Logic::CServer::getSingletonPtr()->getMap("mapNPC")->createPlayer("Mono", "marine.mesh", true);

			// TODO Deberíamos poder propocionar caracteríasticas  (nombre, modelo, etc.)... ==> Ampliar MenuState...
			break;

		case GUI::Key::M:
			_app->setState("netmenu");
			break;
			
		default:
			return false;
			
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
// TODO Por qué se devuelve  true o false?
// TODO Toda la carga y creación del jugador deberia encapsularse en un startGame() reusable
	bool CMenuState::startReleased(const CEGUI::EventArgs& e)
	{
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

		_mapsToLoad.push_back("mapPlayer");
		_mapsToLoad.push_back("mapNPC");

		if (!Logic::CServer::getSingletonPtr()->loadWorld(_mapsToLoad))
			return false;
		
		// Llamamos al método de creación del jugador. Deberemos decidir
		// si el jugador es el jugador local. Al ser el monojugador lo es.
		Logic::CServer::getSingletonPtr()->getMap("mapPlayer")->createPlayer("Mono", "marine.mesh", true);

		// TODO Deberíamos poder propocionar características  (nombre, modelo, etc.)... ==> Ampliar MenuState...


		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->exitRequest();
		return true;

	} // exitReleased

	//--------------------------------------------------------

	bool CMenuState::multiplayerReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("netmenu");
		return true;

	} // multiplayerReleased


} // namespace Application
