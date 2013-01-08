/**
@file SwitchPosition.cpp

Contiene la implementación del componente que controla el movimiento de
una entidad que tiene dos estado (posiciones) para ir de una a otra.
 
@see Logic::CSwitchPosition
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "SwitchPosition.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CSwitchPosition);
	
	//---------------------------------------------------------

	CSwitchPosition::CSwitchPosition() : IComponent(), _targetPosition(Vector3::ZERO), 
										_speed(0.05f), _loop(false) 
	{
		_position[0] = Vector3::ZERO;
		_position[1] = Vector3::ZERO;
	}

	//---------------------------------------------------------

	bool CSwitchPosition::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("speed"))
			_speed = entityInfo->getFloatAttribute("speed");

		if(entityInfo->hasAttribute("loop"))
			_loop = entityInfo->getBoolAttribute("loop");

		if(entityInfo->hasAttribute("position")) {
			_position[0] = entityInfo->getVector3Attribute("position");
			_targetPosition = _position[0];
		}

		if(entityInfo->hasAttribute("position2")) 
			_position[1] = entityInfo->getVector3Attribute("position2");

		return true;

	} // spawn

	
	//---------------------------------------------------------

	bool CSwitchPosition::accept(const TMessage &message)
	{
		return message._type == Message::SWITCH;

	} // accept
	
	//---------------------------------------------------------

	void CSwitchPosition::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::SWITCH:
			assert((message._int >= 0) && (message._int <= 1));
			_targetPosition = _position[message._int];
			break;
		}

	} // process
	
	//---------------------------------------------------------

	void CSwitchPosition::tick(unsigned int msecs)
	{
		// Invocamos al método de la clase padre
		IComponent::tick(msecs);

		// Si la posición actual aun no es la objetivo nos desplazamos hacia ella.
		float distance = _entity->getPosition().distance(_targetPosition);
		if(distance > 0.0001)
		{
			// Calcular vector de desplazamiento
			Vector3 direction = _targetPosition - _entity->getPosition();
			direction.normalise();
			direction *= msecs * _speed;

			// Controlamos que no nos pasemos del destino
			if (direction.length() > distance) {
				direction /= direction.length() / distance;
			}

			// Movemos la entidad a la nueva posición
			_entity->setPosition(_entity->getPosition() + direction);
		}
		// Si estamos en loop y hemos llegado a una posición transitamos a la otra.
		else if(_loop)
		{
			if(_targetPosition == _position[0])
				_targetPosition = _position[1];
			else
				_targetPosition = _position[0];
		}

	} // tick

} // namespace Logic

