#include "Movement.h"

#include "KinematicMovement.h"
#include "DynamicMovement.h"

namespace AI {

	/**
	Constante en la que se almacena un valor de tiempo que medirá los 
	milisegundos que queremos que transcurran hasta alcanzar el destino de un
	movimiento.
	*/
	const float IMovement::TIME_TO_TARGET = 250.0f;
	/**
	Constante en la que se almacena un valor de distancia que medirá a qué 
	distancia se empieza a ralentizar el movimiento para llegar a un destino.
	*/
	const float IMovement::SLOW_RADIUS = 30.0f;
	/////////////////////////////////////////
	/**
	Método de factoría que devuelve un movimiento a partir de un enumerado.
	*/
	IMovement* IMovement::getMovement(int type, float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel)
	{
			switch (type) {
				case MOVEMENT_KINEMATIC_SEEK:
					return  new CKinematicSeek(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_KINEMATIC_ARRIVE:
					return  new CKinematicArrive(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_KINEMATIC_ALIGN_TO_SPEED:
					return  new CKinematicAlignToSpeed(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_DYNAMIC_SEEK:
					return  new CDynamicSeek(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_DYNAMIC_ARRIVE:
					return  new CDynamicArrive(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				default:
					return 0;
			}
	};
}