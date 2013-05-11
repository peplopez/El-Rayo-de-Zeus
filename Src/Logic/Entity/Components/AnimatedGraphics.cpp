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
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolFloatString.h"

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

				CMessageBoolString *rxMsg = static_cast<CMessageBoolString*>(message);
				// Paramos todas las animaciones antes de poner una nueva.
				// Un control más sofisticado debería permitir interpolación
				// de animaciones. Galeon no lo plantea.
				_graphicalEntity->stopAllAnimations();
				_graphicalEntity->setAnimation(rxMsg ->getString(), 0, rxMsg ->getBool());
				LOG("SET_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::STOP_ANIMATION:	{
				CMessageString *rxMsg = static_cast<CMessageString*>(message);
				_graphicalEntity->stopAnimation(rxMsg ->getString());
				LOG("STOP_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::REWIND_ANIMATION:	{
				CMessageBoolString *rxMsg = static_cast<CMessageBoolString*>(message);
				_graphicalEntity->rewind(rxMsg ->getString(), rxMsg->getBool() );
				LOG("REWIND_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::SET_ANIMATION_WITH_TIME:
			{
				CMessageBoolFloatString *rxMsg = static_cast<CMessageBoolFloatString*>(message);
				// de animaciones. Galeon no lo plantea.
				_graphicalEntity->stopAllAnimations();
				_graphicalEntity->setAnimation(rxMsg ->getString(), rxMsg ->getFloat(), rxMsg ->getBool());
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

		// DEFAULT ANIMATION
		if(entityInfo->hasAttribute("defaultAnimation")) { 
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_graphicalEntity->setAnimation(_defaultAnimation,0,true);
			_graphicalEntity->setObserver(this);
		}

		return _graphicalEntity;
	} // createGraphicsEntity

	bool CAnimatedGraphics::initializeAnimSet(const Map::CEntity *entityInfo)
	{
		assert(_animSet && "LOGIC::ANIMATED_GRAPHICS>> No existe animSet");
		// leemos directamente del map de la entidad. cogemos las animaciones. Si queremos animaciones nuevas aquí hay que ponerlas para que sean tenidas en cuenta.
		if (entityInfo->hasAttribute("animIdle"))
			_animSet->addAnimation(Graphics::IDLE,entityInfo->getStringAttribute("animIdle"));
		if (entityInfo->hasAttribute("animRun"))
			_animSet->addAnimation(Graphics::RUN,entityInfo->getStringAttribute("animRun"));
		if (entityInfo->hasAttribute("animDeath"))
			_animSet->addAnimation(Graphics::DEATH,entityInfo->getStringAttribute("animDeath"));
		if (entityInfo->hasAttribute("animJump"))
			_animSet->addAnimation(Graphics::JUMP,entityInfo->getStringAttribute("animJump"));
		if (entityInfo->hasAttribute("animDamage"))
			_animSet->addAnimation(Graphics::DAMAGE,entityInfo->getStringAttribute("animDamage"));
		if (entityInfo->hasAttribute("animActivateAltar"))
			_animSet->addAnimation(Graphics::ACTIVATE_ALTAR,entityInfo->getStringAttribute("animActivateAltar"));
		if (entityInfo->hasAttribute("animCoverWithWeapon"))
			_animSet->addAnimation(Graphics::COVER_WITH_WEAPON,entityInfo->getStringAttribute("animCoverWithWeapon"));
		if (entityInfo->hasAttribute("animCoverWithShield"))
			_animSet->addAnimation(Graphics::COVER_WITH_SHIELD,entityInfo->getStringAttribute("animCoverWithShield"));
		if (entityInfo->hasAttribute("animAttack1"))
			_animSet->addAnimation(Graphics::ATTACK1,entityInfo->getStringAttribute("animAttack1"));
		if (entityInfo->hasAttribute("animAttack2"))
			_animSet->addAnimation(Graphics::ATTACK2,entityInfo->getStringAttribute("animAttack2"));
		if (entityInfo->hasAttribute("animAttack3"))
			_animSet->addAnimation(Graphics::ATTACK3,entityInfo->getStringAttribute("animAttack3"));
		if (entityInfo->hasAttribute("animCombo1"))
			_animSet->addAnimation(Graphics::COMBO1,entityInfo->getStringAttribute("animCombo1"));
		if (entityInfo->hasAttribute("animCombo2"))
			_animSet->addAnimation(Graphics::COMBO2,entityInfo->getStringAttribute("animCombo2"));
		if (entityInfo->hasAttribute("animCombo3"))
			_animSet->addAnimation(Graphics::COMBO3,entityInfo->getStringAttribute("animCombo3"));

		return true;
	
	}
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		if (animation != Graphics::AnimNames::DEATH )
		{
			// [ƒ®§] Ejemplo de gestión de eventos de animación -> En este caso se avisa de que animación ha finalizado (necesario en CDeath)
			CMessageString *txMsg = new CMessageString();
			txMsg->setType(Message::ANIMATION_FINISHED);
			txMsg->setString(animation);
			//PEP HACK:
			if (animation==Graphics::AnimNames::JUMP)
				txMsg->setAction(Message::JUMP);
			_entity->emitMessage(txMsg);
			
		// Si acaba una animación y tenemos una por defecto la ponemos
			if (animation != Graphics::AnimNames::ATTACK1 && animation != Graphics::AnimNames::ATTACK2)			
			{
				_graphicalEntity->stopAnimation(animation);
				_graphicalEntity->setAnimation(_defaultAnimation,0,true);
			}
			else
			{
				if (animation == Graphics::AnimNames::ATTACK1)						
					_graphicalEntity->pauseAnimation(animation,0.5833);
			    if (animation == Graphics::AnimNames::ATTACK2)			
					_graphicalEntity->pauseAnimation(animation,0.41);
			}   
			
		}

	}

		
	void CAnimatedGraphics::animationMomentReached(const std::string &animation)
	{
		// [ƒ®§] Ejemplo de gestión de eventos de animación -> En este caso se avisa de que animación ha finalizado (necesario en CDeath)
		CMessageString *msg = new CMessageString();
		msg->setType(Message::ANIMATION_MOMENT);
		msg->setString(animation);
		_entity->emitMessage(msg);
	}

} // namespace Logic

