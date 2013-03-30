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
		CMessageBoolString *maux = static_cast<CMessageBoolString*>(message);
		CMessageString *maux2 = static_cast<CMessageString*>(message);
		switch(message->getType())
		{
			case Message::SET_ANIMATION:
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

				_graphicalEntity->setAnimation(maux->getString(),0,maux->getBool());

				LOG("SET_ANIMATION: " << maux->getString());
				break;

			case Message::STOP_ANIMATION:
				_graphicalEntity->stopAnimation(maux->getString());
			//	_graphicalEntity->pauseAnimation(maux->getString(),_graphical);

				LOG("STOP_ANIMATION: " << maux2->getString());
				break;
			case Message::REWIND_ANIMATION:
				_graphicalEntity->rewind(maux->getString(),maux->getBool());
				break;
		}

	} // process

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

