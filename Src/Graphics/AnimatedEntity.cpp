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
		
	bool CAnimatedEntity::setAnimation(const std::string &anim, bool loop)
	{
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		_currentAnimation = _entity->getAnimationState(anim);
		_currentAnimation->setEnabled(true);
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
		// Si la animación a parar es la animación activa ya no lo estará.
		if(animation == _currentAnimation)
			_currentAnimation = 0;
		return true;

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
			_currentAnimation->addTime(secs);
			// Comprobamos si la animación ha terminado para avisar
			if(_observer && _currentAnimation->hasEnded())
				_observer->animationFinished
							(_currentAnimation->getAnimationName());
		}

	} // tick


} // namespace Graphics
