/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CDeath
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Death.h"

#include "Application/BaseApplication.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
// FRS Prohibido depender de Application en Logic (siempre que se pueda); además el gameover lo gestiona ya la FSM 

#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Entity/Messages/MessageAudio.h"


namespace Logic 
{
	IMP_FACTORY(CDeath);
	
	//---------------------------------------------------------

	bool CDeath::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_audio = "media\\audio\\fallecimiento.wav"; // FRS Usar map.txt es de débiles!!! xD
		return true;
	} // spawn
	
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

/* UNDONE Muerte gestionada por FSM
		if(_entity->getType() == "Player" || _entity->getType() == "OtherPlayer")
			deathAnimated(message);
		else
*/
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
		case Message::DEAD: {
			CMessageBoolUShort *txMsg = new CMessageBoolUShort(); // Poner la animación de muerte
				txMsg->setType(TMessageType::SET_ANIMATION);
				txMsg->setUShort( Logic::DEATH );	
				txMsg->setBool(false);
				_entity->emitMessage(txMsg);
				/* Aquí ponemos el sonido */
			Logic::CMessageAudio *maudio=new Logic::CMessageAudio();		
			maudio->setType(Message::AUDIO);			
			maudio->setPath(_audio);
			maudio->setId("muerte");
			maudio->setPosition(_entity->getPosition());
			maudio->setNotIfPlay(false);
			maudio->setIsPlayer(false);
			_entity->emitMessage(maudio);
		} break;	
		
		// ANIMACION FINALIZADA		
		case Message::ANIMATION_FINISHED: {
			
			CMessageUShort *rxMsg = static_cast<CMessageUShort*>(message);
				if(rxMsg->getUShort() == Logic::DEATH ) { // Completada animación de muerte? -> END_GAME					
				/* UNDONE FRS Esto ahora lo gestiona la FSM
					if(_entity->isPlayer() ) // PLAYER MUERTO -> GameOver
						Application::CBaseApplication::getSingletonPtr()->setState("gameOver"); // HACK Player muerto -> respawn es distinto de base muerta
				
					else // Resto de entidades
				*/		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				
				}
		 }break;
		
		} // switch

	} // death


} // namespace Logic

