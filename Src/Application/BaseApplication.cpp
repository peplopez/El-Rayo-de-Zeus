//---------------------------------------------------------------------------
// BaseApplication.cpp
//---------------------------------------------------------------------------

/**
@file BaseApplication.cpp

Contiene la implementación de la clase aplicacion, que maneja la ejecución
de todo el juego.

@see Application::CBaseApplication
@see Application::CApplicationState

@author Marco Antonio Gómez Martín & David Llansó
@date Julio, 2010
*/

#include "BaseApplication.h"
#include "ApplicationState.h"
#include "Clock.h"

#include <assert.h>

namespace Application {

	CBaseApplication *CBaseApplication::_instance = 0;

	CBaseApplication::CBaseApplication() : 
		_initialized(false),
		_currentState(0),
		_nextState(0),
		_exit(false),
		_clock(0)
	{
		assert(!_instance && "No puede crearse más de una aplicación");

		_instance = this;

	} // CBaseApplication

	//--------------------------------------------------------

	CBaseApplication::~CBaseApplication()
	{
		_instance = 0;

	} // ~CBaseApplication

	//--------------------------------------------------------

	bool CBaseApplication::init() 
	{
		assert(!_initialized && "La aplicación ya está inicializada");

		_initialized = true;

		return true;

	} // init

	//--------------------------------------------------------

	void CBaseApplication::release()
	{
		assert(_initialized && "La aplicación no está inicializada");

		// Desactivamos y eliminamos todos los estados.
		releaseAllStates();

		_initialized = false;

	} // release

	//--------------------------------------------------------

	void CBaseApplication::releaseAllStates()
	{
		// Desactivamos el estado actual
		if (_currentState)
		{
			_currentState->deactivate();
			_currentState = 0;
		}

		// Eliminamos los estados

		TStateTable::const_iterator it, end;

		for (it = _states.begin(), end = _states.end(); 
			 it != end; ++it) 
		{
			it->second->release();
			delete it->second;
		}
		_states.clear();

	} // releaseAllStates

	//--------------------------------------------------------

	bool CBaseApplication::addState(const std::string &name,
					   CApplicationState *newState) 
	{
		TStateTable::const_iterator it;

#ifdef _DEBUG
		// Comprobamos que no existe un estado con ese nombre.
		// Otra posibilidad es no hacerlo en Debug, sino siempre,
		// y, en caso de que ya exista, eliminarlo (pues la aplicación
		// acepta la responsabilidad de borrar los estados que contiene).
		// Sin embargo, en ese caso, habría que comprobar que no es
		// el estado actual, ni el estado siguiente al que se espera ir...
		it = _states.find(name);
		assert(it == _states.end());
#endif
		_states[name] = newState;
		return newState->init();

	} // addState

	//--------------------------------------------------------

	bool CBaseApplication::setState(const std::string &name) 
	{
		// Buscamos el estado.
		TStateTable::const_iterator it;

		it = _states.find(name);

		// Si no hay ningún estado con ese nombre, no hacemos nada
		if (it == _states.end())
			return false;

		_nextState = it->second;

		return true;

	} // setState

	//--------------------------------------------------------

	void CBaseApplication::run() 
	{
		assert(_clock && "Asegurate de haber creado un reloj en el init de la clase de tu aplicacion!");

		// Actualizamos una primera vez el tiempo, antes de
		// empezar, para que el primer frame tenga un tiempo
		// de frame razonable.
		_clock->updateTime();

		// Ejecución del bucle principal. Simplemente miramos si
		// tenemos que hacer una transición de estado, y si no hay que
		// hacerla, ejecutamos la vuelta
		while (!exitRequested()) 
		{
			if (!_currentState ||
					(_nextState && (_currentState != _nextState)))
				changeState();

			_clock->updateTime();

			tick(_clock->getLastFrameDuration());
		}

	} // run

	//--------------------------------------------------------

	unsigned int CBaseApplication::getAppTime() 
	{
		return _clock->getTime();

	} // getAppTime

	//--------------------------------------------------------

	void CBaseApplication::changeState() 
	{
		// Avisamos al estado actual que le vamos a eliminar
		if (_currentState)
			_currentState->deactivate();

		assert(_nextState);
		_nextState->activate();
		_currentState = _nextState;

	} // changeState

	//--------------------------------------------------------

	void CBaseApplication::tick(unsigned int msecs) 
	{
		// Aparentemente esta función es sencilla. Aquí se pueden
		// añadir otras llamadas que sean comunes a todos los estados
		// de todas las aplicaciones.
		// El método es virtual. Si para una aplicación concreta, se
		// identifican cosas comunes a todos los estados, se pueden
		// añadir en la implementación del método de esa aplicación.

		if (_currentState)
			_currentState->tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CBaseApplication::keyPressed(GUI::TKey key)
	{
		// Avisamos al estado actual de la pulsación.
		if (_currentState)
			return _currentState->keyPressed(key);
		
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CBaseApplication::keyReleased(GUI::TKey key)
	{
		// Avisamos al estado actual del fin de la pulsación.
		if (_currentState)
			return _currentState->keyReleased(key);
		
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CBaseApplication::mouseMoved(const GUI::CMouseState &mouseState)
	{
		// Avisamos al estado actual del movimiento.
		if (_currentState)
			return _currentState->mouseMoved(mouseState);
		
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CBaseApplication::mousePressed(const GUI::CMouseState &mouseState)
	{
		// Avisamos al estado actual de la pulsación.
		if (_currentState)
			return _currentState->mousePressed(mouseState);
		
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CBaseApplication::mouseReleased(const GUI::CMouseState &mouseState)
	{
		// Avisamos al estado actual del fin de la pulsación.
		if (_currentState)
			return _currentState->mouseReleased(mouseState);
		
		return false;

	} // mouseReleased

} // namespace Application
