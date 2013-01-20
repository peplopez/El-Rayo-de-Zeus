/**
@file JumpMovement.h

Contiene la declaración del componente que controla el movimiento 
angular de entidades.

@see Logic::CJumpMovement
@see Logic::IComponent

@author Pablo Terrado
@date Enero, 2013
*/

#include "JumpMovement.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
 

//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CJumpMovement);

	bool CJumpMovement::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// Pablo. Si la entidad tiene el atributo jumpSpeed la capturamos y la guardamos en _jumpSpeed
		// En ppio solo la va a tener el player
		if(entityInfo->hasAttribute("initialJumpSpeed"))
			jumpSpeed = entityInfo->getFloatAttribute("initialJumpSpeed");

		// Pablo. Inicializo la gravedad
		gravity = 9.8 * 0.001; //expresada en metros / milesimas de segundo

		// Paso la velocidad inicial de salto a metros por milisegundos (en lugar de metros/seg)
		jumpSpeed*= 0.001;

		//Pablo. Calculo la altura maxima del salto
		Hmax = (jumpSpeed*jumpSpeed) / (2*gravity);

		//Pablo. Tiempo hasta alcanzar la máxima altura
		// Vi - Vf / g (Velocidad inicial - Velocidad final / aceleracion). La Vf es 0 en altura max.
		Tmaxaltura = jumpSpeed / gravity;
		Tmax = Tmaxaltura * 2;

		return true;
		}

	bool CJumpMovement::activate()
	{
		
		if (_entity->getType().compare("Player")!=0)
			_jumping = false;
		return true;
	}
		

	void CJumpMovement::deactivate(){}

	
	bool CJumpMovement::accept(const TMessage &message)
	{
		if (_entity->getType().compare("Player")==0)
			return message._type == Message::CONTROL;
		 if (_entity->getType().compare("AnimatedEntity")==0)
			return message._type == Message::NPC_CONTROL;
	}

		
	 void CJumpMovement::process(const TMessage &message)
	 {
		switch(message._type)
		{

			case Message::CONTROL:
			{
				if(!message._string.compare("jump")) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
					jump();
			}
			case Message::NPC_CONTROL:
			{
				if(!message._string.compare("jump")) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
					jump();
			}
		} //fin del switch
	}//fin de CJumpMovement::process(const TMessage &message)


		// Pablo
		void CJumpMovement::jump()
		{
			_jumping = true;

			// Pablo

			//_entity->getY(_entity->getBase(), _entity->getRing(),_jumping);
			//_entity->setPosition(_entity->getPosition());
			// Fin Pablo

			// Cambiamos la animación
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Jump";
			message._bool = true;
			_entity->emitMessage(message,this);

		}
	
	void CJumpMovement::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		// Si estamos saltando calculamos la próxima posición
		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.


	    Vector3 direction(Vector3::ZERO);

		//Pablo
		if(_jumping)
		{
			direction = Math::getDirection(_entity->getYaw() + Math::PI/2);

			Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());


			newPosition.y=_entity->getYJump(_entity->getBase(),_entity->getRing());
			_entity->setPosition(newPosition);
			direction.normalise();

			_jumping = false;
		}

			direction.normalise();
			_entity->getPosition();

			Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
			
			newPosition.y=_entity->getYJump(_entity->getBase(),_entity->getRing());
			_entity->setPosition(newPosition);
			direction.normalise();
		
	} //fin del CJumpMovement::tick

} // namespace Logic