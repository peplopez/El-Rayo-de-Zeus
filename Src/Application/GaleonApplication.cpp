//---------------------------------------------------------------------------
// GaleonApplication.cpp
//---------------------------------------------------------------------------

/**
@file GaleonApplication.cpp

Contiene la implementación de la clase aplicacion para aplicaciones
basadas en Ogre. Esta clase maneja la ejecución de todo el juego.

@see Application::CGaleonApplication
@see Application::C3DApplication

@author David Llansó
@date Septiembre, 2010
*/
#include "GaleonApplication.h"

#include "InitState.h" //PT

#include "ExitState.h"
#include "MenuState.h"
#include "GameOverState.h"
#include "GameState.h"

#include "NetMenuState.h"
#include "LobbyClientState.h"
#include "LobbyServerState.h"

namespace Application {

	CGaleonApplication::CGaleonApplication() : C3DApplication()
	{

	} // CGaleonApplication

	//--------------------------------------------------------

	CGaleonApplication::~CGaleonApplication()
	{

	} // ~CGaleonApplication

	//--------------------------------------------------------

	bool CGaleonApplication::init() 
	{
		// Inicializamos la clase de la que heredamos.
		if (!C3DApplication::init())
			return false;

		// CREACION DE ESTADOS. 
		// La aplicación se hace responsable de destruirlos.
		if(!addState("gameOver", new CGameOverState(this)))
			return false;
		//PT Añadido nuevo estado "inicio" (desde el que se pulsando sobre la pantalla se pasara al estado "menu")
		if(!addState("inicio", new CInitState(this)))
			return false;
		if(!addState("menu", new CMenuState(this)))
			return false;		
		if(!addState("game", new CGameState(this)))
			return false;
		if(!addState("exit", new CExitState(this)))
			return false;

		// ESTADOS DE RED
		if(!addState("netmenu", new CNetMenuState(this)))
			return false;
		if(!addState("lobbyclient", new CLobbyClientState(this)))
			return false;
		if(!addState("lobbyserver", new CLobbyServerState(this)))
			return false;

		// ESTADO INICIAL
		/*if(!setState("menu"))
			return false;*/


		//PT se sustituye el estado inicial menu, por el estado inicial inicio.
		if(!setState("inicio"))
			return false;


		return true;

	} // init

	//--------------------------------------------------------


} // namespace Application
