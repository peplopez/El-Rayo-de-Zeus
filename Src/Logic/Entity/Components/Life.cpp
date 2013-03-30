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
#include "Logic/Entity/Messages/MessageInt.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Messages/MessageAudio.h"
#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CLife);

	//---------------------------------------------------------
	
	CLife::~CLife() {
		if(_lifeBarBB)
		{
			_graphicalScene->remove(_lifeBarBB);	
			delete _lifeBarBB;
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
		_lifeBarBB = new Graphics::CBillboard( entity->getName(), 
			Vector3(0, lifeBarPosition, 0), lifeBarWidth, lifeBarHeight, "lifeBar");  //le paso un string con el nombre de la entidad			
			_graphicalScene->add(_lifeBarBB);
			_lifeBarBB->setTextureCoords(0.0f, 0.0f, 0.5f, 1.0f);

			//_audio = "media\\audio\\QUEJIDO1.wav";
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLife::accept(const CMessage *message)
	{
		return message->getType() == Message::LIFE_MODIFIER;		

	} // accept
	
	//---------------------------------------------------------

	void CLife::process(CMessage *message)
	{
		switch(message->getType())
		{
			case Message::LIFE_MODIFIER:
			{
				modifyLife( static_cast<CMessageInt*>(message)->getInt() );
				break;
			}
		}
	} // process


	//---------------------------------------------------------

	void CLife::modifyLife(int lifeModifier) {


		Math::delimit( _life += lifeModifier, 0, _LIFE_MAX); // Disminuir/ aumentar la vida de la entidad
			
		// DIES
		if(_life <= 0) {

			CMessage *msg = new CMessage();
			msg->setType(Logic::Message::DEAD);
				_entity->emitMessage(msg, this);
		
		// DAMAGE / HEAL 
		} else { // Solo animaciones

			CMessageBoolString *msg = new CMessageBoolString();
				msg->setType(Logic::Message::SET_ANIMATION);	
				msg->setBool(false);
			/* Aquí ponemos el sonido */
			Logic::CMessageAudio *maudio=new Logic::CMessageAudio();		
			maudio->setType(Message::AUDIO);			
			maudio->setPath(_audio);
			maudio->setId("impacto");
			maudio->setPosition(_entity->getPosition());
			_entity->emitMessage(maudio);
			if(lifeModifier < 0)
				msg->setString("Damage"); //Poner la animación de herido.
			else if(lifeModifier > 0)
				msg->setString("FireUzi"); // Poner la animación de curacion

			//_entity->emitMessage(msg, this); //cancelo el envio, estoy haciendolo en la maquina de estados
		}		

		// LIFEBAR CONTROL
		float ratio = (float)_life / (float)_LIFE_MAX; // FRS Menudo fail hicimos aquí con los enteros xD
			_lifeBarBB->setTextureCoords(
				(1.0f - ratio) / 2.0f,			// u1
				0.0f,							// v1
				0.5f + (1.0f - ratio) / 2.0f,	// u2
				1.0f							// v2
			);


	} // modifyLife


} // namespace Logic

