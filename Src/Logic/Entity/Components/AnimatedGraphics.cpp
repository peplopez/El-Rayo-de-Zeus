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

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
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

	bool CAnimatedGraphics::accept(const CMessage *message)
	{
		return CGraphics::accept(message) ||
			   message->getType() == Message::SET_ANIMATION ||
			   message->getType() == Message::STOP_ANIMATION ||
			   message->getType() == Message::REWIND_ANIMATION;

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
				/*if (maux->getString().compare("FireAK47")==0)
					_graphicalEntity->setAnimation(maux->getString(),0,maux->getBool());
				else	*/	
				/*if (message->getAction()==Message::UNDEF)
					_graphicalEntity->setAnimation(maux->getString(),0.8,maux->getBool());
				else*/

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
		}

	} // process

	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::reCreateGraphicalEntity()
	{
		assert(_scene && "LOGIC::ANIMATED_GRAPHICS>> No existe escena gráfica!");
		assert( _model.length() > 0  &&  "LOGIC::ANIMATED_GRAPHICS>> No existe modelo!");	

		delete _graphicalEntity;
		_graphicalEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model);
			if(!_scene->add(_graphicalEntity) )
				return 0;
		
		CGraphics::_graphicalEntity = _graphicalEntity;

		_graphicalEntity->setAnimation(_defaultAnimation,0,true);
		_graphicalEntity->setObserver(this);
		

		return _graphicalEntity;
	} // createGraphicsEntity

	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicalEntity(const Map::CEntity *entityInfo)
	{
		assert(_scene && "LOGIC::ANIMATED_GRAPHICS>> No existe escena gráfica!");
		assert( _model.length() > 0  &&  "LOGIC::ANIMATED_GRAPHICS>> No existe modelo!");	

		_graphicalEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model);
			if(!_scene->add(_graphicalEntity) )
				return 0;

		// DEFAULT ANIMATION
		if(entityInfo->hasAttribute("defaultAnimation")) { 
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_graphicalEntity->setAnimation(_defaultAnimation,0,true);
			_graphicalEntity->setObserver(this);
		}

		return _graphicalEntity;
	} // createGraphicsEntity

	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		if (animation!="Death")
		{
			// [ƒ®§] Ejemplo de gestión de eventos de animación -> En este caso se avisa de que animación ha finalizado (necesario en CDeath)
			CMessageString *txMsg = new CMessageString();
				txMsg->setType(Message::ANIMATION_FINISHED);
				txMsg->setString(animation);
				_entity->emitMessage(txMsg);

			// Si acaba una animación y tenemos una por defecto la ponemos
			_graphicalEntity->stopAllAnimations();
			_graphicalEntity->setAnimation(_defaultAnimation,0,true);
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

