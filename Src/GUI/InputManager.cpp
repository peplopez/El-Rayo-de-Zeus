//---------------------------------------------------------------------------
// InputManager.cpp
//---------------------------------------------------------------------------

/**
@file InputManager.cpp

Contiene la implementación del gestor de periféricos de entrada
así como las clases oyentes que deben extender las clases que
quieran ser avisadas de los eventos de dichos periféricos:
ratón y teclado.

@see GUI::CInputManager
@see GUI::CKeyboardListener
@see GUI::CMouseListener

@author David Llansó
@date Julio, 2010
*/

#include "InputManager.h"

#include "BaseSubsystems/Server.h"

#include <OISInputManager.h>

#include <sstream>
#include <cassert>


namespace GUI{

	CInputManager *CInputManager::_instance = 0;

	//--------------------------------------------------------

	CInputManager::CInputManager() :
		_mouse(0),
		_keyboard(0),
		_inputSystem(0)
	{
		assert(!_instance && "¡Segunda inicialización de GUI::CInputManager no permitida!");
		_instance = this;

	} // CInputManager

	//--------------------------------------------------------

	CInputManager::~CInputManager() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CInputManager

	//--------------------------------------------------------

	bool CInputManager::Init() 
	{
		assert(!_instance && "¡Segunda inicialización de GUI::CInputManager no permitida!");

		new CInputManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CInputManager::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CInputManager::open() 
	{
		if(!BaseSubsystems::CServer::getSingletonPtr())
			return false;

		// Cogemos el sistema de entrada.
		_inputSystem = BaseSubsystems::CServer::getSingletonPtr()->getInputSystem();

		// Cogemos el buffer del teclado y nos hacemos oyentes.
		_keyboard = BaseSubsystems::CServer::getSingletonPtr()->getBufferedKeyboard();
		if(_keyboard)
			_keyboard->setEventCallback(this);

		// Cogemos el buffer del ratón y nos hacemos oyentes.
		_mouse = BaseSubsystems::CServer::getSingletonPtr()->getBufferedMouse();
		if(_mouse)
			_mouse->setEventCallback(this);

		return true;

	} // open

	//--------------------------------------------------------

	void CInputManager::close()
	{
		// No somos responsables de la destrucción de los objetos.
		_mouse = 0;
		_keyboard = 0;
		_inputSystem = 0;

	} // close

	//--------------------------------------------------------

	void CInputManager::tick() 
	{
		// Se necesita capturar todos los dispositivos.
		if(_mouse) {
			_mouse->capture();
		}

		if(_keyboard) {
			_keyboard->capture();
		}

	} // capture

	//--------------------------------------------------------

	void CInputManager::addKeyListener(CKeyboardListener *keyListener) 
	{
		if(_keyboard)
			_keyListeners.push_front(keyListener);
		
	} // addKeyListener

	//--------------------------------------------------------

	void CInputManager::addMouseListener(CMouseListener *mouseListener) 
	{
		if(_mouse)
			_mouseListeners.push_front(mouseListener);
		
	} // addMouseListener

	//--------------------------------------------------------

	void CInputManager::removeKeyListener(CKeyboardListener *keyListener) 
	{
		_keyListeners.remove(keyListener);

	} // removeKeyListener

	//--------------------------------------------------------

	void CInputManager::removeMouseListener(CMouseListener *mouseListener) 
	{
		_mouseListeners.remove(mouseListener);

	} // removeMouseListener

	//--------------------------------------------------------

	void CInputManager::removeAllListeners() 
	{
		_keyListeners.clear();
		_mouseListeners.clear();

	} // removeAllListeners

	//--------------------------------------------------------

	void CInputManager::removeAllKeyListeners() 
	{
		_keyListeners.clear();

	} // removeAllKeyListeners

	//--------------------------------------------------------

	void CInputManager::removeAllMouseListeners() 
	{
		_mouseListeners.clear();

	} // removeAllMouseListeners

	//--------------------------------------------------------

	bool CInputManager::keyPressed(const OIS::KeyEvent &e) 
	{
		if (!_keyListeners.empty()) 
		{
			std::list<CKeyboardListener*>::const_iterator it;
			it = _keyListeners.begin();
			for (; it != _keyListeners.end(); it++) 
			{
				if ((*it)->keyPressed(ois2galeon(e)))
				  return true;
			}
		}

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CInputManager::keyReleased(const OIS::KeyEvent &e) 
	{
		if (!_keyListeners.empty()) 
		{
			std::list<CKeyboardListener*>::const_iterator it;
			it = _keyListeners.begin();
			for (; it != _keyListeners.end(); it++) 
			{
				if ((*it)->keyReleased(ois2galeon(e)))
				  return true;
			}
		}

		return false;

	} // keyReleased

	//--------------------------------------------------------

	TKey CInputManager::ois2galeon(const OIS::KeyEvent &e)
	{
		unsigned int text = e.text;
		//Las teclas del numpad no vienen con el texto. Lo metemos a mano.
        switch (e.key)
        {
        case OIS::KC_DECIMAL:
                text = 46;
                break;
        case OIS::KC_DIVIDE:
                text = 47;
                break;
        case OIS::KC_NUMPAD0:
                text = 48;
                break;
        case OIS::KC_NUMPAD1:
                text = 49;
                break;
        case OIS::KC_NUMPAD2:
                text = 50;
                break;
        case OIS::KC_NUMPAD3:
                text = 51;
                break;
        case OIS::KC_NUMPAD4:
                text = 52;
                break;
        case OIS::KC_NUMPAD5:
                text = 53;
                break;
        case OIS::KC_NUMPAD6:
                text = 54;
                break;
        case OIS::KC_NUMPAD7:
                text = 55;
                break;
        case OIS::KC_NUMPAD8:
                text = 56;
                break;
        case OIS::KC_NUMPAD9:
                text = 57;
                break;
        }

		return TKey(text,(const Key::TKeyID)e.key);

	}


	//--------------------------------------------------------

	bool CInputManager::mouseMoved(const OIS::MouseEvent &e) 
	{
		if (!_mouseListeners.empty()) 
		{
			// Actualizamos el estado antes de enviarlo
			_mouseState.setExtents(e.state.width, e.state.height);
			_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			_mouseState.movX = e.state.X.rel;
			_mouseState.movY = e.state.Y.rel;
			_mouseState.scrool = e.state.Z.rel;
			_mouseState.button = Button::UNASSIGNED;

			std::list<CMouseListener*>::const_iterator it;
			it = _mouseListeners.begin();
			for (; it != _mouseListeners.end(); it++) 
			{
				if ((*it)->mouseMoved(_mouseState))
				  return true;
			}
		}

		return false;

	} // mouseMoved

	//--------------------------------------------------------

	bool CInputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID button) 
	{
		if (!_mouseListeners.empty()) 
		{
			// Actualizamos el estado antes de enviarlo
			_mouseState.setExtents(e.state.width, e.state.height);
			_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			_mouseState.movX = e.state.X.rel;
			_mouseState.movY = e.state.Y.rel;
			_mouseState.scrool = e.state.Z.rel;
			_mouseState.button = (TButton)button;

			std::list<CMouseListener*>::const_iterator it;
			it = _mouseListeners.begin();
			for (; it != _mouseListeners.end(); it++) 
			{
				if ((*it)->mousePressed(_mouseState))
				  return true;
			}
		}

		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CInputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID button) 
	{
		if (!_mouseListeners.empty()) 
		{
			// Actualizamos el estado antes de enviarlo
			_mouseState.setExtents(e.state.width, e.state.height);
			_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			_mouseState.movX = e.state.X.rel;
			_mouseState.movY = e.state.Y.rel;
			_mouseState.scrool = e.state.Z.rel;
			_mouseState.button = (TButton)button;

			std::list<CMouseListener*>::const_iterator it;
			it = _mouseListeners.begin();
			for (; it != _mouseListeners.end(); it++) 
			{
				if ((*it)->mouseReleased(_mouseState))
				  return true;
			}
		}

		return false;

	} // mouseReleased
	
} // namespace GUI