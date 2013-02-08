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
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageBool.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"

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


		// Pablo. Si la entidad tiene el atributo jumpSpeed la capturamos y la guardamos en _jumpSpeed
		// En ppio solo la va a tener el player
		if(entityInfo->hasAttribute("initialJumpSpeed"))
			jumpSpeed = entityInfo->getFloatAttribute("initialJumpSpeed");

		// Pablo. Inicializo la gravedad
		if(entityInfo->hasAttribute("gravity"))
			_gravity = entityInfo->getFloatAttribute("gravity");
		//gravity = 9.8; //expresada en metros /seg

		// velocidad inicial. es 28 m / seg

		//Pablo. Calculo la altura maxima del salto
		//Hmax = (jumpSpeed*jumpSpeed) / (2*gravity); // 40 m es la altura maxima del salto ( en nuestro caso son pixeles)
		if(entityInfo->hasAttribute("Hmax"))
			_Hmax = entityInfo->getFloatAttribute("Hmax");
		//Hmax = 40;

		//Pablo. Tiempo hasta alcanzar la máxima altura
		// Vi - Vf / g (Velocidad inicial - Velocidad final / aceleracion). La Vf es 0 en altura max.
		_Tmaxaltura = jumpSpeed / _gravity; // 2,857142 segundos --> 2857,142 milisegundos
		_Tmax = _Tmaxaltura * 2; // 5,71428571 seg. --> 5714,28571 milisegundos.
		_potenciaSalto=_potenciaSaltoInicial;
		_entity->setJumping(false);
			//_isJumping=
		return true;
		}

	bool CAngularMovement::activate()
	{
		_sentidoDerecha=_entity->getSense();
		
		if (_entity->getType().compare("Player")!=0)
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
		if (_entity->getType().compare("Player")==0)
			stopMovement();
		return true;
	}
		

	void CAngularMovement::deactivate(){}

	
	bool CAngularMovement::accept(const CMessage *message)
	{
		if (_entity->getType().compare("Player")==0)
			return message->getType() == Message::CONTROL;
		 if (_entity->getType().compare("AnimatedEntity")==0)
			return message->getType() == Message::NPC_CONTROL;
	}

		
	 void CAngularMovement::process(CMessage *message)
		 {
		switch(message->getType())
		{
		case Message::CONTROL:
			{
			if(message->getAction() == Message::GO_UP)
				goUp();
			else if(message->getAction() == Message::GO_DOWN)
				goDown();
			else if(message->getAction() == Message::WALK_LEFT)
				walkLeft();
			else if(message->getAction() == Message::WALK_RIGHT)
				walkRight();
			else if(message->getAction() == Message::WALK_STOP)
				stopMovement();
			else if(message->getAction() == Message::JUMP) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
					jump();
			else if(message->getAction() == Message::WALK_BACK)
				 {	
					CMessageBoolFloat* maux = static_cast<CMessageBoolFloat*>(message);
					_sentidoColision=maux->getBool();
					_correccionGrados=maux->getFloat();
					walkBack();
				 }
			else if(message->getAction() == Message::CHANGE_DIRECTION)
				 {	
					CMessageBool* maux = static_cast<CMessageBool*>(message);
					 _sentidoColision=maux->getBool();
					changeDirection(_sentidoColision);
				 }
			else if(message->getAction() == Message::TURN)
				{
					CMessageFloat* maux = static_cast<CMessageFloat*>(message);
					turn(maux->getFloat());
				}
			}
		case Message::NPC_CONTROL:
			{
			/*if(!message._string.compare("goUp"))
				goUp();
			else if(!message._string.compare("goDown"))
				goDown();
			*/ if(message->getAction() == Message::WALK_LEFT)
				walkLeft();
			else if(message->getAction() == Message::WALK_RIGHT)
				walkRight();
			else if(message->getAction() == Message::WALK_STOP)
				stopMovement();
			else if(message->getAction() == Message::JUMP) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
					jump();
			else if(message->getAction() == Message::WALK_BACK)
				 {	
					CMessageBoolFloat* maux = static_cast<CMessageBoolFloat*>(message);
					_sentidoColision=maux->getBool();
					_correccionGrados=maux->getFloat();
					walkBack();
				 }
			else if(message->getAction() == Message::CHANGE_DIRECTION)
				 {	
					CMessageBool* maux = static_cast<CMessageBool*>(message);
					_sentidoColision=maux->getBool();
					changeDirection(_sentidoColision);
				 }
			else if(message->getAction() == Message::CHANGE_BASE)
				 {	
					CMessageFloat* maux = static_cast<CMessageFloat*>(message);
					changeBase(maux->getFloat());
				 }
			else if(message->getAction() == Message::TURN)
				{
					CMessageFloat* maux = static_cast<CMessageFloat*>(message);
					turn(maux->getFloat());
				}
			}
		}
	}

		void CAngularMovement::walkRight()
		{
			_walkingRight = true;
			// Cambiamos la animación
			_entity->setSense(LogicalPosition::DERECHA);
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("RunKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);

		}
		
		void CAngularMovement::walkLeft()
		{
			_walkingLeft = true;
			
			// Cambiamos la animación
			_entity->setSense(LogicalPosition::IZQUIERDA);
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("RunKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);

		}

		// Pablo
		void CAngularMovement::jump()
		{
			_initialJump = true;
		}
	
		void CAngularMovement::walkBack()
		{			
			_walkBack=true; //para retroceder en las colisiones   
			// Cambiamos la animación
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("RunKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);
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
			//if(!_jumping)
				
			_walkingLeft = _walkingRight = false;

		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		
			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("IdleKatana");
			message->setBool(true);
			_entity->emitMessage(message,this);
		}
		
		
	void CAngularMovement::changeDirection(bool newDirection)
	{		
				CMessageString *m = new CMessageString();
				m->setType(Message::SET_SHADER);
				m->setString("marine");
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

			if (_changingBase || _changingRing)
			{
				if (_entity->getType().compare("Player")==0)
				{
					CMessageString *m = new CMessageString();	
					m->setType(Message::SET_SHADER);
					m->setString("transito");
					_entity->emitMessage(m,this);
					_changingBase=_changingRing=false;
				}
			}
			else
			{
			Vector3 direction(Vector3::ZERO);
				

			if(_walkingLeft || _walkingRight || _initialJump || _entity->getJumping())
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
					/*if (!_walkingLeft && !_walkingRight && _jumping){
							_entity->setDegree(_entity->getDegree()+_angularSpeed);
							_entity->yaw(Math::fromDegreesToRadians(-_angularSpeed));
					}*/

					/*
					WALKBACK
					*/
					if(_walkingLeft)
						direction *= -1;
				}

				direction.normalise();
				//_entity->getPosition();

				// SALTO

				if (_entity->getType().compare("Player")==0)
			{
					if(_initialJump==true && _entity->getJumping()==false)
					{
						// Pablo. Inicializo a false _initialJump para que solo lo tenga en cuenta una vez.
						_potenciaSalto=_potenciaSaltoInicial;
						_initialJump = false;
						_entity->setJumping(true);
						_timeJumping = 0;
						Vector3 newPositionInitial=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
						//inicialY = _entity->getY(_entity->getBase(),_entity->getRing());
						_inicialY = newPositionInitial.y;
						Vector3 newPosition2=newPositionInitial;//_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
						_posicionSalto = newPosition2;
			
					}

					//Pablo
					if(_entity->getJumping())
					{
						//std::cout << "inicialY: " << inicialY << "\n";
						//std::cout << "posicionSalto.y: " << posicionSalto.y << "\n";

						//_timeJumping se incrementa en cada tick
						//_timeJumping+= msecs;

						//Vector3 newPosition2=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());

						//float altura = jumpSpeed*_timeJumping -(gravity*(_timeJumping*_timeJumping)/2);

						//newPosition.y = inicialY + altura;

						//si esta saltando, y esta bajando decrementamos la y en 1 unidad cada tick de reloj
						if(_jumpingDown){
							_posicionSalto.y-= (_potenciaSaltoInicial-_potenciaSalto)*(0.05*msecs);
							if (_potenciaSalto>0)_potenciaSalto-=0.3;
						}
						else //si esta saltando y esta subiendo incrementamos la y en 1 unidad cada tick de reloj
						{
							_posicionSalto.y+= _potenciaSalto * (0.05*msecs);
							if (_potenciaSalto>0)_potenciaSalto-=0.3;
						}


						//control para que no suba más arriba que su inicialY + la altura maxima del salto
						if(_posicionSalto.y>=_inicialY+_Hmax)
						{
							_jumpingDown=true;
							_potenciaSalto=_potenciaSaltoInicial-0.5;
						}
						//control para que no baje más que su inicialY
						if(_posicionSalto.y<_inicialY)
						{
							_posicionSalto.y=_inicialY;
							_jumpingDown=false;
							_entity->setJumping(false);
						}


						//_entity->setPosition(posicionSalto);
						direction.normalise();

						/*
						if(_timeJumping >= Tmax ) {
							_jumping = false;
						}
						*/

					} //fin del if (_jumping)
			}// FIN if (_entity->getType().compare("Player")!=0)

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