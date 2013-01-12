	/**
@file LifeController.h

Contiene la declaración del componente que controla el movimiento 
angular de entidades.

@see Logic::CLifeController
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/

//#include "Logic/Entity/Components/"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"


//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CLifeController);

	bool CLifeController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		if(entityInfo->hasAttribute("angularSpeed"))
			_angularSpeed = entityInfo->getFloatAttribute("angularSpeed");

		return true;
		}

	bool CLifeController::activate()
	{
			return true;
	}
		

	void CLifeController::deactivate()
	{
	}

	
		 bool CLifeController::accept(const TMessage &message)
		 {
			return message._type == Message::CONTROL; //De momento, luego tendrá que aceptar de otras entidades NPC
		 }

		
		 void CLifeController::process(const TMessage &message)
		 {
		/*switch(message._type)
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
		}*/

		 }

		void CLifeController::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);

		// Si nos estamos desplazando calculamos la próxima posición
		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.
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
		void setLife(float life)
		{
			_life=life;
		}

		float getLife()
		{
			return _life;
		}
	

} // namespace Logic