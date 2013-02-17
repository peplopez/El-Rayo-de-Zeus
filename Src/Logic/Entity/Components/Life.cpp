/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Life.h"

#include "Logic/Entity/Entity.h"

#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

// Para informar por red que se ha acabado el juego
#include "Net/Manager.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"


namespace Logic 
{
	IMP_FACTORY(CLife);
	
	//---------------------------------------------------------

	bool CLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("life"))
			_life = entityInfo->getFloatAttribute("life");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLife::accept(const CMessage *message)
	{
		return message->getType() == Message::DAMAGED || 
				message->getType() == Message::CONTACT || // HACK provisional
				message->getType() == Message::ANIMATION_FINISHED;

	} // accept
	
	//---------------------------------------------------------

	void CLife::process(CMessage *message)
	{
		switch(message->getType())
		{
			
			case Message::DAMAGED:
			case Message::CONTACT:
			{
				CMessageFloat *maux = static_cast<CMessageFloat*>(message);
				// Disminuir la vida de la entidad
				_life -= maux->getFloat();
				
				CMessageBoolString *msg = new CMessageBoolString();
					msg->setType(TMessageType::SET_ANIMATION);	
					msg->setBool(false);
				if(_life > 0)  // TODO Poner la animación de herido.
					msg->setString("Damage");
				else  // TODO Si la vida es menor que 0 poner animación de morir.
					msg->setString("Death");
				_entity->emitMessage(msg, this);
			
			} break;
		
			// ANIMACION FINALIZADA
			case Message::ANIMATION_FINISHED: 
			{	
				CMessageString *maux = static_cast<CMessageString*>(message);
				// TODO Si matan a un jugador habrá que avisarle que, para él, el 
					// juego ha terminado. Si hubiese más jugadores también deberían
					// enterarse de que ese jugador ha muerto para que eliminen su entidad...
				if(maux->getString() == "Death") { // Completada animación de muerte -> END_GAME
					CMessage *msg = new CMessage();
						msg->setType(TMessageType::DEAD);
					_entity->emitMessage(msg, this);
				}

			}break;

		}




	} // process


} // namespace Logic

