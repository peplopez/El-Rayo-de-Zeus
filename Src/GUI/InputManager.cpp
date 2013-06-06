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

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::PHYSIC>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace GUI{

	CInputManager *CInputManager::_instance = 0;

	//--------------------------------------------------------

	CInputManager::CInputManager() :
		_mouse(0),
		_keyboard(0),
		_joystick(0),
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

		_joystick = BaseSubsystems::CServer::getSingletonPtr()->getBufferedJoystick();
		if(_joystick)
		{
			_joystick->setEventCallback(this);
			_joystickState = new CJoystickState(_joystick->getJoyStickState().mAxes.size(), _joystick->getJoyStickState().mButtons.size());
			loadJoyBindingMap();
		}

		

		return true;

	} // open

	//--------------------------------------------------------

	void CInputManager::close()
	{
		// No somos responsables de la destrucción de los objetos.
		_mouse = 0;
		_keyboard = 0;
		_joystick = 0;
		_inputSystem = 0;

		//del joystickState sí somos responsables
		delete _joystickState;

	} // close

	//--------------------------------------------------------

	void CInputManager::loadJoyBindingMap()
	{
		//Ogre::ConfigFile cf;
  //      cf.load("controls.cfg");
  //      Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
  //      secIt.getNext();
  //      Ogre::String sectionstring = "";
  //      Ogre::String typestring = "";
  //      Ogre::String paramstring = "";
  //      while (secIt.hasMoreElements()) 
		//{
  //          sectionstring = secIt.peekNextKey();
  //          Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
  //          //Do the code for section... e.g. print sectionstring
  //          for (Ogre::ConfigFile::SettingsMultiMap::iterator setIt = settings->begin(); setIt != settings->end(); ++setIt)
		//	{
  //              typestring = setIt->first;
  //              paramstring = setIt->second;
  //              // do the code for key/value
  //          }
  //      }

		_jAxisBindings[5] = TJoyAxis::MOVEXAXIS;
		_jAxisBindings[4] = TJoyAxis::MOVEYAXIS;
		_jAxisBindings[1] = TJoyAxis::POINTERXAXIS;
		_jAxisBindings[0] = TJoyAxis::POINTERYAXIS;

		_jButtonBindings[0] = TJoyButton::ATTACK1;
		_jButtonBindings[1] = TJoyButton::ATTACK2;
		_jButtonBindings[2] = TJoyButton::JUMP;
		_jButtonBindings[3] = TJoyButton::ACTIVATE;
		_jButtonBindings[4] = TJoyButton::MODIFIER;

	}

	//--------------------------------------------------------

	TJoyButton CInputManager::getButtonForOISButton(int OISButton)
	{
        return _jButtonBindings.at(OISButton);
    }

	//--------------------------------------------------------

	TJoyAxis CInputManager::getAxisForOISAxis(int OISAxis) 
	{
        return _jAxisBindings.at(OISAxis);
    }

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

		if(_joystick) {
			_joystick->capture();
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

	void CInputManager::addJoystickListener(CJoystickListener *joystickListener) 
	{
		if(_joystick)
			_joystickListeners.push_front(joystickListener);
		
	} // addJoystickListener

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

	void CInputManager::removeJoystickListener(CJoystickListener *joystickListener) 
	{
		_joystickListeners.remove(joystickListener);

	} // removeJoystickListener

	//--------------------------------------------------------

	void CInputManager::removeAllListeners() 
	{
		_keyListeners.clear();
		_mouseListeners.clear();
		_joystickListeners.clear();

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

	void CInputManager::removeAllJoystickListeners() 
	{
		_joystickListeners.clear();

	} // removeAllJoystickListeners

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

	
	//--------------------------------------------------------

	bool CInputManager::axisMoved(const OIS::JoyStickEvent &e, int axis) 
	{
		
		if (!_joystickListeners.empty()) 
		{
			LOG("InputManager: AXIS: " << axis << "   VALUE: " << e.state.mAxes[axis].abs);
			_joystickState->_axes[getAxisForOISAxis(axis)].abs = e.state.mAxes[axis].abs;
			_joystickState->_axes[getAxisForOISAxis(axis)].rel= e.state.mAxes[axis].rel;
			std::list<CJoystickListener*>::const_iterator it;
			it = _joystickListeners.begin();
			for (; it != _joystickListeners.end(); ++it) 
			{
				if ((*it)->axisMoved(_joystickState, getAxisForOISAxis(axis)))
				  return true;
			}
		}

		return true;

	} // axisMoved

	//--------------------------------------------------------
	
	bool CInputManager::buttonPressed(const OIS::JoyStickEvent &e, int button) 
	{
		
		if (!_joystickListeners.empty()) 
		{
			_joystickState->_buttons[getButtonForOISButton(button)] = e.state.mButtons[button];
			std::list<CJoystickListener*>::const_iterator it;
			it = _joystickListeners.begin();
			for (; it != _joystickListeners.end(); ++it) 
			{
				if ((*it)->buttonPressed(_joystickState, getButtonForOISButton(button)))
				  return true;
			}
		}

		return true;

	} // buttonPressed


	//--------------------------------------------------------
	
	bool CInputManager::buttonReleased(const OIS::JoyStickEvent &e, int button) 
	{
		
		if (!_joystickListeners.empty()) 
		{
			_joystickState->_buttons[getButtonForOISButton(button)] = e.state.mButtons[button];
			std::list<CJoystickListener*>::const_iterator it;
			it = _joystickListeners.begin();
			for (; it != _joystickListeners.end(); ++it) 
			{
				if ((*it)->buttonReleased(_joystickState, getButtonForOISButton(button)))
				  return true;
			}
		}

		return true;

	} // buttonPressed
	
} // namespace GUI