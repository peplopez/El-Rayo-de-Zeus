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
#include "Logic/BaseInfo.h"
#include "Logic/RingInfo.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"

namespace Application {

	// ƒ®§ Al inicializar la app
	bool CGameState::init() 
	{
		CApplicationState::init();

		//Parameters for game in LUA
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("GameParameters");

		//_gameStatus = Logic::CGameStatus::getSingletonPtr();

		// FRS el Logic:: loadLevel se mueve a la parte final del lobby/menu state
		//, ya que los mapas cargados varían dependiendo de si somos server, client o monojudador

		// INICIALIZACIÓN DEL GUI
		// ----------------------

		// Ahora mismo la implementación está totalmente acoplada a CEGUI
		// Hay que desacoplarlo utilizando un nuevo paquete donde se abstraiga
		// el subsistema utilizado

		// Cargamos la ventana del HUD del Jugador.
		//CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Hud.layout");
		//_hudWindow = CEGUI::WindowManager::getSingleton().getWindow("Hud");

		// Cargamos la ventana del HUD del Jugador mediante LUA

		//ScriptManager::CServer::getSingletonPtr()->loadExeScript("Hud");
		//ScriptManager::CServer::getSingletonPtr()->executeProcedure("initHud");


		//inicialización del GameStatus:
		// se supone que hemos elegido ya en este punto cuantos jugadores somos
		/*if (_gameStatus==0)
			_gameStatus=new Logic::CGameStatus(8);
		else
			delete _gameStatus;
			*/
		return true;
	
	} // init

	//--------------------------------------------------------

	// ƒ®§ Al cerrar la app
	void CGameState::release() 
	{
		// Liberamos el nivel junto con las escenas físico-graficas.
		Logic::CServer::getSingletonPtr()->unLoadWorld();
		Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		CApplicationState::release();
	} // release

	//--------------------------------------------------------

	// ƒ®§ Al entrar en GameState (cambio de currentState)
	void CGameState::activate() 
	{
		/*	if (_gameStatus!=0)
			{
				delete _gameStatus; //se reinicia el juego... esto no es pausa de juego, es reinicio,
				//al menos de momento. Llegar al estado GameState es reinciar partida.
				_gameStatus=new Logic::CGameStatus(8);
			}else			
				_gameStatus=new Logic::CGameStatus(8);
		*/
		CApplicationState::activate();
	
		// Activamos el mapa que ha sido cargado para la partida (incluye la activacion de la escenas)
		Logic::CServer::getSingletonPtr()->activateAllMaps();

		// Queremos que el GUI maneje al jugador.
        GUI::CServer::getSingletonPtr()->getPlayerController()->activate();

		// Activamos la ventana y sus subventanas que nos muestran el HUD del Jugador con LUA
	/*	ScriptManager::CServer::getSingletonPtr()->executeProcedure("showHud");*/

	} // activate

	//--------------------------------------------------------

	//  ƒ®§ Al salir de gameState (cambio de currentState)
	void CGameState::deactivate() 
	{

		// Desactivamos la ventana de HUD por LUA
		/*ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideHud");*/

		// Desactivamos la clase que procesa eventos de entrada para  controlar al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();
		
		// Desactivamos el mapa de la partida (incluye la desactivacion de la escenas)
		Logic::CServer::getSingletonPtr()->deactivateAllMaps();
		
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

		/*_time += msecs;*/

		/*	if (_gameStatus->getBase(3)->areAllAltarsActivated())
			std::cout<<"APPLICATION::GAMESTATE::RAYAZO EN BASE 3"<<std::endl;
		*/
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
			Logic::CServer::getSingletonPtr()->unLoadWorld();
			Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();
			Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();
			_app->setState("menu");
			break;

		// TODO
		case GUI::Key::PAUSE:
			_app->setState("pause"); // TODO FRS no existe todavía el estado pause
			break;
		////PT. para ocultar y mostrar el HUD
		//case GUI::Key::V:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("changeHudVisibility");
		//	break;

		//	//PT prueba para aumentar el numero de altares activados
		//case GUI::Key::B:
		//	   ScriptManager::CServer::getSingletonPtr()->executeProcedure("incrementAltarsActivated");
		//	   ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateAltarsActivated");
		//	break;

		//case GUI::Key::G:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("decrementAltarsActivated");
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateAltarsActivated");
		//	break;

		//case GUI::Key::N:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("incrementEnemiesInBase");
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateEnemiesInBase");
		//	break;

		//case GUI::Key::H:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("decrementEnemiesInBase");
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateEnemiesInBase");
		//	break;
		//case GUI::Key::M:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("incrementPM");
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("updatePM");
		//	break;

		//case GUI::Key::J:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("decrementPM");
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("updatePM");
		//	break;

		//case GUI::Key::P:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("decrementBaseLife");
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateBaseLife");
		//	break;

		//case GUI::Key::O:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("incrementBaseLife");
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateBaseLife");
		//	break;
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
