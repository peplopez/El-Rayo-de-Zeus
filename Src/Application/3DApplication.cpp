//---------------------------------------------------------------------------
// 3DApplication.cpp
//---------------------------------------------------------------------------

/**
@file 3DApplication.cpp

Contiene la implementación de la clase aplicacion para aplicaciones
basadas en Ogre. Esta clase maneja la ejecución de todo el juego.

@see Application::C3DApplication
@see Application::CBaseApplication

@author David Llansó
@date Julio, 2010
*/

#include "OgreClock.h"
#include "3DApplication.h"

#include "Graphics/Server.h"
#include "BaseSubsystems/Server.h"
#include "GUI/InputManager.h"
#include "GUI/Server.h"
#include "Logic/Server.h"
#include "Logic/Maps/ComponentFactory.h"
#include "net/manager.h"

#include <cassert>

#include <iostream>

namespace Application {

	C3DApplication::C3DApplication() : CBaseApplication()
	{

	} // C3DApplication

	//--------------------------------------------------------

	C3DApplication::~C3DApplication()
	{

	} // ~C3DApplication

	//--------------------------------------------------------

	bool C3DApplication::init() 
	{
		// Inicializamos la clase base.
		if (!CBaseApplication::init())
			return false;

		// Inicializamos los diferentes subsistemas, los de 
		// Ogre entre ellos.
		if (!BaseSubsystems::CServer::Init())
			return false;

		// Inicializamos el servidor gráfico.
		if (!Graphics::CServer::Init())
			return false;

		// Inicializamos el gestor de entrada de periféricos.
		if (!GUI::CInputManager::Init())
			return false;

		// Nos registramos como oyentes de los eventos del teclado.
		GUI::CInputManager::getSingletonPtr()->addKeyListener(this);
		// Y como oyentes de los eventos del ratón.
		GUI::CInputManager::getSingletonPtr()->addMouseListener(this);

		// Inicializamos el servidor de interfaz con el usuario.
		if (!GUI::CServer::Init())
			return false;

		//// Inicialización del servidor de física.
		//if (!Physics::CServer::Init())
		//	return false;

		// Inicializamos el servidor de la lógica.
		if (!Logic::CServer::Init())
			return false;

		// Inicializamos el servidor de IA
		//if (!AI::CServer::Init())
		//	return false;

		// Inicializamos la red
		if (!Net::CManager::Init())
			return false;

		// Creamos el reloj basado en Ogre.
		_clock = new COgreClock();

		return true;

	} // init

	//--------------------------------------------------------

	void C3DApplication::release()
	{
		// Eliminamos el reloj de la aplicación.
		delete _clock;

		// Destruimos la factoría de componentes. La factoría
		// de componentes no es de construcción y destrucción explícita
		// debido a como se registran los componentes. Por ello Init y
		// Release no son simétricos.

		// Inicializamos la red
		if (Net::CManager::getSingletonPtr())
			Net::CManager::Release();
		
		// Liberar servidor de IA 
		//if (AI::CServer::getSingletonPtr())
		//	AI::CServer::Release();

		if(Logic::CComponentFactory::getSingletonPtr())
			delete Logic::CComponentFactory::getSingletonPtr();

		if(Logic::CServer::getSingletonPtr())
			Logic::CServer::Release();

		// Liberar los recursos del servidor de física
		//if (Physics::CServer::getSingletonPtr())
		//	Physics::CServer::Release();

		if(GUI::CServer::getSingletonPtr())
			GUI::CServer::Release();

		if(GUI::CInputManager::getSingletonPtr())
		{
			// Dejamos de ser oyentes de los eventos del teclado.
			GUI::CInputManager::getSingletonPtr()->removeKeyListener(this);
			// Y de los eventos del ratón
			GUI::CInputManager::getSingletonPtr()->removeMouseListener(this);
			GUI::CInputManager::Release();
		}
		
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::Release();

		if(BaseSubsystems::CServer::getSingletonPtr())
			BaseSubsystems::CServer::Release();

		CBaseApplication::release();

	} // release

	//--------------------------------------------------------

	void C3DApplication::tick(unsigned int msecs) 
	{
		CBaseApplication::tick(msecs);

		GUI::CInputManager::getSingletonPtr()->tick();

		Graphics::CServer::getSingletonPtr()->tick(msecs/1000.0f);

		Net::CManager::getSingletonPtr()->tick(msecs);

	} // tick

} // namespace Application
