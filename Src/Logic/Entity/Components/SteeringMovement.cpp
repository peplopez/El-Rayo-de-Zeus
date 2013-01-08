/**
@file SteeringMovement.cpp

Contiene la declaración de la clase CSteringMovement. Se encarga de
mover un avatar utilizando movimientos cinéticos o dinámicos.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "SteeringMovement.h"

#include "Map/MapEntity.h"

#include "AI/Movement.h"

namespace Logic 
{

	IMP_FACTORY(CSteeringMovement);

	//---------------------------------------------------------
	/**
	Destructor
	*/
	CSteeringMovement::~CSteeringMovement()
	{
		if (_currentMovement != 0)
			delete _currentMovement;
		if (_yaw != 0)
			delete _yaw;
	}

	//---------------------------------------------------------
	/**
	Inicialización del componente, utilizando la información extraída de
	la entidad leída del mapa (Maps::CEntity). Toma del mapa los atributos
	speed, angularSpeed, accel y angularAccel, que indican los valores máximos 
	de velocidad y aceleración lineales y angulares.

	@param entity Entidad a la que pertenece el componente.
	@param map Mapa Lógico en el que se registrará el objeto.
	@param entityInfo Información de construcción del objeto leído del
	fichero de disco.
	@return Cierto si la inicialización ha sido satisfactoria.
	*/
	bool CSteeringMovement::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("speed"))
			_maxLinearSpeed = entityInfo->getFloatAttribute("speed");
		if(entityInfo->hasAttribute("angularSpeed"))
			_maxAngularSpeed = entityInfo->getFloatAttribute("angularSpeed");
		if(entityInfo->hasAttribute("accel"))
			_maxLinearAccel = entityInfo->getFloatAttribute("accel");
		if(entityInfo->hasAttribute("angularAccel"))
			_maxAngularAccel = entityInfo->getFloatAttribute("angularAccel");

		_yaw = AI::IMovement::getMovement(AI::IMovement::MOVEMENT_KINEMATIC_ALIGN_TO_SPEED, 
			_maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel); 
		_yaw->setEntity(entity);

		return true;

	} // spawn

	//---------------------------------------------------------
	/**
	Método llamado en cada frame que actualiza el estado del componente.
	<p>
	Se encarga de mover la entidad en cada vuelta de ciclo cuando es necesario.

	@param msecs Milisegundos transcurridos desde el último tick.
	*/
	void CSteeringMovement::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		// TODO PRÁCTICA IA
		// En cada tick tenemos que:
		// Comprobar si hay que cambiar de movimiento o destino
		//		Si es así, sacar el nuevo movimiento de la factoría (puede ser NULL ==> parado)
		//		Asignarle el nuevo destino y la entidad
		//		Si tiene que cambiar la animación notificarlo con un mensaje

		// Si tenemos que movernos
		//		Invocar al método move del movimiento actual. Las velocidades actuales se guardan en _currentProperties
		//		Calcular la nueva posición y notificarla a la física para que nos mueva
		//		Calcular la nueva rotación y actualizarla en la propia entidad con el método setYaw
		//		Actualizar las velocidades usando la aceleración


		// Comprobamos si hay que cambiar de movimiento o destino
		if (_movType != _currentMovementType || _currentTarget != _target) {
			if (_currentMovement != 0) {
				delete _currentMovement;
			}
			_currentMovementType = _movType;
			_currentTarget = _target;
			_currentMovement = AI::IMovement::getMovement(_currentMovementType, _maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel);
			// Si hay un nuevo movimiento
			if (_currentMovement != 0) {
				// Ajustamos el target
				_arrived = false;
				_currentMovement->setEntity(_entity);
				_currentMovement->setTarget(_currentTarget);
				// Y la animación
				sendAnimationMessage("Walk");
			} else {
				// Si no hay movimiento paramos la animación
				sendAnimationMessage("Idle");
			}
		}


		// Si nos estamos desplazando calculamos la próxima posición
		if (!_arrived && _currentMovement != 0) {
			_arrived = _entity->getPosition().positionEquals(_target);
			_currentMovement->move(msecs, _currentProperties);
			// Aplicamos la rotación
			_yaw->move(msecs, _currentProperties);

			// Enviar un mensaje para que el componente físico mueva el personaje
			TMessage message;
			message._type = Message::AVATAR_WALK;
			message._vector3 = _currentProperties.linearSpeed * (float)msecs;
			_entity->emitMessage(message, this);
			// Aplicar la rotación
			//_entity->yaw(_entity->getYaw() - out.targetYaw);
			// En este caso suponemos que la entidad siempre se mueve hacia adelante, 
			// así que tomamos la dirección del vector de velocidad.
			//_entity->setYaw(atan2(-message._vector3.x, -message._vector3.z));
			_entity->setYaw(_entity->getYaw() + (float)_currentProperties.angularSpeed * msecs);

			// Acelerar
			_currentProperties.linearSpeed += _currentProperties.linearAccel * (float)msecs;
			// Clipping
			double speedValue = _currentProperties.linearSpeed.length();
			if (speedValue > _maxLinearSpeed) {
				_currentProperties.linearSpeed *= (_maxLinearSpeed / (float)speedValue);
			}

			_currentProperties.angularSpeed += _currentProperties.angularAccel * msecs;
			if (_currentProperties.angularSpeed > _maxAngularSpeed) 
				_currentProperties.angularSpeed = Ogre::Math::Sign((float)_currentProperties.angularSpeed) * _maxAngularSpeed;

		}

	} // tick

	//---------------------------------------------------------
	/**
	Envía un mensaje para cambiar la animación actual.

	@param animation Nueva animación.
	*/
	void CSteeringMovement::sendAnimationMessage(std::string animation) 
	{
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = animation;
		message._bool = true;
		_entity->emitMessage(message,this);
	}

	//---------------------------------------------------------
	/**
	Este componente sólo acepta mensajes de tipo MOVE_TO.
	*/
	bool CSteeringMovement::accept(const TMessage &message)
	{
		return message._type == Message::MOVE_TO;
	} // accept

	//---------------------------------------------------------
	/**
	Si se recibe un mensaje de tipo MOVE_TO, almacena el vector como 
	destino del movimiento y el tipo de movimiento.
	*/
	void CSteeringMovement::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::MOVE_TO:
			// Anotamos el vector de desplazamiento para usarlo posteriormente en 
			// el método tick. De esa forma, si recibimos varios mensajes AVATAR_MOVE
			// en el mismo ciclo sólo tendremos en cuenta el último.
			_target = message._vector3;
			_movType = message._int;
			break;
		}

	} // process

	//---------------------------------------------------------

}