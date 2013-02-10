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
		
		//_angularSpeed=0.00625;


		// Pablo. Si la entidad tiene el atributo _jumpSpeed la capturamos y la guardamos en __jumpSpeed
		// En ppio solo la va a tener el player
		if(entityInfo->hasAttribute("initialJumpSpeed"))
			_jumpSpeed = entityInfo->getFloatAttribute("initialJumpSpeed");

		// Pablo. Inicializo la gravedad
		if(entityInfo->hasAttribute("gravity"))
			_gravity = entityInfo->getFloatAttribute("gravity");
		//gravity = 9.8; //expresada en metros /seg

		// velocidad inicial. es 25 m / seg

		//Pablo. Calculo la altura maxima del salto
		//Hmax = (_jumpSpeed*_jumpSpeed) / (2*gravity); 
		// 31,88 m es la altura maxima del salto ( en nuestro caso son pixeles)
		if(entityInfo->hasAttribute("Hmax"))
			_Hmax = entityInfo->getFloatAttribute("Hmax");

		//Pablo. Tiempo hasta alcanzar la máxima altura
		// Vi - Vf / g (Velocidad inicial - Velocidad final / aceleracion). La Vf es 0 en altura max.
		_Tmaxaltura = _jumpSpeed / _gravity; // 2,55 segundos
		_Tmax = _Tmaxaltura * 2; // 5,10 seg. 
		_potenciaSalto=_jumpSpeed;
		_entity->setJumping(false);
		return true;
		}

	bool CAngularMovement::activate()
	{
		_sentidoDerecha=_entity->getSense();
		
		//if (_entity->getType().compare("Player")!=0)
		if (!_entity->isPlayer())
			_entity->setJumping(false); // Pablo
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
		//if (_entity->getType().compare("Player")==0)
		if (_entity->isPlayer())
			stopMovement();
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
					_correccionGrados=message._float;
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
			/*if(!message._string.compare("goUp"))
				goUp();
			else if(!message._string.compare("goDown"))
				goDown();
			*/ if(!message._string.compare("walkLeft"))
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
					_correccionGrados=message._float;
					walkBack();
				 }
			else if(!message._string.compare("changeDirection"))
				 {	
					_sentidoColision=message._bool;
					changeDirection(_sentidoColision);
				 }
			else if(!message._string.compare("changeBase"))
				 {	
					changeBase(message._float);
				 }
			else if(!message._string.compare("turn"))
				turn(message._float);
			}
		}
	}

		void CAngularMovement::walkRight()
		{
			_walkingRight = true;
			_entity->setSense(LogicalPosition::DERECHA);			
		}
		
		void CAngularMovement::walkLeft()
		{
			_walkingLeft = true;
			_entity->setSense(LogicalPosition::IZQUIERDA);			
		}

		// Pablo
		void CAngularMovement::jump()
		{
			_initialJump = true;
		}
	
		void CAngularMovement::walkBack()
		{			
			_walkBack=true; //para retroceder en las colisiones   
		}		
		
		void CAngularMovement::goDown()
		{
			_changingRing=true;
			//Pablo. Sólo si no esta saltandose puede realizar la accion de cambio de anillo.
			if(_entity->getJumping()==false)
			{
				_changingRing=true;
				if (_entity->getRing()==Ring::ANILLO_CENTRAL)
				{
					_entity->setRing(Ring::ANILLO_INFERIOR);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
				if (_entity->getRing()==Ring::ANILLO_SUPERIOR)
				{
					_entity->setRing(Ring::ANILLO_CENTRAL);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
			}			
		}
		
		void CAngularMovement::goUp()
		{
			_changingRing=true;
			//Pablo. Sólo si no esta saltandose puede realizar la accion de cambio de anillo.
			if(_entity->getJumping()==false)
			{
				_changingRing=true;	
				if (_entity->getRing()==Ring::ANILLO_CENTRAL)
				{
					_entity->setRing(Ring::ANILLO_SUPERIOR);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
				if (_entity->getRing()==Ring::ANILLO_INFERIOR)
				{
					_entity->setRing(Ring::ANILLO_CENTRAL);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
			}			
		}
		
		void CAngularMovement::changeBase(int base)
		{
			_changingBase=true;
			//Pablo. Sólo si no esta saltandose puede realizar la accion de cambio de anillo.
			if(_entity->getJumping()==false && !_walkingLeft && !_walkingRight)
			{
				_changingBase=true;	
				if (_entity->getRing()==Ring::ANILLO_CENTRAL)
				{
					_entity->setBase(base);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
				if (_entity->getRing()==Ring::ANILLO_INFERIOR)
				{
					_entity->setBase(base);
					Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
					_entity->setPosition(newPosition);
				}
			}
		}

		void CAngularMovement::stopMovement()
		{
			_walkingLeft = _walkingRight = false;
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

			if (_changingBase || _changingRing)
			{
				if(_entity->isPlayer())
				{
					Logic::TMessage m;		
					m._string="transito";
					m._type = Logic::Message::SET_SHADER;
					_entity->emitMessage(m,this);
					//_changingBase=_changingRing=false;
					if (_changingBase)
					{
						_changingBaseTime+=msecs;
						if (_changingBaseTime>_maxChangingBaseTime)
						{
							_changingBase=false;
							_changingBaseTime=0;
							Logic::TMessage m;		
							m._string="marine";
							m._type = Logic::Message::SET_SHADER;
							_entity->emitMessage(m,this);
						}
					}
					if (_changingRing)
						{
						_changingRingTime+=msecs;
						if (_changingRingTime>_maxChangingRingTime)
						{
							_changingRing=false;
							_changingRingTime=0;
							Logic::TMessage m;		
							m._string="marine";
							m._type = Logic::Message::SET_SHADER;
							_entity->emitMessage(m,this);
						}
					}
				}
			}
			else
			{
				Vector3 direction(Vector3::ZERO);

				if(_walkingLeft || _walkingRight || _initialJump || _entity->getJumping())
				{
					//si la entidad se mueve a la derecha o a la izquierda
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
							if (!_walkBack)
							{
								_entity->setDegree(_entity->getDegree()-_angularSpeed*msecs); 
								_entity->yaw(Math::fromDegreesToRadians(_angularSpeed*msecs));
							}
						}
						else
						{
							if(_sentidoDerecha==false)
							{
								_entity->yaw(Math::PI);					
								_sentidoDerecha=true;
							}
							if (!_walkBack)
							{
								_entity->setDegree(_entity->getDegree()+_angularSpeed*msecs);
								_entity->yaw(Math::fromDegreesToRadians(-_angularSpeed*msecs));
							}			
					
						}

						/*
						WALKBACK
						*/
						if(_walkingLeft)
							direction *= -1;
					} //fin if(_walkingLeft || _walkingRight)

				direction.normalise();
				//_entity->getPosition();



				// SALTO

				if (_entity->isPlayer())
				{
					//si la entidad esta saltando
					if(_initialJump==true && _entity->getJumping()==false)
					{
						 //std::cout << "Tiempo Inicial: " << _time  << "\n";
						// Pablo. Inicializo a false _initialJump para que solo lo tenga en cuenta una vez.
						_potenciaSalto=_jumpSpeed;
						_initialJump = false;
						_entity->setJumping(true);
						_timeJumping = 0;
						Vector3 newPositionInitial=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
						//inicialY = _entity->getY(_entity->getBase(),_entity->getRing());
						_inicialY = newPositionInitial.y;
						Vector3 newPosition2=newPositionInitial;//_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
						_posicionSalto = newPosition2;
			
					}

					//Pablo Si la entidad esta saltando
					if(_entity->getJumping())
					{


						// std::cout << "_timeJumping: " << _timeJumping  << "\n";
						//_timeJumping se incrementa en cada tick
						_timeJumping+=msecs/1000.0; //tiempo en segundos


						if(!_jumpingDown) //esta subiendo en el salto
						{
							//Velocidad = VelocidadInicial - g*t
							//_speed es la velocidad que lleva el personaje en el salto en un determinado momento de tiempo
							_speed = _jumpSpeed - (_gravity * _timeJumping);
							_posicionSalto.y= _inicialY + (_jumpSpeed*_timeJumping) - (_gravity*(_timeJumping*_timeJumping))/2;
						}
						else{ //esta bajando en el salto
							_speed = _jumpSpeed + (_gravity * _timeJumping);
							_posicionSalto.y-= ( (_jumpSpeed*_timeJumping) + (_gravity*(_timeJumping*_timeJumping))/2);
						}



						/*
						//si esta saltando, y esta bajando decrementamos la y en 1 unidad cada tick de reloj
						if(_jumpingDown){
							_posicionSalto.y-= (_jumpSpeed-_potenciaSalto)*(0.04*msecs);
							if (_potenciaSalto>0)
								_potenciaSalto-=0.3;
						}
						
						else //si esta saltando y esta subiendo incrementamos la y en 1 unidad cada tick de reloj
						{
							_posicionSalto.y+= _potenciaSalto * (0.04*msecs);

							if (_potenciaSalto>0)
								_potenciaSalto-=0.3;
						}*/
						

						
						//control para que no suba más arriba que su inicialY + la altura maxima del salto
						if(_posicionSalto.y>=_inicialY+_Hmax)
						{
							_jumpingDown=true;
							_timeJumping = 0;
							////_potenciaSalto=_jumpSpeed-0.5;
						}
						//control para que no baje más que su inicialY
						if(_posicionSalto.y<_inicialY)
						{
							_posicionSalto.y=_inicialY;
							_jumpingDown=false;
							_entity->setJumping(false);
						}
						


						direction.normalise();


						//Pablo 07-02-2013
						/*if(_timeJumping >= _Tmaxaltura ) {
							_jumpingDown=true;
							_speed = _jumpSpeed;
						}

						if(_timeJumping >= _Tmax ) {
							_entity->setJumping(false);
						}*/

					} //fin del if(_entity->getJumping())
				}// FIN if(_entity->isPlayer())

			// Fin Pablo. Salto



				//FIN SALTO
				Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
			
				//newPosition.y=_entity->getY();
				if (_entity->getJumping())
					newPosition.y=_posicionSalto.y;
				_entity->setPosition(newPosition);
				direction.normalise();
			
			}


			// Pablo. Salto


			if (_walkBack)
			{
				//stopMovement();   
				_walkBack=false;

				if (_sentidoColision) // la direccion
				{
					_entity->setDegree(_entity->getDegree()+((_angularSpeed+(_correccionGrados/msecs))*msecs)); 
					_entity->yaw(Math::fromDegreesToRadians(-(_angularSpeed*msecs)));
				}
				else
				{
					_entity->setDegree(_entity->getDegree()-((_angularSpeed+(_correccionGrados/msecs))*msecs)); 
					_entity->yaw(Math::fromDegreesToRadians((_angularSpeed)));
				}							
				
				_sentidoColision=false;
				Vector3 newPositionWalkBack=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());		
				
				if (_entity->getJumping())
					newPositionWalkBack.y=_posicionSalto.y;
				_entity->setPosition(newPositionWalkBack);

			}
		}
	} //fin de CAngularMovement:tick



		void CAngularMovement::turn(float amount)
		{
				_entity->yaw(amount);		
		}

} // namespace Logic