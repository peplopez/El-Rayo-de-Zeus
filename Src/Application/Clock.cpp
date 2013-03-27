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
#include "BaseApplication.h"

namespace Application {

	void IClock::updateTime() {
		unsigned long newTime = getPhysicalTime();
		_lastFrameDuration = newTime - _lastTime;
		_lastTime = newTime;
	}

	void IClock::timeRequest(const unsigned long x)//		void timeRequest(IClockListener* timeObserver,const unsigned long x);
	{
		unsigned long initialTime = getPhysicalTime();		
		unsigned long destityTime = getPhysicalTime()+x;
		
	do{
		//_contando++;
		//
		updateTime();
		} while (_lastTime<destityTime);
	
		_observer->timeArrived();
	}

} // namespace Application
