/**
@file PhysicCharacter.cpp

Contiene la implementación del componente que se utiliza para representar jugadores y enemigos en
el mundo físico usando character controllers.

@see Logic::CPhysicCharacter
@see Logic::CPhysicEntity
@see Physics::IObserver

@author ƒ®§
@date 21-02-2013
*/

#include "PhysicCharacter.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageInt.h" // TODO PeP: sería óptimo enviar un unsigned short???
#include "Logic/Entity/Messages/MessageFloat.h"

#include "Physics/Server.h"


namespace Logic {

	IMP_FACTORY(CPhysicCharacter);

	//---------------------------------------------------------

	bool CPhysicCharacter::accept(const CMessage *message)
	{
		return message->getType() == Message::AVATAR_MOVE;
	} 

	//---------------------------------------------------------

	// Acumulamos las diferentes coordenadas vector de desplazamiento para usarlo posteriormente en  el método tick.
	// De esa forma, si recibimos varios mensajes AVATAR_WALK tendremos la última coordenada de cada tipo (degrees, height, ring, base)
	void CPhysicCharacter::process(CMessage *message)
	{
		switch( message->getAction() ) {
			
		case Message::WALK_LEFT:
		case Message::WALK_RIGHT:
			_movement._degrees = static_cast<CMessageFloat*>(message)->getFloat();	
			break;

		case Message::JUMP:
			_movement._height = static_cast<CMessageFloat*>(message)->getFloat();
			break;

		case Message::CHANGE_RING:		// TODO ƒ®§ por seguridad quizá habría que probar que _ring < MAX del enum
			_movement._ring = static_cast<Ring>( static_cast<CMessageInt*>(message)->getInt() );
			break;

		case Message::CHANGE_BASE:
			_movement._base= static_cast<CMessageInt*>(message)->getInt();	
			break;

		} // switch message action

			//PEP: y ahora ya tenemos la posición lógica completa
			//es posible enviarla entera, enviar _logicalPosReceived, o enviar sólamente lo que haya cambiado respecto al tick anterior
			//eso se conseguiría de varias maneras, la primera que se me ocurre es guardar la posición lógica anterior con la que se 
			//acaba de obtener y enviar sólamente lo que haya cambiado.
			
			//ƒ®§ Hombre, lo suyo es que los AVATAR_WALK que se reciban sean diferenciales ya de por sí ("vectores de 4 coordenadas" lógicos unitarios)
			//De ese modo te evitas tener que almacenar la ultima posicion...
	} 

	//---------------------------------------------------------

	void CPhysicCharacter::tick(unsigned int msecs) 
	{
		// Llamar al método de la clase padre (IMPORTANTE).
		IComponent::tick(msecs);

		// Actualizar la posición y orientación de la entidad lógica 
		// usando la información proporcionada por el motor de física	
		// Este a genera  SET_TRANSFORM por debajo que informa al CGraphics
		_entity->setLogicalPosition( _server->getActorLogicPosition(_physicActor) );  

		// TODO Efecto de la gravedad quizá sea necesario..?
		//if (_falling) { // PeP: _entity->getHeight() también nos proporciona la misma info, si es 0 está en el suelo.
		//	_movement += Vector3(0,-1,0);
		//}

		// Intentamos mover el actor según los AVATAR_MOVE acumulados. 
		 _server->moveActor(_physicActor, _movement); // TODO añadir msecs);

		// TODO Actualizamos el flag que indica si estamos cayendo
		//_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);
		
		_movement = TLogicalPosition(); // Ponemos el movimiento a cero
	}


} // Logic


