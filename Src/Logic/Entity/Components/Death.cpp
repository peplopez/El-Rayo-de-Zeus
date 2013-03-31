/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CDeath
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Death.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Application/BaseApplication.h"

#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolString.h"


namespace Logic 
{
	IMP_FACTORY(CDeath);
	
	//---------------------------------------------------------

	bool CDeath::accept(const CMessage *message)
	{
		return	message->getType() == Message::DEAD ||
				message->getType() == Message::ANIMATION_FINISHED;

	} // accept
	
	//---------------------------------------------------------

	// TODO separar en funciones / tipo
	void CDeath::process(CMessage *message)
	{
		// HACK FRS habrá que tener en cuenta todos los que tengan muerte animada...
		// (por parámetro en map.txt? animatedDeath = true o CDeathAnimated?)
		if(_entity->getType() == "Player" || _entity->getType() == "OtherPlayer")
			deathAnimated(message);
		else
			death(message);
	
	} // process


	//---------------------------------------------------------


	void CDeath::death (CMessage *message) {
		if(message->getType() == TMessageType::DEAD)		
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
	} // death

	//---------------------------------------------------------

	void CDeath::deathAnimated(CMessage *message) {

		switch(message->getType())
		{

		// MUERTO
		case Message::DEAD:		{	

			_entity->getComponent<CAvatarController>()->sleep(); // Desactivamos control de teclado

			CMessageBoolString *txMsg = new CMessageBoolString(); // Poner la animación de muerte
				txMsg->setType(TMessageType::SET_ANIMATION);
				txMsg->setString("Death");	
				txMsg->setBool(false);
				_entity->emitMessage(txMsg);

		} break;	
		
		// ANIMACION FINALIZADA		
		case Message::ANIMATION_FINISHED: {
			
			CMessageString *rxMsg = static_cast<CMessageString*>(message);
				if(rxMsg->getString() == "Death") { // Completada animación de muerte? -> END_GAME					
					if(_entity->isPlayer() ) // PLAYER MUERTO -> GameOver
						Application::CBaseApplication::getSingletonPtr()->setState("gameOver"); // HACK Player muerto -> respawn es distinto de base muerta
					else // Resto de entidades
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				}
		 }break;
		
		} // switch

	} // death


} // namespace Logic

