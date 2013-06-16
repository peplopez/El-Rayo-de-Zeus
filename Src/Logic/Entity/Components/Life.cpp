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
		//Si la entidad no es el PLAYER le creamos el Billboard

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

				//if (message->getAction()==TActionType::DAMAGE)
				//	modifyLife(-10);
				//else
				//	modifyLife(10);

				//PT Me da igual que la action sea DAMAGE o HEAL, lo importante es que sea LIFE_MODIFIER el tipo
				// ya que luego en el entero del mensaje me vendrá el numero a modificar en negativo o positivo
				//En Msg->getUint viene el modificador entero.

				//if (message->getAction()==TActionType::DAMAGE)
				//	_lifemodificator = -Msg->getUInt();
				//else
				//	_lifemodificator = Msg->getUInt();

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

		_life += lifeModifier;

		Math::Clamp( _life, 0, _LIFE_MAX); // Disminuir/ aumentar la vida de la entidad
			
		// DIES
		if(_life <= 0) {

			CMessage *msg = new CMessage();
			msg->setType(Logic::Message::DEAD);
			msg->setAction(Logic::Message::DAMAGE); // HACK PeP para que funcione máquina estados
			_entity->emitMessage(msg, this);

			// PT Cuando la entidad pierde toda su vida, se elimina la entidad 
			// (tanto grafica como fisicamente)
			if(!_entity->isPlayer() && _entity->getType()!="OtherPlayer")
			{
				if(_lifeBarBB!=NULL)
				{
					_graphicalScene->remove(_lifeBarBB);
					_lifeBarBB = NULL;
				}

				// PT. Estoy intentando eliminar la entidad cuando no es
				// de tipo Player. 
				// Lo comento porque peta en OgreRoot.cpp -> 
				// bool Root::renderOneFrame(Real timeSinceLastFrame) 
				// -> evt.timeSinceLastEvent = calculateEventTime(now, FETT_ANY);
				//CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				//_lifeBarBB = NULL;

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
			float ratio = float (_life) / float (_LIFE_MAX);


			//Si la entidad no es el player actualizamos su billboard
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
			else // if entity is Player then we update its HUD progressbar, sending a message
			{
				CMessageFloat *message = new CMessageFloat();
				message->setType(Logic::Message::HUD);
				message->setAction(Logic::Message::UPDATE_HUD_LIFE);
				message->setFloat(ratio);
				_entity->emitMessage(message);

			}


	} // modifyLife


} // namespace Logic

