/**
@file AnimatedGraphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de una entidad estática.
 
@see Logic::CAnimatedGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "AnimatedGraphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"

namespace Logic 
{
	IMP_FACTORY(CAnimatedGraphics);
	
	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		_animatedGraphicsEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model);
		if(!_scene->addEntity(_animatedGraphicsEntity))
			return 0;

		_animatedGraphicsEntity->setTransform(_entity->getTransform());
		
		if(entityInfo->hasAttribute("defaultAnimation"))
		{
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
			_animatedGraphicsEntity->setObserver(this);
		}


		return _animatedGraphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	bool CAnimatedGraphics::accept(const TMessage &message)
	{
		return CGraphics::accept(message) ||
			   message._type == Message::SET_ANIMATION ||
			   message._type == Message::STOP_ANIMATION||message._type == Message::CONTROL||message._type == Message::NPC_CONTROL;

	} // accept
	
	//---------------------------------------------------------

	void CAnimatedGraphics::process(const TMessage &message)
	{
		CGraphics::process(message);

		switch(message._type)
		{
		case Message::SET_ANIMATION:
			// Paramos todas las animaciones antes de poner una nueva.
			// Un control más sofisticado debería permitir interpolación
			// de animaciones. Galeon no lo plantea.
			_animatedGraphicsEntity->stopAllAnimations();
			_animatedGraphicsEntity->setAnimation(message._string,message._bool);
			break;
		case Message::STOP_ANIMATION:
			_animatedGraphicsEntity->stopAnimation(message._string);
			break;
		case Message::CONTROL:
			if  (message._string.compare("changeDirection")==0||message._string.compare("changeBase")==0||message._string.compare("jump")==0||message._string.compare("goUp")==0||message._string.compare("goDown")==0||message._string.compare("turn")==0)
				//mensajes que ignoro
				break;
			
			_animatedGraphicsEntity->stopAllAnimations();
			if  (message._string.compare("walkLeft")==0 || message._string.compare("walkRight")==0) 
			{
				_animatedGraphicsEntity->setAnimation("RunKatana",true);
				break;
			}
			if  (message._string.compare("walkBack")==0 || message._string.compare("walkStop")==0) 
			{
				_animatedGraphicsEntity->setAnimation("IdleKatana",true);
				break;
			}
			break;
			
		case Message::NPC_CONTROL:
			if  (message._string.compare("changeDirection")==0||message._string.compare("changeBase")==0||message._string.compare("jump")==0||message._string.compare("goUp")==0||message._string.compare("goDown")==0||message._string.compare("turn")==0)
				//mensajes que ignoro
				break;
			
			_animatedGraphicsEntity->stopAllAnimations();
			if  (message._string.compare("walkLeft")==0 || message._string.compare("walkRight")==0) 
			{
				_animatedGraphicsEntity->setAnimation("RunKatana",true);
				break;
			}
			if  (message._string.compare("walkBack")==0 || message._string.compare("walkStop")==0) 
			{
				_animatedGraphicsEntity->setAnimation("IdleKatana",true);
				break;
			}
			
			break;
		}

	} // process
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		// Si acaba una animación y tenemos una por defecto la ponemos
		_animatedGraphicsEntity->stopAllAnimations();
		_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
	}

} // namespace Logic

