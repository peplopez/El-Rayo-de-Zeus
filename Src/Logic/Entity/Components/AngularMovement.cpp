/**
@file AngularMovement.h

Contiene la declaración del componente que controla el movimiento 
angular de entidades.

@see Logic::CAngularMovement
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/

#include "AngularMovement.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"


//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CAngularMovement);

	bool CAngularMovement::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		if(entityInfo->hasAttribute("angularSpeed"))
			_angularSpeed = entityInfo->getFloatAttribute("angularSpeed");

		return true;
		}

	bool CAngularMovement::activate()
	{
		 _sentidoIzquierda=true;
			return true;
	}
		

	void CAngularMovement::deactivate(){}

	
		 bool CAngularMovement::accept(const TMessage &message)
		 {
			return message._type == Message::CONTROL; //De momento, luego tendrá que aceptar de otras entidades NPC
		 }

		
		 void CAngularMovement::process(const TMessage &message)
		 {
		switch(message._type)
		{
		case Message::CONTROL:
			if(!message._string.compare("goUp"))
				goUp();
			else if(!message._string.compare("goDown"))
				goDown();
			else if(!message._string.compare("walkLeft"))
				walkLeft();
			else if(!message._string.compare("walkRight"))
				walkRight();
			else if(!message._string.compare("walkStop"))
				stopMovement();
			else if(!message._string.compare("turn"))
				turn(message._float);
		}

		 }

		
		void CAngularMovement::walkRight()
		{
			_walkingRight = true;
			// Cambiamos la animación
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Run";
			message._bool = true;
			_entity->emitMessage(message,this);

		}
		
		void CAngularMovement::walkLeft()
		{
			_walkingLeft = true;
			// Cambiamos la animación
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Run";
			message._bool = true;
			_entity->emitMessage(message,this);

		}
		
		
		void CAngularMovement::goDown()
		{}
		
		void CAngularMovement::goUp()
		{}
		
		void CAngularMovement::stopMovement()
		{
		_walkingLeft = _walkingRight = false;

		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
		}
		

		void CAngularMovement::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);

		// Si nos estamos desplazando calculamos la próxima posición
		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.
	

			//if (!_entity->isPlayer())



		if(_walkingLeft || _walkingRight)
		{
			Vector3 direction(Vector3::ZERO);
			
			/*if(_walkingLeft || _walkingRight)
			{
				direction = Math::getDirection(_entity->getYaw());
				if(_walkingBack)
					direction *= -1;
			}*/

			if(_walkingLeft || _walkingRight)
			{
				direction = Math::getDirection(_entity->getYaw() + Math::PI/2);
				//Matrix4 orientacion = _entity->getOrientation();
				//Math::yaw(Math::fromDegreesToRadians(_actualDegree),orientacion);
				if(_walkingRight){
					if(_sentidoIzquierda==true){
						_sentidoIzquierda=false;
						_entity->yaw(Math::PI);
						
					}
						//_entity->setYaw(0);
						_actualDegree+=-_angularSpeed;
						_entity->yaw(Math::fromDegreesToRadians(_angularSpeed));
				}
				else
				{
					if(_sentidoIzquierda==false){
						_entity->yaw(Math::PI);					
						_sentidoIzquierda=true;
					}
					_actualDegree+=_angularSpeed;
					_entity->yaw(Math::fromDegreesToRadians(-_angularSpeed));
				}
				//turn(-0.02f*_angularSpeed);
				//turn(Math::PI/2);
				
				
				//_entity->setOrientation(
				if(_walkingLeft)
					direction *= -1;
			}

			//direction += directionStrafe;
			direction.normalise();
			//direction *= msecs * _angularSpeed;
			//float desplazamiento

			Vector3 newPosition = Math::fromPolarToCartesian(_actualDegree,_actualRadius);
				//_entity->getPosition() + direction;
			_entity->setPosition(newPosition);
		}
		}

		void CAngularMovement::turn(float amount)
		{
				_entity->yaw(amount);		
		}

} // namespace Logic