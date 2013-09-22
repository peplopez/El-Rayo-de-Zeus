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

#include <BaseSubsystems/Server.h>
#include <Graphics/Server.h>
#include <GUI/Server.h>
#include <Logic/Server.h>
#include <Logic/Maps/Map.h>
#include <Logic/Maps/EntityFactory.h>
#include <ScriptManager/Server.h>

#include <CEGUIMouseCursor.h>//HACK FRS



#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "APP::MENU_SINGLE>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif



namespace Application {

	const std::string CMenuSingleState::WINDOW_PREFIX = "MenuSingle";

	void CMenuSingleState::_play()
	{
		
		Logic::TMultiSettings allSettings;

		if( !_readPlayerForm(allSettings) )	{
			_setProgress(0, "Error al leer datos de formulario");		

		//Carga del juego (arquetipos, blueprints, mapas y players...)
		} else if( !_loadGame(allSettings) ) {	
			_setProgress(0, "Error al cargar juego");	

		} else {
			_app->setState("game"); // Acaba generando un GameState->activate()
			_setProgress(1.0f, "LISTO!!");
		}
			
	}// play

	//---------------------------------------------------------------------

	

	//RECUPERAMOS LA INFORMACION DEL PLAYER FORM (Nickname, Avatar, Color, enemigos, ...)
	bool CMenuSingleState::_readPlayerForm(Logic::TMultiSettings& allSettings)
	{
		//NICKNAME
		std::string& nick = _guiServer->getWindowText(WINDOW_PREFIX + "/NickBox");
			if( !nick.length() ) {
				ScriptManager::CServer::getSingletonPtr()->executeProcedureString(
					"showError", std::string("It must be provided a not empty Nickname"));
				return false;
			}

		int colorIndex		= _guiServer->getComboSelectedID(WINDOW_PREFIX + "/ColorBox");
		int avatarIndex		= _guiServer->getComboSelectedID(WINDOW_PREFIX + "/AvatarBox");
		int stageIndex		= _guiServer->getComboSelectedID(WINDOW_PREFIX + "/StageBox");
		int nOpps			= _guiServer->getComboSelectedID(WINDOW_PREFIX + "/OppBox");

		// FRS Si no se escoge opción, default = 0;
		colorIndex =	colorIndex	< 0 ? 0 : colorIndex; 
		avatarIndex =	avatarIndex	< 0 ? 0 : avatarIndex;
		stageIndex =	stageIndex	< 0 ? 0 : stageIndex;

		Logic::TPlayerColor color	= static_cast< Logic::TPlayerColor>(colorIndex);
		Logic::TPlayerAvatar avatar = static_cast< Logic::TPlayerAvatar>(avatarIndex);
		Logic::TPlayerStage stage	= static_cast< Logic::TPlayerStage>(stageIndex);
		nOpps = nOpps < 0 ? 1 : nOpps + 1;
				
		// Al estar en el estado MenuSingleState el jugador es Single Player (Monojugador)
		allSettings.push_back( Logic::CPlayerSettings(nick, true, false, color, avatar, stage) );
		for(int i=0; i< nOpps; ++i)
			allSettings.push_back( Logic::CPlayerSettings::createRandSettings(true) );

		return true;		
	} // readPlayerForm




	//---------------------------------------------------------------------
	
	bool CMenuSingleState::_loadGame(Logic::TMultiSettings& allSettings)
	{
		_setProgress(0.4f, "Loading maps..." );

//*
		
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints"))
			return false;
		if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes"))
			return false;
		if (!Logic::CEntityFactory::getSingletonPtr()->loadPatternEntities("map_pattern"))
			return false;										// JLS Cargamos los anillos a partir del nombre del mapa.
		if (!Logic::CServer::getSingletonPtr()->setRingPositions())//[ƒ®§] Esto no deberia ejecutarse como parte del loadLevel...?
			return false;
		if (!Logic::CServer::getSingletonPtr()->loadWorld(allSettings) ) // FRS CARGA DESDE MAP_PATTERN
			return false;

		
/*/
		Logic::TMapNames mapsToLoad;		// FRS OLD: Carga desde fichero
			mapsToLoad.push_back("mapRed");
			mapsToLoad.push_back("mapBlue");	
				if (!Logic::CServer::getSingletonPtr()->loadWorld(mapsToLoad) )
					return false;

		// Llamamos al método de creación del jugador. 
		// Al estar en el estado MenuSingleState el jugador es Single Player (Monojugador)
		Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer(
		Logic::CPlayerSettings(playerNick, Logic::Player::Color::BLUE, Logic::Player::Avatar::SPARTAN), true);
/**/

		_setProgress(0.9f, "World loaded");

		return true;
	} //loadGame








