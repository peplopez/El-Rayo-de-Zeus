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
#include "Graphics/Server.h" //Pablo 04-02-2013

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageInt.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Maps/Map.h"

#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CLife);
	
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

		// crear el graphics::cbillboard y añadirle las dimensiones y ponerle las coordenadas
		_lifeBarBB = Graphics::CBillboard( entity->getName(), lifeBarPosition, lifeBarWidth, lifeBarHeight);  //le paso un string con el nombre de la entidad			
			
			_lifeBarBB.setCoordenadas(0.0f, 0.0f, 0.5f, 1.0f);
		
		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CLife::deactivate()
	{
		_lifeBarBB.deactivateBillboard( _entity->getName() ); //Eliminacion del billboard de la entidad	
		IComponent::deactivate();
	} // deactivate

	
	//---------------------------------------------------------

	bool CLife::accept(const CMessage *message)
	{
		return message->getType() == Message::LIFE_MODIFIER;		

	} // accept
	
	//---------------------------------------------------------

	void CLife::process(CMessage *message)
	{
		modifyLife( static_cast<CMessageInt*>(message)->getInt() );
	} // process


	//---------------------------------------------------------

	void CLife::modifyLife(int lifeModifier) {

		Math::delimit( _life += lifeModifier, 0, _LIFE_MAX); // Disminuir/ aumentar la vida de la entidad
			
		// DIES
		if(_life <= 0) {

			CMessage *msg = new CMessage();
				msg->setType(TMessageType::DEAD);
				_entity->emitMessage(msg, this);
		
		// DAMAGE / HEAL 
		} else { // Solo animaciones

			CMessageBoolString *msg = new CMessageBoolString();
				msg->setType(TMessageType::SET_ANIMATION);	
				msg->setBool(false);

			if(lifeModifier < 0)
				msg->setString("Damage"); //Poner la animación de herido.
			else if(lifeModifier > 0)
				msg->setString("Heal"); // Poner la animación de curacion

			_entity->emitMessage(msg, this);
		}		

		// LIFEBAR CONTROL
		float ratio = _life / _LIFE_MAX;
			_lifeBarBB.setCoordenadas(
				(1.0f - ratio) / 2.0f,			// u1
				0.0f,							// v1
				0.5f + (1.0f - ratio) / 2.0f,	// u2
				1.0f							// v2
			);

	
	} // modifyLife


} // namespace Logic

