//---------------------------------------------------------------------------
// GameState.cpp
//---------------------------------------------------------------------------

/**
@file GameState.cpp

Contiene la implementación del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llansó
@date Agosto, 2010
*/

#include "GameState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "Physics/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>

namespace Application {

	// ƒ®§ Al inicializar la app
	bool CGameState::init() 
	{
		CApplicationState::init();

		// FRS el Logic:: loadLevel se mueve a la parte final del lobby/menu state
		//, ya que los mapas cargados varían dependiendo de si somos server, client o monojudador

		return true;

	} // init

	//--------------------------------------------------------

	// ƒ®§ Al cerrar la app
	void CGameState::release() 
	{
		Logic::CServer::getSingletonPtr()->unLoadLevel();

		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();
		Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();
		
		// Liberamos la escena física.
		Physics::CServer::getSingletonPtr()->destroyScene();

		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	// ƒ®§ Al entrar en GameState (cambio de currentState)
	void CGameState::activate() 
	{
		CApplicationState::activate();
		
		// Activamos el mapa que ha sido cargado para la partida.
		Logic::CServer::getSingletonPtr()->activateMap();

		// Activamos escena física
		Physics::CServer::getSingletonPtr()->activateScene();

		// Queremos que el GUI maneje al jugador.
        GUI::CServer::getSingletonPtr()->getPlayerController()->activate();
		
		// Activamos la ventana que nos muestra el HUD.
		/*CEGUI::System::getSingletonPtr()->setGUISheet(_hudWindow);
		/*
		CEGUI::System::getSingletonPtr()->setGUISheet(_hudWindow);
		_hudWindow->setVisible(true);
		_hudWindow->activate();*/
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Hud.layout");
			_hudWindow = CEGUI::WindowManager::getSingleton().getWindow("Hud");


	} // activate

	//--------------------------------------------------------

	//  ƒ®§ Al salir de gameState (cambio de currentState)
	void CGameState::deactivate() 
	{
		// Desactivamos la ventana de HUD.
		_hudWindow->deactivate();
		_hudWindow->setVisible(false);

		// Desactivamos la clase que procesa eventos de entrada para 
		// controlar al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();

		// Desactivamos escena física
		Physics::CServer::getSingletonPtr()->deactivateScene();
		
		// Desactivamos el mapa de la partida.
		Logic::CServer::getSingletonPtr()->deactivateMap();
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;
		
		std::stringstream text;
		text << "Time: " << _time/1000;
		//_hudWindow->setText(text.str());

	} // tick

	//--------------------------------------------------------

	bool CGameState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	// TODO FRS de momento el ESC = salir de la partida, pero en un futuro podría ser = Pause... + New / Continue
	bool CGameState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			Logic::CServer::getSingletonPtr()->unLoadLevel();
			_app->setState("menu");
			break;

		case GUI::Key::PAUSE:
			_app->setState("pause");
			break;

		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
