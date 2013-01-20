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

		// Pablo. Si la entidad tiene el atributo jumpSpeed la capturamos y la guardamos en _jumpSpeed
		// En ppio solo la va a tener el player
		if(entityInfo->hasAttribute("initialJumpSpeed"))
			jumpSpeed = entityInfo->getFloatAttribute("initialJumpSpeed");

		// Pablo. Inicializo la gravedad
		gravity = 9.8; //expresada en metros /seg

		// velocidad inicial. es 28 m / seg

		//Pablo. Calculo la altura maxima del salto
		Hmax = (jumpSpeed*jumpSpeed) / (2*gravity); // 40 m es la altura maxima del salto ( en nuestro caso son pixeles)

		//Pablo. Tiempo hasta alcanzar la máxima altura
		// Vi - Vf / g (Velocidad inicial - Velocidad final / aceleracion). La Vf es 0 en altura max.
		Tmaxaltura = jumpSpeed / gravity; // 2,857142 segundos --> 2857,142 milisegundos
		Tmax = Tmaxaltura * 2; // 5,71428571 seg. --> 5714,28571 milisegundos.

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
			_jumping = false; // Pablo
		}
		else
		{
			_walkingLeft=true;
			_walkingRight=false;
			_jumping = false; // Pablo
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
				else if(!message._string.compare("jump")) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
					jump();
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
		} //fin del switch
	}//fin de CAngularMovement::process(const TMessage &message)

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

		// Pablo
		void CAngularMovement::jump()
		{
			_initialJump = true;


			/*if(_walkingLeft)
			  _entity->setSense(LogicalPosition::IZQUIERDA);
			if(_walkingRight)
			  _entity->setSense(LogicalPosition::DERECHA);*/

			// Pablo
			//_entity->getY(_entity->getBase(), _entity->getRing(),_jumping);
			//_entity->setPosition(_entity->getPosition());
			// Fin Pablo

			// Cambiamos la animación
			/*TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Jump";
			message._bool = true;
			_entity->emitMessage(message,this);
			*/

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
			Vector3 direction(Vector3::ZERO);
			direction = Math::getDirection(_entity->getYaw() + Math::PI/2);
			unsigned short base = _entity->getBase();
			unsigned short anillo = _entity->getRing();
			switch(anillo)
			{
			case 0:
				return; //no se puede bajar de anillo si estas en el inferior
			case 1:
				break;
			case 2:
				break;
			}
		}
		
		void CAngularMovement::goUp()
		{
			Vector3 direction(Vector3::ZERO);
			direction = Math::getDirection(_entity->getYaw() + Math::PI/2);
			unsigned short base = _entity->getBase();
			unsigned short anillo = _entity->getRing();
			switch(anillo)
			{
			case 0:
				break; 
			case 1:
				break;
			case 2:
				return; //no se puede subir de anillo si estas en el superior
			}
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
		/*if (_hit>0)
		{
			_hit++;
			if(_hit==100)
			{*/


				Logic::TMessage m;
				m._string="marine";
				m._type = Logic::Message::SET_SHADER;
				_entity->emitMessage(m,this);
				if (_entity->getType().compare("Player")==0)
				return;
					//_hit=0;
				
			

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

		// inicialY antes de saltar
		float inicialY;

		// Si nos estamos desplazando calculamos la próxima posición
		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.
	

		//if (!_entity->isPlayer())


	    Vector3 direction(Vector3::ZERO);

		if(_initialJump==true && _jumping==false)
		{
			_jumping = true;
			_timeJumping = 0;
			inicialY = _entity->getY(_entity->getBase(),_entity->getRing());
			
		}

		//Pablo
		if(_jumping)
		{
			//_timeJumping se incrementa en cada tick
			_timeJumping+= msecs;

			//direction = Math::getDirection(_entity->getYaw() + Math::PI/2);

			Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());

			float altura = jumpSpeed*_timeJumping -(gravity*(_timeJumping*_timeJumping)/2);

			newPosition.y = inicialY + altura;
			if(newPosition.y>=Hmax)
			{
				//_entity->setPosition(Hmax);
			}

			_entity->setPosition(newPosition);

			//direction.normalise();

			if(_timeJumping >= Tmax ) {
				_jumping = false;
			}

			
		}
		else if(_walkingLeft || _walkingRight)//		if(_walkingLeft || _walkingRight)
		{
			if(_walkingLeft || _walkingRight)
			{
				direction = Math::getDirection(_entity->getYaw() + Math::PI/2);
				//Matrix4 orientacion = _entity->getOrientation();
				//Math::yaw(Math::fromDegreesToRadians(_actualDegree),orientacion);
				//SI ANDA A LA DERECHA
				if(_walkingRight){
					if(_sentidoDerecha==true)
					{
						_sentidoDerecha=false;
						_entity->yaw(Math::PI);
						std::cout << "PosY: " << _entity->getPosition().y;
					}
						//_entity->setYaw(0);
						//_actualDegree+=-_angularSpeed;
					if (!_walkBack)
					{
						_entity->setDegree(_entity->getDegree()-_angularSpeed); 
						_entity->yaw(Math::fromDegreesToRadians(_angularSpeed));
					}
				}
				else //SI ANDA A LA IZQUIERDA
				{
					if(_sentidoDerecha==false)
					{
						_entity->yaw(Math::PI);					
						_sentidoDerecha=true;
						std::cout << "PosY: " << _entity->getPosition().y;
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
			
		} // fin del if(_walkingLeft || _walkingRight)

		// Pablo. Inicializo a false _initialJump para que solo lo tenga en cuenta una vez.
		_initialJump = false;
		
	} //fin del CAngularMovement::tick

	void CAngularMovement::turn(float amount)
	{
		_entity->yaw(amount);		
	}

} // namespace Logic