//---------------------------------------------------------------------------
// Clock.cpp
//---------------------------------------------------------------------------

/**
@file Clock.cpp

Contiene la implementación de un interfaz para un temporizador.

@see Application::IClock

@author David Llansó
@date Julio, 2010
*/

#include "Clock.h"

namespace Application {

	void IClock::updateTime() {
		unsigned long newTime = getPhysicalTime();
		_lastFrameDuration = newTime - _lastTime;
		_lastTime = newTime;
	}

} // namespace Application
