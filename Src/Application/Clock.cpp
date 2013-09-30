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

	void IClock::updateTime() 
	{
		unsigned long newTime = getPhysicalTime();
		_lastFrameDuration = newTime - _lastTime;
		_lastTime = newTime;
	}

	void IClock::removeTimeObservers(IClockListener* listener)
	{
		// l.remove_if([](int n){ return n > 10; }); // remove all elements greater than 10
		if (!_timeObservers.empty())
			_timeObservers.remove_if([listener](std::pair<IClockListener*,unsigned long> pai) 
											{return pai.first!=NULL? listener==pai.first: false;});
	}

	void IClock::removeAllTimeObservers()
	{			
		_timeObservers.clear();
	}


} // namespace Application
