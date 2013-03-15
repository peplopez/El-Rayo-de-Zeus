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
		
	bool CAnimatedEntity::setAnimation(const std::string &anim, float moment, bool loop)
	{

		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		_currentAnimation = _entity->getAnimationState(anim);
		_currentAnimation->setEnabled(true);
		_currentAnimation->setTimePosition(moment);
		_currentAnimation->setLoop(loop);
		return true;

	} // setAnimation

	//--------------------------------------------------------
		
	bool CAnimatedEntity::stopAnimation(const std::string &anim)
	{
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
		
	bool CAnimatedEntity::rewind(const std::string &anim,const bool moment)
	{
		_rewinding=true;
		/*if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		Ogre::AnimationState *animation = _entity->getAnimationState(anim);
		animation->setTimePosition(secs);
		*///if( animation->hasEnded() )			// [f®§] Necesario para resetear animaciones finitas (loop = false).
			//animation->setTimePosition(0);  // De lo contrario, no dejan de lanzar el evento finished a los observers

		// Si la animación a parar es la animación activa ya no lo estará.
		/*if(animation == _currentAnimation)
			_currentAnimation = 0;

		*/		return true;
	} // stopAnimation


	//--------------------------------------------------------
		
	void CAnimatedEntity::stopAllAnimations()
	{
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
		if(_currentAnimation)
		{
			if (_rewinding)
			{_currentAnimation->addTime(-secs);
				if (_currentAnimation->getTimePosition()<=0)
				{
					
					_observer->animationFinished(_currentAnimation->getAnimationName());
					_rewinding=false;
				}
			}	
		//	if (_currentAnimation->getTimePosition()<0.1)
	//			_rewinding=false;
			else
				_currentAnimation->addTime(secs);
			// Comprobamos si la animaci?n ha terminado para avisar
			if(_observer && _currentAnimation->hasEnded())
				_observer->animationFinished(_currentAnimation->getAnimationName());

			
			if(_observer && _currentAnimation->getAnimationName().compare("FireKatana")==0)
				if (_currentAnimation->getTimePosition()<0.2 ) _momentEnabled=true;
			if(_observer && _currentAnimation->getAnimationName().compare("GetObject")==0)
				if (_currentAnimation->getTimePosition()<0.2 ) _momentEnabled=true;

			if(_observer && _currentAnimation->getAnimationName().compare("FireKatana")==0)
				if (_momentEnabled)
				if (_currentAnimation->getTimePosition()>0.6 )
				{
					_momentEnabled=false;
					_observer->animationMomentReached("FireKatana");
				}
			if(_observer && _currentAnimation->getAnimationName().compare("GetObject")==0)
				if (_momentEnabled)
				if (_currentAnimation->getTimePosition()>0.6)
				{
					_momentEnabled=false;				
					_observer->animationMomentReached("GetObject");
				}
		}

	} // tick

} // namespace Graphics
