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

#include "Logic/GameStatus.h"

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

	// INICIALIZACIÓN DEL GUI
	// ----------------------

	// Ahora mismo la implementación está totalmente acoplada a CEGUI
	// Hay que desacoplarlo utilizando un nuevo paquete donde se abstraiga
	// el subsistema utilizado


	// Cargamos la ventana que muestra el tiempo de juego transcurrido.
	//CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Time.layout");
	//_timeWindow = CEGUI::WindowManager::getSingleton().getWindow("Time");

	// Cargamos la ventana del HUD del Jugador.
	CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Hud.layout");
	_hudWindow = CEGUI::WindowManager::getSingleton().getWindow("Hud");

	//PT. Inicialización de variables del HUD
	_numberEnemies = 0;
	_puntosMerito = 100;
	_numberAltaresActivated = 0;

	//inicialización del GameStatus:
	// se supone que hemos elegido ya en este punto cuantos jugadores somos
	Logic::CGameStatus* gameStatus=new Logic::CGameStatus(8);

		return true;

	} // init

	//--------------------------------------------------------

	// ƒ®§ Al cerrar la app
	void CGameState::release() 
	{
		// Liberamos el nivel junto con las escenas físico-graficas.
		Logic::CServer::getSingletonPtr()->unLoadMap();
		Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		CApplicationState::release();
	} // release

	//--------------------------------------------------------

	// ƒ®§ Al entrar en GameState (cambio de currentState)
	void CGameState::activate() 
	{
		CApplicationState::activate();
		
		// Activamos el mapa que ha sido cargado para la partida (incluye la activacion de la escenas)
		Logic::CServer::getSingletonPtr()->activateMap();

		// Queremos que el GUI maneje al jugador.
        GUI::CServer::getSingletonPtr()->getPlayerController()->activate();

	// Activamos la ventana que nos muestra el tiempo transcurrido.
		/*CEGUI::System::getSingletonPtr()->setGUISheet(_timeWindow);
		_timeWindow->setVisible(true);
		_timeWindow->activate();*/

	// Activamos la ventana que nos muestra el HUD del Jugador
		CEGUI::System::getSingletonPtr()->setGUISheet(_hudWindow);
		_hudWindow->setVisible(true);
		_hudWindow->activate();

		_NumberEnemyWindow = _hudWindow->getChild("Hud/NumEnemigos");
		_PuntosMeritoWindow = _hudWindow->getChild("Hud/PuntosMerito");
		_NumberAltaresActivatedWindow = _hudWindow->getChild("Hud/NumAltares");

		
	} // activate

	//--------------------------------------------------------

	//  ƒ®§ Al salir de gameState (cambio de currentState)
	void CGameState::deactivate() 
	{

		// Desactivamos la ventana de tiempo.
		//_timeWindow->deactivate();
		//_timeWindow->setVisible(false);

		// Desactivamos la ventana de HUD.
		_hudWindow->deactivate();
		_hudWindow->setVisible(false);

		// Desactivamos la clase que procesa eventos de entrada para  controlar al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();
		
		// Desactivamos el mapa de la partida (incluye la desactivacion de la escenas)
		Logic::CServer::getSingletonPtr()->deactivateMap();
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);
		// FRS Los siguientes ticks no se colocan a nivel de C3DApplication::tick
		// porque a diferencia de otros servers, sólo deben actualizarse durante el GameState)
		
		Physics::CServer::getSingletonPtr()->tick(msecs);// Simulación física 		
		Logic::CServer::getSingletonPtr()->tick(msecs);// Actualizamos la lógica de juego.

		_time += msecs;

		std::stringstream text;
		text << "Time: " << _time/1000;

		//_timeWindow->setText(text.str());

	} // tick

	


	/**************
		INPUT
	*************/

	bool CGameState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	// TODO FRS de momento el ESC = salir de la partida, pero en un futuro podría ser = Pause... + New / Continue
	bool CGameState::keyReleased(GUI::TKey key)
	{
		std::stringstream textAltaresActivated;
		std::stringstream textNumberEnemies;
		std::stringstream textPuntosMerito;

		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			Logic::CServer::getSingletonPtr()->unLoadMap();
			Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();
			Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();
			_app->setState("menu");
			break;

		// TODO
		case GUI::Key::PAUSE:
			_app->setState("pause"); // TODO FRS no existe todavía el estado pause
			break;
		//PT. para ocultar y mostrar el HUD
		case GUI::Key::V:
			if(_hudWindow->isVisible() == true)
				_hudWindow->setProperty("Visible", "false");
			else
				_hudWindow->setProperty("Visible", "true");
			break;
			//PT prueba para aumentar el numero de altares activados
		case GUI::Key::B:
				_numberAltaresActivated += 1;			
				textAltaresActivated <<  _numberAltaresActivated << "/11";
				_NumberAltaresActivatedWindow->setText(textAltaresActivated.str());
			break;
		case GUI::Key::N:
				_numberEnemies += 1;
				textNumberEnemies <<  _numberEnemies;
				_NumberEnemyWindow->setText(textNumberEnemies.str());
			break;
		case GUI::Key::M:
				_puntosMerito += 10;
				textPuntosMerito <<  _puntosMerito;
				_PuntosMeritoWindow->setText(textPuntosMerito.str());
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