	/*************************
		CApplicationState
	**************************/

	bool CMenuSingleState::init() 
	{
	
	// CEGUI

		// Cargamos la ventana que muestra el menú con LUA
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("MenuSingle");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initMenuSingle");
		
		_guiServer = GUI::CServer::getSingletonPtr();
		
		//// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_guiServer->setCallbackButton(WINDOW_PREFIX + "/Play",  &CMenuSingleState::_playReleased, this);
		_guiServer->setCallbackButton(WINDOW_PREFIX + "/Back",  &CMenuSingleState::_backReleased, this);
		_guiServer->setCallbackProgress(WINDOW_PREFIX + "/Progreso", &CMenuSingleState::_onProgressChanged, this);
		

		// COMBOBOXES
		_guiServer->createCombobox(
			 WINDOW_PREFIX + "/ColorBox", 
			 Logic::Player::COLOR_STRINGS, 
			 Logic::Player::Color::_COUNT
		);
		 _guiServer->createCombobox(
			WINDOW_PREFIX + "/AvatarBox",	
			Logic::Player::AVATAR_STRINGS, 
			Logic::Player::Avatar::_COUNT
		);
		_guiServer->createCombobox(
			WINDOW_PREFIX + "/StageBox", // TODO
			Logic::Player::STAGE_SCENES, 
			Logic::Player::Stage::_COUNT 
		);		
		std::string nOpps[] = {"1","2","3"}; 
		_guiServer->createCombobox(
			WINDOW_PREFIX + "/OppBox", // TODO
			nOpps, 
			sizeof(nOpps) / sizeof( nOpps[0] )
		);

		// CHECKBOX 
		// TODO
		//_checkLowQ = 


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

		// Activamos la ventana que nos muestra el menú y activamos el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showMenuSingle");

		// In case we come back from game to MenuSingleState
		// HACK FRS Windowed? -> Desactivar el cursor de CEGUI y superponer el de WIN32
		#if _WIN32	
				if(BaseSubsystems::CServer::getSingletonPtr()->isWindowedMode() )
					CEGUI::MouseCursor::getSingletonPtr()->hide();
		#endif

		// In case we come back from game to MenuSingleState
		_guiServer->setWindowVisible(WINDOW_PREFIX + "/Progreso", false);
		_guiServer->setWindowVisible(WINDOW_PREFIX + "/TextoProgreso", false);
	} // activate

	//--------------------------------------------------------

	void CMenuSingleState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideMenuSingle");
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuSingleState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick






	/**************************
		CEGUI::WindowManager
	****************************/

	bool CMenuSingleState::_playReleased(const CEGUI::EventArgs& e)
	{
		_play();
		return true;
	} // startReleased
			
	//--------------------------------------------------------

	bool CMenuSingleState::_backReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("menu");
		return true;
	} // backReleased

	//---------------------------------------------------------------------

	bool CMenuSingleState::_onProgressChanged(const CEGUI::EventArgs &e)
	{
		Graphics::CServer::getSingletonPtr()->tick(0);
		return true;
	} //onProgressChanged

	//---------------------------------------------------------------------

	void CMenuSingleState::_setProgress(float progressAmount,  const std::string& statusMsg)
	{
		_guiServer->updateProgress( 
			WINDOW_PREFIX + "/Progreso", WINDOW_PREFIX + "/TextoProgreso", 
			progressAmount, statusMsg
		);
		// TODO FRS si el progress sólo se va a actualizar a través de esta función -> barajar 
		// el ejecutar el Graphics::CServer::getSingletonPtr()->tick(0) aquí y dejarnos de callbacks
	}

	



	/******************
		KEYBOARD
	******************/

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
			_play();
			break;
					
		//case GUI::Key::R:
		//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("reloadMenuSingle");
		//	break;
			
		default:
			return false;
			
		}
		return true;

	} // keyReleased

	
	
	/************
		MOUSE
	************/
	
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

	
} // namespace Application
