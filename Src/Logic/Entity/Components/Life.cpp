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
		_graphicalScene->remove(_lifeBarBB);
		_graphicalScene = NULL;
	}

	//---------------------------------------------------------

	void CLife::attachToMap(CMap* map)
	{
		_graphicalScene = map->getGraphicScene();
		_graphicalScene->add(_lifeBarBB);
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
		std::stringstream ssAux; // FRS Importante añadir ID para evitar entidades gráficas con = nombre
			ssAux << _entity->getName() << _entity->getEntityID();
			std::string	parentName = ssAux.str();

		_lifeBarBB = new Graphics::CBillboard( 
			parentName,// nombre de la entidad gráfica de ref
			Vector3(0, lifeBarPosition, 0), 
			lifeBarWidth, lifeBarHeight, "lifeBar"
		);  		
			_graphicalScene->add(_lifeBarBB);
			_lifeBarBB->setTextureCoords(0.0f, 0.0f, 0.5f, 1.0f);

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

				if (message->getAction()==TActionType::DAMAGE)
					modifyLife(-10);
				else
					modifyLife(10);
				break;
			}

			//PT
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

			////PT Cuando la entidad pierde toda su vida, se elimina su billboard (barra de vida) de la escena
			if(!_entity->isPlayer())
			{
				//if(_lifeBarBB!=NULL)
				//{
				//	_graphicalScene->remove(_lifeBarBB);
				//	_lifeBarBB = NULL;
				//}

				//PT desatacho/elimino la entidad LOGICAMENTE
				//_entity->detachFromMap();

				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				_lifeBarBB = NULL;

				//_lifeBarBB = 0;
				//delete _lifeBarBB;

				//CMessage *msg2 = new CMessage();
				//msg2->setType(Logic::Message::DELETE_GRAPHICAL_ENTITY);
				//_entity->emitMessage(msg2, this);

				//PT desatacho/elimino la entidad GRAFICAMENTE

				//PT TODO desatachar/eliminar la entidad FISICAMENTE


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
			float ratio = float (_life) / float (_LIFE_MAX); // FRS Menudo fail hicimos aquí con los enteros xD

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


	} // modifyLife


} // namespace Logic

