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

	bool CAnimatedEntity::setAnimation(const std::string &anim, float moment, bool loop, std::list<float>* eventChain)
	{
		assert(_entity && "La entidad no ha sido cargada en la escena");
		_paused=false;
		_ticksPaused=0;
		_activeEventChain=eventChain;	

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
		
	bool CAnimatedEntity::stopAnimation(const std::string &anim)
	{
		assert(_entity  && "La entidad no ha sido cargada en la escena");

		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;

		Ogre::AnimationState *animation = _entity->getAnimationState(anim);
			animation->setEnabled(false);
			if( animation->hasEnded() )			// [f®§] Necesario para resetear animaciones finitas (loop = false).
				animation->setTimePosition(0);  // De lo contrario, no dejan de lanzar el evento finished a los observers

		// Si la animación a parar es la animación activa ya no lo estará.
		if(animation == _currentAnimation)
			_currentAnimation = 0;

		return true;
	} // stopAnimation

	//--------------------------------------------------------

	bool CAnimatedEntity::pauseAnimation(const std::string &anim,float moment)
	{
		assert(_entity  && "La entidad no ha sido cargada en la escena");
		_ticksPaused=0;
		if (!_paused)
		{
			_paused=true;			
		}else
		{
			_paused=false;
			return false;
		}
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		Ogre::AnimationState *animation = _entity->getAnimationState(anim);
		//animation->setEnabled(false);
		if( animation->hasEnded() )			// [f®§] Necesario para resetear animaciones finitas (loop = false).
		animation->setTimePosition(moment);  // De lo contrario, no dejan de lanzar el evento finished a los observer
	} // pauseAnimation

	bool CAnimatedEntity::pauseAnimationXTicks(const std::string &anim,float moment, unsigned int ticks)
	{
		_maxTicks=ticks;
		pauseAnimation(anim, moment);
		return true;
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
		//PeP, este tick se ha quedado en los huesos, si hay aun ciertas cosas es por la funcionalidad de poder pausar una animación, cosa que se maneja en parte en el tick.
		if(_currentAnimation)
		{
			if (_rewinding)
			{
				_currentAnimation->addTime(-secs);
				if (_currentAnimation->getTimePosition()<=0)
				{
					_observer->animationFinished(_currentAnimation->getAnimationName());
					_rewinding=false;
				}
			}	
			else
				if (!_paused)
					_currentAnimation->addTime(secs);
				else
					_ticksPaused++;
			
			if (_paused && _maxTicks>0 && _ticksPaused>_maxTicks)
			{
				_ticksPaused=0;
				_maxTicks=0;
				_paused=false;
			}
			if(_observer)
				if (_currentAnimation->getTimePosition()<0.2 ) 
					_momentEnabled=true;
			
			if (_activeEventChain!=NULL)
				if(_observer && !_activeEventChain->empty() && _activeEventChain->front()<_currentAnimation->getTimePosition())
					if (_momentEnabled)
						{
							_momentEnabled=false;
							//_activeEventChain->pop_front();
							_observer->animationMomentReached(_currentAnimation->getAnimationName());
						}			
				// Comprobamos si la animaci?n ha terminado para avisar
			if(_observer && _currentAnimation->hasEnded())
				_observer->animationFinished(_currentAnimation->getAnimationName());
		}

	} // tick

} // namespace Graphics
