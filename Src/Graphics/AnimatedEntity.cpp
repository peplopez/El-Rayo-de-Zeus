//---------------------------------------------------------------------------
// AnimatedEntity.cpp
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.cpp

Contiene la implementación de la clase que representa una entidad gráfica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "AnimatedEntity.h"


#include <assert.h>
#include <OgreEntity.h>
#include <OgreAnimationState.h>

namespace Graphics 
{
	

	bool CAnimatedEntity::load()
	{
		try{

			_loaded = CEntity::load();
			
			/**
			HACK - ESC Para que al cambiar de escena mantengamos la animacion
			*/
			setAnimation(_currentAnimationName, 0, true, NULL);


		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;
	} // load

	//--------------------------------------------------------

	bool CAnimatedEntity::setAnimation(const std::string &anim, float moment, bool loop, std::vector<std::pair<unsigned short,float>>* eventChain)
	{
		assert(_entity && "La entidad no ha sido cargada en la escena");
		_paused = false;
		_secsPaused = 0.0f;
		_activeEventChain = eventChain;	
		_index = 0;
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		
		_currentAnimationName = anim;
		_currentAnimation = _entity->getAnimationState(anim);
		_currentAnimation->setEnabled(true);
		_currentAnimation->setTimePosition(moment);
		_currentAnimation->setLoop(loop);

		return true;

	} // setAnimation

	//--------------------------------------------------------
		
	bool CAnimatedEntity::stopAnimation()
	{
		assert(_entity  && "La entidad no ha sido cargada en la escena");

		if(!_entity->getAllAnimationStates()->hasAnimationState(_currentAnimationName))
			return false;

		Ogre::AnimationState *animation = _entity->getAnimationState(_currentAnimationName);
			animation->setEnabled(false);
			if( animation->hasEnded() )			// [f®§] Necesario para resetear animaciones finitas (loop = false).
				animation->setTimePosition(0);  // De lo contrario, no dejan de lanzar el evento finished a los observers

		// Si la animación a parar es la animación activa ya no lo estará.
		if(animation == _currentAnimation)
			_currentAnimation = 0;

		return true;
	} // stopAnimation

	//--------------------------------------------------------

	void CAnimatedEntity::setAnimationTime(float moment)
	{
		assert(_entity  && "La entidad no ha sido cargada en la escena");

		_currentAnimation->setTimePosition(moment);

	} // 

	//--------------------------------------------------------

	void CAnimatedEntity::pauseAnimation(float moment)
	{
		assert(_entity  && "La entidad no ha sido cargada en la escena");

		_pauseRequested = true;
		_maxSecs = -1.0f;
		_secsPaused = 0.0f;
		_timeToPause = moment;

	} //

	//--------------------------------------------------------

	void CAnimatedEntity::pauseAnimationXsecs(float moment, float secs)
	{
		assert(_entity  && "La entidad no ha sido cargada en la escena");

		_pauseRequested = true;
		_maxSecs = secs;
		_secsPaused = 0.0f;
		_timeToPause = moment;
	}

	//--------------------------------------------------------

	void CAnimatedEntity::pauseAnimationXsecs(float secs)
	{
		assert(_entity  && "La entidad no ha sido cargada en la escena");

		_paused = true;
		_maxSecs = secs;
		_secsPaused = 0.0f;
		_timeToPause = 0;
	}

	//--------------------------------------------------------
	
	void CAnimatedEntity::resumeAnimation()
	{
		if (_paused)
		{
			_paused = false;
			_secsPaused = 0.0f;
			_maxSecs = 0.0f;
		}
	}

	//--------------------------------------------------------
		
	void CAnimatedEntity::stopAllAnimations()
	{
		assert(_entity  && "La entidad no ha sido cargada en la escena");

		if(_entity->getAllAnimationStates()->hasEnabledAnimationState())
		{
			Ogre::ConstEnabledAnimationStateIterator it = 
				_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
			Ogre::AnimationState *animation;

			//hay que recorrer con el iterador las animaciones una a una y pausarlas
			while(it.hasMoreElements())
			{
				animation = it.getNext();
				animation->setEnabled(false);
				if( animation->hasEnded() )			// [f®§] Necesario para resetear animaciones finitas (loop = false).
					animation->setTimePosition(0);  // De lo contrario, no dejan de lanzar el evento finished a los observers
			}

			// Si había animación activa ya no lo está.
			_currentAnimation = 0;
		}

	} // stopAllAnimations

	//--------------------------------------------------------
	
	void CAnimatedEntity::tick(float secs)
	{
		//PeP, este tick se ha quedado en los huesos, si hay aun ciertas cosas es por la funcionalidad de poder pausar una animación,
		//cosa que se maneja en parte en el tick.
		if(_currentAnimation)
		{
			if (_pauseRequested)
				if (_currentAnimation->getTimePosition() + secs >= _timeToPause)
				{
					_currentAnimation->setTimePosition(_timeToPause);
					_secsPaused = _currentAnimation->getTimePosition() + secs - _timeToPause;
					_paused = true;
					_pauseRequested = false;

				}

			if (_rewinding)
			{
				_currentAnimation->addTime(-secs);
				if (_currentAnimation->getTimePosition()<=0)
				{
					_observer->animationFinished(std::pair<unsigned short,float>(0,0)); //correspondiente a ANIMATION_END
					_rewinding=false;
				}
			}	
			else
			{
				if (_paused)
				{
					_secsPaused += secs;
					if (_maxSecs > 0.0f && _secsPaused > _maxSecs)
					{
						_currentAnimation->addTime(_secsPaused - _maxSecs);
						_paused = false;
					}

				}
				else
					_currentAnimation->addTime(secs);
			}
			

			
			if (_activeEventChain!=NULL)
			{
				if(_observer && !_activeEventChain->empty())
				{
					while (_index < _activeEventChain->size() && _activeEventChain->at(_index).second < _currentAnimation->getTimePosition())
					{
						_observer->animationMomentReached(_activeEventChain->at(_index));
						++_index;
					}
				}			
			}

			if(_observer && _currentAnimation->hasEnded())
			{
				if (!_paused)
					_observer->animationFinished(std::pair<unsigned short,float>(1,0)); //correspondiente a ANIMATION_END
			}
		}

	} // tick

} // namespace Graphics
