/**
@file PhysicCharacter.cpp

Contiene la implementaci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::CPhysicCharacter
@see Logic::CPhysicEntity
@see Physics::IObserver

@author ���
@date 21-02-2013
*/

#include "PhysicCharacter.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h" // TODO PeP: ser�a �ptimo enviar un unsigned short???
#include "Logic/Entity/Messages/MessageFloat.h"

#include "Physics/Server.h"



#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::PHYSIC_CHARACTER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace Logic {

	IMP_FACTORY(CPhysicCharacter);

	//---------------------------------------------------------

	bool CPhysicCharacter::accept(const CMessage *message)
	{
		return message->getType() == Message::AVATAR_MOVE;
	} 

	//---------------------------------------------------------

	// Acumulamos las diferentes coordenadas vector de desplazamiento para usarlo posteriormente en  el m�todo tick.
	// De esa forma, si recibimos varios mensajes AVATAR_WALK tendremos la �ltima coordenada de cada tipo (degrees, height, ring, base)
	void CPhysicCharacter::process(CMessage *message)
	{
		switch( message->getAction() ) {
			
		case Message::WALK_LEFT:
		case Message::WALK_RIGHT:
			_movDegrees = static_cast<CMessageFloat*>(message)->getFloat();	
			break;
		case Message::JUMP:
			_movHeight = static_cast<CMessageFloat*>(message)->getFloat();
			break;
		case Message::CHANGE_RING:		// TODO ��� por seguridad quiz� habr�a que probar que _ring < MAX del enum --> asserts!
			_movRing = static_cast<CMessageChar*>(message)->getChar();
			break;
		case Message::CHANGE_BASE:
			_movBase = static_cast<CMessageChar*>(message)->getChar();	
			break;

		} // switch message action

		LOG("Movement = " << (int) _movBase << ":" << (int) _movRing  << ":" << _movDegrees << ":" << _movHeight );
		// UNDONE_movement._base << ":" << (int) _movement._ring << ":" << _movement._degrees << ":" << _movement._height );

			//PEP: y ahora ya tenemos la posici�n l�gica completa
			//es posible enviarla entera, enviar _logicalPosReceived, o enviar s�lamente lo que haya cambiado respecto al tick anterior
			//eso se conseguir�a de varias maneras, la primera que se me ocurre es guardar la posici�n l�gica anterior con la que se 
			//acaba de obtener y enviar s�lamente lo que haya cambiado.
			
			//��� Hombre, lo suyo es que los AVATAR_WALK que se reciban sean diferenciales ya de por s� ("vectores de 4 coordenadas" l�gicos unitarios)
			//De ese modo te evitas tener que almacenar la ultima posicion...
	} 

	//---------------------------------------------------------

	void CPhysicCharacter::tick(unsigned int msecs) 
	{
		// Llamar al m�todo de la clase padre (IMPORTANTE).
		IComponent::tick(msecs);

		// Actualizar la posici�n y orientaci�n de la entidad l�gica 
		// usando la informaci�n proporcionada por el motor de f�sica	
		// Este a genera  SET_TRANSFORM por debajo que informa al CGraphics

		_entity->yaw(Math::fromDegreesToRadians(_entity->getLogicalPosition()._degrees - _server->getActorLogicPosition(_physicActor)._degrees));
		_entity->setLogicalPosition( _server->getActorLogicPosition(_physicActor) ); 



		// TODO Efecto de la gravedad quiz� sea necesario..?
		//if (_falling) { // PeP: _entity->getHeight() tambi�n nos proporciona la misma info, si es 0 est� en el suelo.
		//	_movement += Vector3(0,-1,0);
		//}

		// Intentamos mover el actor seg�n los AVATAR_MOVE acumulados. 
		 _server->moveActor(_physicActor, _movDegrees, _movHeight, _movRing, _movBase); //_movement); // TODO a�adir msecs);

		// TODO Actualizamos el flag que indica si estamos cayendo
		//_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);
		
		//UNDONE_movement = TLogicalPosition(); // Ponemos el movimiento a cero
		
		_movDegrees = 0;
		_movHeight = 0;
		_movRing = 0;
		_movBase = 0;
	}


} // Logic


