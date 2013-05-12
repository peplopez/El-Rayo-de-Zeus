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
#include "Graphics/AnimSet.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageBoolFloatString.h"
#include "Logic/Entity/Messages/MessageString.h"


#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::ANIMATED>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic 
{
	IMP_FACTORY(CAnimatedGraphics);
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::detachFromMap()
	{
		_scene->remove(_graphicalEntity);
		_scene=NULL;
	}

	//---------------------------------------------------------

	void CAnimatedGraphics::attachToMap(CMap* map)
	{
		_scene = map->getGraphicScene();
		_scene->add(_graphicalEntity);
	}

	//---------------------------------------------------------

	bool CAnimatedGraphics::accept(const CMessage *message)
	{
		return CGraphics::accept(message) ||
			   message->getType() == Message::SET_ANIMATION ||
			   message->getType() == Message::STOP_ANIMATION ||
			   message->getType() == Message::REWIND_ANIMATION ||
			   message->getType() == Message::SET_ANIMATION_WITH_TIME
			   ;

	} // accept
	
	//---------------------------------------------------------

	void CAnimatedGraphics::process(CMessage *message)
	{
		CGraphics::process(message);
		
		
		switch(message->getType())
		{
			case Message::SET_ANIMATION: {
				CMessageBoolUShort *rxMsg = static_cast<CMessageBoolUShort*>(message);
				// Paramos todas las animaciones antes de poner una nueva.
				// Un control más sofisticado debería permitir interpolación
				// de animaciones. Galeon no lo plantea.
				_graphicalEntity->stopAllAnimations();
				Logic::AnimationName name=static_cast<Logic::AnimationName>(rxMsg->getUShort());
				std::string animString = _animSet->getAnimation(name);
				if (_graphicalEntity->setAnimation(animString, 0, rxMsg ->getBool(),_animSet->getEventChain(name)))
					_currentLogicAnimation=name; //Tengo actualizada mi animación lógica actual
				else
					_currentLogicAnimation=Logic::AnimationName::NONE; //Tengo actualizada mi animación lógica actual
				
				LOG("SET_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::STOP_ANIMATION:	{
				CMessageUShort *rxMsg = static_cast<CMessageUShort*>(message);
				Logic::AnimationName name=static_cast<Logic::AnimationName>(rxMsg->getUShort());
				std::string animString = _animSet->getAnimation(name);
				_graphicalEntity->stopAnimation(animString);
				_currentLogicAnimation=Logic::AnimationName::NONE;
				LOG("STOP_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::REWIND_ANIMATION:	{
				CMessageBoolUShort *rxMsg = static_cast<CMessageBoolUShort*>(message);
				Logic::AnimationName name=static_cast<Logic::AnimationName>(rxMsg->getUShort());
				std::string animString = _animSet->getAnimation(name);
				_graphicalEntity->rewind(animString, rxMsg->getBool());
				LOG("REWIND_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::SET_ANIMATION_WITH_TIME:	{  //Pep, de momento esto solo lo usa el salto, pero es un buen recurso.
				CMessageBoolFloatString *rxMsg = static_cast<CMessageBoolFloatString*>(message);
				// de animaciones. Galeon no lo plantea.
				_graphicalEntity->stopAllAnimations();
				_graphicalEntity->setAnimation(rxMsg ->getString(), rxMsg ->getFloat(), rxMsg ->getBool(), NULL);//REVISAR ESTE NULL
				
				LOG("SET_ANIMATION_WITH_TIME: " << rxMsg->getString());
			} break;
		}

	} // process

	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicalEntity(const Map::CEntity *entityInfo)
	{
		assert(_scene && "LOGIC::ANIMATED_GRAPHICS>> No existe escena gráfica!");
		assert( _model.length() > 0  &&  "LOGIC::ANIMATED_GRAPHICS>> No existe modelo!");	

		std::stringstream ssAux; // FRS Importante añadir ID para evitar entidades gráficas con = nombre
			ssAux << _entity->getName() << _entity->getEntityID();
			std::string	name = ssAux.str();

		_graphicalEntity = new Graphics::CAnimatedEntity(name,_model);
			if(!_scene->add(_graphicalEntity) )
				return 0;
		
		// Cargando AnimSet de map
		_animSet = new Graphics::CAnimSet();	
		initializeAnimSet(entityInfo);
		if (_entity->getName()=="Espartano")
			int i=0;
		// DEFAULT ANIMATION
		if(entityInfo->hasAttribute("defaultAnimation")) { 
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_graphicalEntity->setAnimation(_defaultAnimation,0,true,NULL);
			_graphicalEntity->setObserver(this);
		}

		return _graphicalEntity;
	} // createGraphicsEntity

	bool CAnimatedGraphics::initializeAnimSet(const Map::CEntity *entityInfo)
	{
		assert(_animSet && "LOGIC::ANIMATED_GRAPHICS>> No existe animSet");
		// leemos directamente del map de la entidad. cogemos las animaciones. Si queremos animaciones nuevas aquí hay que ponerlas para que sean tenidas en cuenta.
		if (entityInfo->hasAttribute("animIdle"))
			_animSet->addAnimation(Logic::IDLE,entityInfo->getStringAttribute("animIdle"));
		if (entityInfo->hasAttribute("animRun"))
			_animSet->addAnimation(Logic::RUN,entityInfo->getStringAttribute("animRun"));
		if (entityInfo->hasAttribute("animDeath"))
			_animSet->addAnimation(Logic::DEATH,entityInfo->getStringAttribute("animDeath"));
		if (entityInfo->hasAttribute("animJump"))
			_animSet->addAnimation(Logic::JUMP,entityInfo->getStringAttribute("animJump"));
		if (entityInfo->hasAttribute("animDamage"))
			_animSet->addAnimation(Logic::DAMAGE,entityInfo->getStringAttribute("animDamage"));
		if (entityInfo->hasAttribute("animActivateAltar"))
			_animSet->addAnimation(Logic::ACTIVATE_ALTAR,entityInfo->getStringAttribute("animActivateAltar"));
		if (entityInfo->hasAttribute("animCoverWithWeapon"))
			_animSet->addAnimation(Logic::COVER_WITH_WEAPON,entityInfo->getStringAttribute("animCoverWithWeapon"));
		if (entityInfo->hasAttribute("animCoverWithShield"))
			_animSet->addAnimation(Logic::COVER_WITH_SHIELD,entityInfo->getStringAttribute("animCoverWithShield"));
		if (entityInfo->hasAttribute("animAttack1"))
			_animSet->addAnimation(Logic::ATTACK1,entityInfo->getStringAttribute("animAttack1"));
		if (entityInfo->hasAttribute("animAttack2"))
			_animSet->addAnimation(Logic::ATTACK2,entityInfo->getStringAttribute("animAttack2"));
		if (entityInfo->hasAttribute("animAttack3"))
			_animSet->addAnimation(Logic::ATTACK3,entityInfo->getStringAttribute("animAttack3"));
		if (entityInfo->hasAttribute("animCombo1"))
			_animSet->addAnimation(Logic::COMBO1,entityInfo->getStringAttribute("animCombo1"));
		if (entityInfo->hasAttribute("animCombo2"))
			_animSet->addAnimation(Logic::COMBO2,entityInfo->getStringAttribute("animCombo2"));
		if (entityInfo->hasAttribute("animCombo3"))
			_animSet->addAnimation(Logic::COMBO3,entityInfo->getStringAttribute("animCombo3"));

		if (entityInfo->hasAttribute("eventAttack1"))
			_animSet->addEventTime(Logic::ATTACK1,entityInfo->getFloatAttribute("eventAttack1"));
		if (entityInfo->hasAttribute("eventAttack2"))
			_animSet->addEventTime(Logic::ATTACK2,entityInfo->getFloatAttribute("eventAttack2"));
		if (entityInfo->hasAttribute("eventAttack3"))
			_animSet->addEventTime(Logic::ATTACK3,entityInfo->getFloatAttribute("eventAttack3"));
		if (entityInfo->hasAttribute("eventCover"))
			_animSet->addEventTime(Logic::COVER_WITH_SHIELD,entityInfo->getFloatAttribute("eventCover"));
		if (entityInfo->hasAttribute("eventCover"))
			_animSet->addEventTime(Logic::COVER_WITH_WEAPON,entityInfo->getFloatAttribute("eventCover"));


		return true;
	
	} // initializeAnimSet
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		assert(_animSet && "LOGIC::ANIMATED_GRAPHICS>> No existe animSet");
		assert(_currentLogicAnimation!=NONE && "LOGIC::ANIMATED_GRAPHICS>> No tenemos animación Lógica activa.");

		if (_currentLogicAnimation!=Logic::DEATH)
		{
			// [ƒ®§] Ejemplo de gestión de eventos de animación -> En este caso se avisa de que animación ha finalizado (necesario en CDeath)
			CMessageUShort *txMsg = new CMessageUShort();
			txMsg->setType(Message::ANIMATION_FINISHED);
			txMsg->setUShort(_currentLogicAnimation); //PeP: envio que se ha finalizado la animación que se está reproduciendo.
			//PEP HACK:
			/*if (animation==Graphics::AnimNames::JUMP)  //habrá que comprobar que quitando esto no pasa nada raro, de momento así es
				txMsg->setAction(Message::JUMP);
			
			*/_entity->emitMessage(txMsg);
		// Si acaba una animación y tenemos una por defecto la ponemos
			if (_currentLogicAnimation != Logic::ATTACK1 && _currentLogicAnimation != Logic::ATTACK2)			
			{
				_graphicalEntity->stopAnimation(animation);
				_graphicalEntity->setAnimation(_defaultAnimation,0,true,NULL); //tenemos que cambiar defaultanimation por un enum Logico
			}
			else
			{
				if (_currentLogicAnimation == Logic::ATTACK1)						
					_graphicalEntity->pauseAnimationXTicks(animation,0.5833,10);
			    if (_currentLogicAnimation == Logic::ATTACK2)			
					_graphicalEntity->pauseAnimationXTicks(animation,0.41,10);
			}
		}
	}
		
	void CAnimatedGraphics::animationMomentReached(const std::string &animation)
	{
		assert(_animSet && "LOGIC::ANIMATED_GRAPHICS>> No existe animSet");
		assert(_currentLogicAnimation!=NONE && "LOGIC::ANIMATED_GRAPHICS>> No tenemos animación Lógica activa.");

		if (_currentLogicAnimation==Logic::COVER_WITH_SHIELD || _currentLogicAnimation==Logic::COVER_WITH_WEAPON)
		{
			_graphicalEntity->pauseAnimation(animation,_animSet->getEventChain(_currentLogicAnimation)->front() );
		}
		else
		{
			CMessageUShort *msg = new CMessageUShort();
			msg->setType(Message::ANIMATION_MOMENT);
			msg->setUShort(_currentLogicAnimation);
			_entity->emitMessage(msg);
		}
	}

} // namespace Logic

