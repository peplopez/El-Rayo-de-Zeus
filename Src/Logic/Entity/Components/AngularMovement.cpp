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
 #include "Application/BaseApplication.h"

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
		_sentidoDerecha=_entity->getSense();
		
		if (_entity->getType().compare("Player")!=0)
		if (_sentidoDerecha)
		{
			_walkingLeft=false;
			_walkingRight=true;
		}
		else
		{
			_walkingLeft=true;
			_walkingRight=false;
		}
		 //_actualRadius=
		 //if (_entity->getType().compare("Player")==0)
		 	//		std::cout<<"PlayerDegree: "<<_entity->getDegree()<<std::endl;
			return true;
	}
		

	void CAngularMovement::deactivate(){}

	
	bool CAngularMovement::accept(const TMessage &message)
	{
		if (_entity->getType().compare("Player")==0)
			return message._type == Message::CONTROL;
		 if (_entity->getType().compare("AnimatedEntity")==0)
			return message._type == Message::NPC_CONTROL;
	}

		
	 void CAngularMovement::process(const TMessage &message)
		 {
		switch(message._type)
		{
		case Message::CONTROL:
			{
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
			else if(!message._string.compare("walkBack"))
				 {	
					_sentidoColision=message._bool;
					walkBack();
				 }
			else if(!message._string.compare("changeDirection"))
				 {	
					 _sentidoColision=message._bool;
					changeDirection(_sentidoColision);
				 }
			else if(!message._string.compare("turn"))
				turn(message._float);
			}
		case Message::NPC_CONTROL:
			{
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
			else if(!message._string.compare("walkBack"))
				 {	
					_sentidoColision=message._bool;
					walkBack();
				 }
			else if(!message._string.compare("changeDirection"))
				 {	
					 _sentidoColision=message._bool;
					changeDirection(_sentidoColision);
				 }
			else if(!message._string.compare("turn"))
				turn(message._float);
			}
		}
	}

		void CAngularMovement::walkRight()
		{
			_walkingRight = true;
			// Cambiamos la animación
			_entity->setSense(LogicalPosition::DERECHA);
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "RunKatana";
			message._bool = true;
			_entity->emitMessage(message,this);

		}
		
		void CAngularMovement::walkLeft()
		{
			_walkingLeft = true;
			
			// Cambiamos la animación
			_entity->setSense(LogicalPosition::IZQUIERDA);
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "RunKatana";
			message._bool = true;
			_entity->emitMessage(message,this);

		}
	
		void CAngularMovement::walkBack()
		{			
			_walkBack=true; //para retroceder en las colisiones   
			// Cambiamos la animación
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "IdleKatana";
			message._bool = true;
			_entity->emitMessage(message,this);
		}
		
		
		void CAngularMovement::goDown()
		{
			_changingRing=true;_entity->setRing(Ring::ANILLO_INFERIOR);
		}
		
		void CAngularMovement::goUp()
		{
			_changingRing=true;	
			_entity->setRing(Ring::ANILLO_SUPERIOR);
		}
		
		void CAngularMovement::stopMovement()
		{
			_walkingLeft = _walkingRight = false;

		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "IdleKatana";
			message._bool = true;
			_entity->emitMessage(message,this);
		}
		
		
	void CAngularMovement::changeDirection(bool newDirection)
	{		
				Logic::TMessage m;
				m._string="marine";
				m._type = Logic::Message::SET_SHADER;
				_entity->emitMessage(m,this);
				if (_entity->getType().compare("Player")==0)
				return;
	
			//	if (_entity->getType().compare("AnimatedEntity")==0)
				//{
				if (!_walkingLeft && !_walkingRight)
				{
					if (newDirection)
						walkLeft();
					else
						walkRight();
				}
					//	}
			}
	


		void CAngularMovement::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);

		// Si nos estamos desplazando calculamos la próxima posición
		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.
	
		bool cierre=false;
			unsigned int currentTime;
				unsigned int endingTime;
		/*while(_changingRing){
			currentTime=Application::CBaseApplication::getSingletonPtr()->getAppTime();
			
		if (!cierre)
		{
			cierre=true;
			endingTime=currentTime+3000;		
		}
		if (currentTime>endingTime)
			_changingRing=false;
		_entity->setRing(LogicalPosition::ANILLO_SUPERIOR);
			IComponent::tick(msecs);
		}*/

	     Vector3 direction(Vector3::ZERO);
		if(_walkingLeft || _walkingRight)
		{
			if(_walkingLeft || _walkingRight)
			{
				direction = Math::getDirection(_entity->getYaw() + Math::PI/2);
				//Matrix4 orientacion = _entity->getOrientation();
				//Math::yaw(Math::fromDegreesToRadians(_actualDegree),orientacion);
				if(_walkingRight){
					if(_sentidoDerecha==true)
					{
						_sentidoDerecha=false;
						_entity->yaw(Math::PI);						
					}
						//_entity->setYaw(0);
						//_actualDegree+=-_angularSpeed;
					if (!_walkBack)
					{
						_entity->setDegree(_entity->getDegree()-_angularSpeed); 
						_entity->yaw(Math::fromDegreesToRadians(_angularSpeed));
					}
				}
				else
				{
					if(_sentidoDerecha==false)
					{
						_entity->yaw(Math::PI);					
						_sentidoDerecha=true;
					}
					//_actualDegree+=_angularSpeed;
					if (!_walkBack)
					{
						_entity->setDegree(_entity->getDegree()+_angularSpeed);
						_entity->yaw(Math::fromDegreesToRadians(-_angularSpeed));
					}			
					
				}
				//turn(-0.02f*_angularSpeed);
				//turn(Math::PI/2);			
				//_entity->setOrientation(
				if (_walkBack)
					{
						stopMovement();   
						_walkBack=false;

						if (_sentidoColision) // la direccion
						{
							_entity->setDegree(_entity->getDegree()+(_angularSpeed+10)); 
							_entity->yaw(Math::fromDegreesToRadians(-(_angularSpeed)));
						}
						else
						{
							_entity->setDegree(_entity->getDegree()-(_angularSpeed+10)); 
							_entity->yaw(Math::fromDegreesToRadians((_angularSpeed)));
						}							
				
						
						/*Logic::TMessage m;
						m._type = Logic::Message::CONTROL;
						m._string = "walkStop";
						_entity->emitMessage(m);*/
						_sentidoColision=false;
						
					}
				if(_walkingLeft)
					direction *= -1;
			}

			direction.normalise();
			_entity->getPosition();
			//_entity->setDegree(_actualDegree);
			//_entity->setRadio(_actualRadius);

			
			// std::cout<<"PlayerDegree: "<<_entity->getDegree()<<std::endl;
			/*Vector3  newPosition=Vector3();
			if (_entity->getType().compare("Player")==0)
			newPosition= _entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getRadio(),_entity->getBase(),_entity->getRing());
			else	
			newPosition= _entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getRadio(),_entity->getBase(),_entity->getRing());
			*/
			Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
			
			//newPosition.y=_entity->getY();
			_entity->setPosition(newPosition);
			direction.normalise();
			
		}
		
		}

		void CAngularMovement::turn(float amount)
		{
				_entity->yaw(amount);		
		}

} // namespace Logic