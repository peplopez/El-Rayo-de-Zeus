/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Life.h"

#include "BaseSubsystems/Math.h"

#include "Graphics/Billboard.h"
#include "Graphics/Scene.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Messages/MessageAudio.h"
#include "Map/MapEntity.h"

//PT
#include "Logic/Maps/EntityFactory.h"


namespace Logic 
{
	IMP_FACTORY(CLife);

	//---------------------------------------------------------
	
	CLife::~CLife() {
		if(_lifeBarBB)
		{
			_graphicalScene->remove(_lifeBarBB);
			_lifeBarBB = 0; //PT
			delete _lifeBarBB;
		}
	}

	//---------------------------------------------------------


	void CLife::detachFromMap()
	{
		if(!_entity->isPlayer())
		{
			_graphicalScene->remove(_lifeBarBB);
			_graphicalScene = NULL;
		}
	}

	//---------------------------------------------------------

	void CLife::attachToMap(CMap* map)
	{
		if(!_entity->isPlayer())
		{
			_graphicalScene = map->getGraphicScene();
			_graphicalScene->add(_lifeBarBB);
		}
	}

	//---------------------------------------------------------

	bool CLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicalScene = map->getGraphicScene();

		assert( entityInfo->hasAttribute("lifeMax") );
			_life = _LIFE_MAX = entityInfo->getIntAttribute("lifeMax");

		assert( entityInfo->hasAttribute("lifeBarPosition") );		
			float lifeBarPosition = entityInfo->getFloatAttribute("lifeBarPosition");

		assert( entityInfo->hasAttribute("lifeBarWidth") );	
			float lifeBarWidth = entityInfo->getFloatAttribute("lifeBarWidth");	

		assert( entityInfo->hasAttribute("lifeBarHeight") );
			float lifeBarHeight = entityInfo->getFloatAttribute("lifeBarHeight");

		if (entityInfo->hasAttribute("audio") )
			_audio = entityInfo->getStringAttribute("audio");

		// crear el graphics::cbillboard y añadirle las dimensiones y ponerle las coordenadas
		//Si la entidad no es el PLAYER le creamos el Billboard (para el player su vida se visualiza
		//en el hud)

		if(!_entity->isPlayer())
		{
			// crear el graphics::cbillboard y añadirle las dimensiones y ponerle las coordenadas

			_lifeBarBB = new Graphics::CBillboard( 
				_entity->getGraphicalName(),// nombre de la entidad gráfica de ref
				Vector3(0, lifeBarPosition, 0), 
				lifeBarWidth, lifeBarHeight, "lifeBar"
			);  		
				_graphicalScene->add(_lifeBarBB);
				_lifeBarBB->setTextureCoords(0.0f, 0.0f, 0.5f, 1.0f);
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLife::accept(const CMessage *message)
	{
		return message->getType() == Message::LIFE_MODIFIER || message->getType() == Message::LIFE_RESTORE ;

	} // accept	
	
	//---------------------------------------------------------

	void CLife::process(CMessage *message)
	{
		//modifyLife(static_cast<CMessageUInt*>(message)->getUInt() *(message->getAction() == TActionType::DAMAGE ? -10 : 10)  	);
		switch(message->getType())
		{
			case Message::LIFE_MODIFIER:
			{
				CMessageUInt *Msg = static_cast<CMessageUInt*>(message);

				// PT. It doesnt care if action is DAMAGE or HEAL, important thing is that message type is LIFE_MODIFIER
				// because the integer that comes inside the message will be positive or negative
				// In Msg->getUint comes the integer modificator

				_lifemodificator = Msg->getUInt();
				modifyLife(_lifemodificator); 

				break;
			}

			//PT. This case happens when a player is respawned into its origin base
			case Message::LIFE_RESTORE:
			{
				modifyLife(_LIFE_MAX);
				break;
			}
		}
	
	} // process


	//---------------------------------------------------------

	void CLife::modifyLife(int lifeModifier) {

		if(_entity->isPlayer()){
			std::cout<<"----CLife::modifyLife ---- "<<  std::endl;
			std::cout<<" _life = "<< _life << std::endl;
			std::cout<<" lifeModifier = "<< lifeModifier << std::endl;
		}

		_life += lifeModifier;

		Math::Clamp( _life, 0, _LIFE_MAX); // Disminuir/ aumentar la vida de la entidad

		if(_entity->isPlayer()){
			std::cout<<" _life after modifying =  "<< _life << std::endl;
			std::cout<<"----End CLife::modifyLife ---- "<<  std::endl;
		}

		// DIES
		if(_life <= 0) {

			if(_entity->isPlayer()){
				std::cout<<" BUCLE IF: _life <= 0 y se manda mensaje de TIPO DEAD y ACTION DAMAGE"<<  std::endl;
			}

			CMessage *msg = new CMessage();
			msg->setType(Logic::Message::DEAD);
			msg->setAction(Logic::Message::DAMAGE); // HACK PeP para que funcione máquina estados
			_entity->emitMessage(msg, this);

			// PT. When NO PLAYER entity lose all its life
			// entity is deleted graphically as phisically
			if(!_entity->isPlayer() && _entity->getType()!="OtherPlayer")
			{
				if(_lifeBarBB!=NULL)
				{
					_graphicalScene->remove(_lifeBarBB);
					_lifeBarBB = NULL;
				}
			}
		// DAMAGE / HEAL 
		} else if(lifeModifier) { // Solo animaciones
		
			/* Aquí ponemos el sonido */
			Logic::CMessageAudio *maudio=new Logic::CMessageAudio();		
			maudio->setType(Message::AUDIO);
			maudio->setId("impacto");
			maudio->setPosition(_entity->getPosition());
			maudio->setNotIfPlay(false);
			maudio->setIsPlayer(false);
			// TODO
			lifeModifier < 0 ?	maudio->setPath(_audio): //Poner el sonido de herido. 
								maudio->setPath(_audio); // Poner el sonido de curacion

			_entity->emitMessage(maudio);
		}		

			// LIFEBAR CONTROL
			float flife = float (_life);
			float flifeMax = float (_LIFE_MAX);
			//float ratio = float (_life) / float (_LIFE_MAX);
			float ratio = flife / flifeMax;

			if(_entity->isPlayer()){
				std::cout<<" ----------- RATIO --------------- " << std::endl;
				std::cout<<" _life =  "<< _life << " flife = " << flife << std::endl;
				std::cout<<" _LIFE_MAX =  "<< _LIFE_MAX << " flifeMax = " << flifeMax << std::endl;
				std::cout<<" ratio =  "<< ratio <<  std::endl;
				std::cout<<" ----------- END RATIO --------------- " << std::endl;
			}

			//If NOT Player, its billboard is updated 
			if(!_entity->isPlayer())
			{
				//Si la entidad tiene su _lifeBar distinto de NULL actualizamos las coordenadas UV, sino NO.
				if(_lifeBarBB!=NULL)
				{
					_lifeBarBB->setTextureCoords(
						(1.0f - ratio) / 2.0f,			// u1
						0.0f,							// v1
						0.5f + (1.0f - ratio) / 2.0f,	// u2
						1.0f							// v2
					);
				}
			}
			else // if IS Player then we update its HUD progressbar, sending a message
			{
				std::cout<<" UPDATE HUD PLAYER WITH RATIO =  " << ratio << std::endl;
				CMessageFloat *message = new CMessageFloat();
				message->setType(Logic::Message::HUD);
				message->setAction(Logic::Message::UPDATE_HUD_LIFE);
				message->setFloat(ratio);
				_entity->emitMessage(message);

			}

	} // modifyLife


} // namespace Logic

