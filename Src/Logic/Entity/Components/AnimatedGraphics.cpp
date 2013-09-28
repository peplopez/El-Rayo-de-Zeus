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
#include "Map/Entity.h"

#include "Graphics/Scene.h"
#include "Graphics/AnimSet.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageFloatUShort.h"
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
			   message->getType() == Message::SET_ANIMATION_WITH_TIME ||
			   message->getType() == Message::SET_ANIMATION_WITH_PAUSE ||
			   message->getType() == Message::RESUME_ANIMATION
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
				if (_graphicalEntity->setAnimation(animString, 0, rxMsg->getBool(), _animSet->getEventChain(name)))
					_currentLogicAnimation=name; //Tengo actualizada mi animación lógica actual
				else
					_currentLogicAnimation=Logic::AnimationName::NONE; //Tengo actualizada mi animación lógica actual
				
				LOG("SET_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::STOP_ANIMATION:	{
				CMessageUShort *rxMsg = static_cast<CMessageUShort*>(message);
				Logic::AnimationName name=static_cast<Logic::AnimationName>(rxMsg->getUShort());
				std::string animString = _animSet->getAnimation(name);
				_graphicalEntity->stopAnimation();
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

			case Message::RESUME_ANIMATION: {
				_graphicalEntity->resumeAnimation();
				LOG("RESUMING ANIMATION");
			}	break;

			case Message::SET_ANIMATION_WITH_TIME:	{  //Pep, de momento esto solo lo usa el salto, pero es un buen recurso. // de animaciones. Galeon no lo plantea.
				CMessageFloatUShort *rxMsg = static_cast<CMessageFloatUShort*>(message);
				_graphicalEntity->stopAllAnimations();
				Logic::AnimationName name = static_cast<Logic::AnimationName>(rxMsg->getUShort());
				std::string animString = _animSet->getAnimation(name);
				_graphicalEntity->setAnimation(animString, rxMsg->getFloat(), false, 0); //REVISAR ESTE NULL
				_currentLogicAnimation=name;
				LOG("SET_ANIMATION_WITH_TIME: " << rxMsg->getString());

			} break;

			case Message::SET_ANIMATION_WITH_PAUSE:	{  
				CMessageFloatUShort *rxMsg = static_cast<CMessageFloatUShort*>(message);
				_graphicalEntity->stopAllAnimations();
				Logic::AnimationName name = static_cast<Logic::AnimationName>(rxMsg->getUShort());
				std::string animString = _animSet->getAnimation(name);
				_graphicalEntity->setAnimation(animString, 0, false, 0); //REVISAR ESTE NULL
				_graphicalEntity->pauseAnimation(rxMsg->getFloat());
				_currentLogicAnimation=name;
				LOG("SET_ANIMATION_WITH_PAUSE: " << rxMsg->getString());
			} break;
		}

	} // process

	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicalEntity(const Map::CEntity *entityInfo)
	{
		assert(_scene && "LOGIC::ANIMATED_GRAPHICS>> No existe escena gráfica!");
		assert( _model.length() > 0  &&  "LOGIC::ANIMATED_GRAPHICS>> No existe modelo!");	

		_graphicalEntity = new Graphics::CAnimatedEntity(_entity->getGraphicalName(),_model);
		if(!_scene->add(_graphicalEntity) )
			return 0;

		// Cargando AnimSet de map
		_animSet = new Graphics::CAnimSet();	
		initializeAnimSet(entityInfo);

		// DEFAULT ANIMATION
		if(entityInfo->hasAttribute("defaultAnimation")) { 
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_graphicalEntity->setAnimation(_defaultAnimation,0,true,NULL); //Pep, cuando veais esto con NULL sabed que es temporal y que soy consciente.
			
		}
		_graphicalEntity->setObserver(this);
		return _graphicalEntity;
	} // createGraphicsEntity

	//---------------------------------------------------------

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
			_animSet->addAnimation(Logic::SWITCH_ALTAR,entityInfo->getStringAttribute("animActivateAltar"));
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

		if (entityInfo->hasAttribute("event_CT_Attack1"))
			_animSet->addEventTime(Logic::ATTACK1, Logic::COMBO_TRACK, entityInfo->getFloatAttribute("event_CT_Attack1"));
		if (entityInfo->hasAttribute("event_CT_Attack2"))
			_animSet->addEventTime(Logic::ATTACK2, Logic::COMBO_TRACK, entityInfo->getFloatAttribute("event_CT_Attack2"));
		if (entityInfo->hasAttribute("event_CT_Attack3"))
			_animSet->addEventTime(Logic::ATTACK3, Logic::COMBO_TRACK, entityInfo->getFloatAttribute("event_CT_Attack3"));

		
		if (entityInfo->hasAttribute("event_DT_Cover"))
			_animSet->addEventTime(Logic::COVER_WITH_SHIELD, Logic::COVER_MOMENT, entityInfo->getFloatAttribute("event_DT_Cover"));
		if (entityInfo->hasAttribute("event_DT_Cover"))
			_animSet->addEventTime(Logic::COVER_WITH_WEAPON, Logic::COVER_MOMENT, entityInfo->getFloatAttribute("event_DT_Cover"));
		if (entityInfo->hasAttribute("event_DT_ActivateAltar"))
			_animSet->addEventTime(Logic::SWITCH_ALTAR, Logic::SWITCH_MOMENT, entityInfo->getFloatAttribute("event_DT_ActivateAltar"));
		

	// TODO FRS ¿Y no sería mejor hacer una herencia/especialización "AnimatedGraphicsCerberus" 
	// para no andar con estas comprobaciones/excepciones? Depender de los nombres de los blueprints
	// es un riesgo (p.e. si queremos renombrar "Cancerbero" a "Cerberus"). Es mejor guardar la 
	// funcionalidad específica en un componente heredado y asignarlo al blueprint que nos apetezca.
	//		
		//especificos de cancerbero
		if (entityInfo->hasAttribute("animJumpDown"))
			_animSet->addAnimation(Logic::JUMP_DOWN,entityInfo->getStringAttribute("animJumpDown"));
		if (entityInfo->hasAttribute("animAlert"))
			_animSet->addAnimation(Logic::ALERT,entityInfo->getStringAttribute("animAlert"));
		if (entityInfo->hasAttribute("animWalk"))
			_animSet->addAnimation(Logic::WALK,entityInfo->getStringAttribute("animWalk"));
			
		if (_entity->getType()=="Cerberus")	//en el caso de cancerbero en un solo ataque tiene varios eventos Damage_track
		{			
			if (entityInfo->hasAttribute("event_DT_Attack1"))
				_animSet->addEventTime(Logic::ATTACK1, Logic::DAMAGE_TRACK, entityInfo->getFloatAttribute("event_DT_Attack1"));
			if (entityInfo->hasAttribute("event_DT_Attack2"))
				_animSet->addEventTime(Logic::ATTACK1, Logic::DAMAGE_TRACK, entityInfo->getFloatAttribute("event_DT_Attack2"));
			if (entityInfo->hasAttribute("event_DT_Attack3"))
				_animSet->addEventTime(Logic::ATTACK1, Logic::DAMAGE_TRACK, entityInfo->getFloatAttribute("event_DT_Attack3"));
			if (entityInfo->hasAttribute("event_DT_Attack4")) //CORREGIR QUE FALLARÁ
				_animSet->addEventTime(Logic::ATTACK1, Logic::DAMAGE_TRACK, entityInfo->getFloatAttribute("event_DT_Attack4"));
		}
		else
		{
			if (entityInfo->hasAttribute("event_DT_Attack1"))
				_animSet->addEventTime(Logic::ATTACK1, Logic::DAMAGE_TRACK, entityInfo->getFloatAttribute("event_DT_Attack1"));
			if (entityInfo->hasAttribute("event_DT_Attack2"))
				_animSet->addEventTime(Logic::ATTACK2, Logic::DAMAGE_TRACK, entityInfo->getFloatAttribute("event_DT_Attack2"));
			if (entityInfo->hasAttribute("event_DT_Attack3"))
				_animSet->addEventTime(Logic::ATTACK3, Logic::DAMAGE_TRACK, entityInfo->getFloatAttribute("event_DT_Attack3"));
		}	
	//
	/////////

		return true;
	} // initializeAnimSet
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::pair<unsigned short,float> track)
	{
		assert(_animSet && "LOGIC::ANIMATED_GRAPHICS>> No existe animSet");
		assert(_currentLogicAnimation!=NONE && "LOGIC::ANIMATED_GRAPHICS>> No tenemos animación Lógica activa.");

		// [ƒ®§] Ejemplo de gestión de eventos de animación -> En este caso se avisa de que animación ha finalizado (necesario en CDeath)
			CMessageUShort *txMsg = new CMessageUShort();
			txMsg->setType(Message::ANIMATION_FINISHED);
			txMsg->setUShort(_currentLogicAnimation); //PeP: envio que se ha finalizado la animación que se está reproduciendo.
			_entity->emitMessage(txMsg);

		if (_currentLogicAnimation != Logic::DEATH)
		{
			
			// Pep, si acaba una animación y tenemos una por defecto la ponemos, pero la animación por defecto debe ser lógica, hay que cambiarlo, pronto estará
			if (_currentLogicAnimation != Logic::ATTACK1 && _currentLogicAnimation != Logic::ATTACK2)			
			{
				_graphicalEntity->stopAnimation();
				_graphicalEntity->setAnimation(_defaultAnimation,0,true,NULL); //tenemos que cambiar defaultanimation por un enum Logico
			}
		}
		/*else
		{
			CMessageBool *txMsg = new CMessageBool();
			txMsg->setType(Message::d);
			txMsg->setUShort(_currentLogicAnimation); //PeP: envio que se ha finalizado la animación que se está reproduciendo.
			_entity->emitMessage(txMsg);

		}*/
		
	}
	//---------------------------------------------------------

	void CAnimatedGraphics::animationMomentReached(const std::pair<unsigned short,float> track)  //cambiar nombre de track por par u otra cosa
	{
		assert(_animSet && "LOGIC::ANIMATED_GRAPHICS>> No existe animSet");
		assert(_currentLogicAnimation!=NONE && "LOGIC::ANIMATED_GRAPHICS>> No tenemos animación Lógica activa.");

		if (_currentLogicAnimation == Logic::COVER_WITH_SHIELD || _currentLogicAnimation == Logic::COVER_WITH_WEAPON || _currentLogicAnimation == Logic::SWITCH_ALTAR)
		{
			_graphicalEntity->pauseAnimation(track.second);
		}
		else
		{
			CMessageUShort *msg = new CMessageUShort();
			msg->setUShort(_currentLogicAnimation);
			switch (static_cast<Logic::Tracks>(track.first))
			{
			case Logic::DAMAGE_TRACK:				
				msg->setType(Message::DAMAGE_MOMENT);	
			break;
			case Logic::COMBO_TRACK:			
				msg->setType(Message::COMBO_MOMENT);
			break;
			case Logic::COVER_MOMENT:			
				msg->setType(Message::UNASSIGNED);
			break;
			}
			_entity->emitMessage(msg);
		}
	}

	
	
} // namespace Logic

