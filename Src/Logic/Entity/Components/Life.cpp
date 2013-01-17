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

	bool CLife::accept(const TMessage &message)
	{
		return message._type == Message::DAMAGED;

	} // accept
	
	//---------------------------------------------------------

	void CLife::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::DAMAGED:
			{
				// Disminuir la vida de la entidad
				_life -= message._float;

				// Si han matado al jugador sacarlo de la partida
				if (_life <= 0 && _entity->getType() == "Player") 
				{
					// TODO Si matan a un jugador habrá que avisarle que, para él, el 
					// juego ha terminado. Si hubiese más jugadores también deberían
					// enterarse de que ese jugador ha muerto para que eliminen su entidad...
					// TODO Pero queremos eliminar la entidad o dejarla deactive?
					// TODO Quizá aquí merezca la pena tener un CDeath que se encargue de estas cosas con un Msg DIE entidad tal
					
					// MENSAJE "END_GAME | TEntityID"
					Net::NetMessageType txMsg = Net::NetMessageType::END_GAME;
					TEntityID entityID = _entity->getEntityID();
					Net::CBuffer serialMsg;						
						serialMsg.write( &txMsg, sizeof(txMsg));
						serialMsg.write( &entityID, sizeof(entityID) );	
					Net::CManager::getSingletonPtr()->send( serialMsg.getbuffer(),  serialMsg.getSize() );

					// TRATAMIENTO DEL PLAYER
					if(_entity->isPlayer() ) // MONOJUGADOR -> GameOver
						Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
					else					// MULTIJUGADOR
						_entity->deactivate();
				}
				
				TMessage msg;
					msg._type = TMessageType::SET_ANIMATION;						
				if(_life > 0)  // TODO Poner la animación de herido.
					msg._string = "Damage";
				else  // TODO Si la vida es menor que 0 poner animación de morir.
					msg._string = "Death";
				_entity->emitMessage(msg, this);
			}
			break;
		}

	} // process


} // namespace Logic

