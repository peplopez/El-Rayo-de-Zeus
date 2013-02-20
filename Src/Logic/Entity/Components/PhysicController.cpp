/**
@file PhysicController.cpp

Contiene la implementación del componente que se utiliza para representar jugadores y enemigos en
el mundo físico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
//#include "Physics/Server.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageInt.h" //PeP: sería óptimo enviar un unsigned short???
#include "Logic/Entity/Messages/MessageFloat.h"

//PT Bomba de humo (particula)
#include "Graphics/Scene.h"
#include "Graphics/Server.h"

//#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicController);

//---------------------------------------------------------

CPhysicController::CPhysicController() : IPhysics(),// _controller(NULL), 
								       _movement(0,0,0), _falling(false)
{	//dejo la creación del proyecto de física para el domingo
	//_server = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicController::~CPhysicController() 
{//para el domingo
	/*if (_controller) {
		_controller->release();
		_controller = NULL;
	}

	_server = NULL;*/
} 

//---------------------------------------------------------

bool CPhysicController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
{
	// Invocar al método de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

		_logicalPosReceived=_entity->getLogicalPosition();
	// Crear el character controller asociado al componente
//	_controller = createController(entityInfo);

	return true;
}

//---------------------------------------------------------

bool CPhysicController::accept(const CMessage *message)
{
	return message->getType() == Message::AVATAR_MOVE;
} 

//---------------------------------------------------------

void CPhysicController::process(CMessage *message)
{
	switch(message->getType())
	{
	case Message::AVATAR_MOVE:
		if(message->getAction() == Message::WALK_LEFT || message->getAction() == Message::WALK_RIGHT)
		{
			if(message->getAction() == Message::WALK_LEFT)
				_logicalPosReceived._sense=Sense::IZQUIERDA;
			else
				_logicalPosReceived._sense=Sense::DERECHA;			
			
			CMessageFloat* maux = static_cast<CMessageFloat*>(message);
			_logicalPosReceived._degrees=maux->getFloat();
			
		} else if(message->getAction() == Message::JUMP)
		{
			CMessageFloat* maux = static_cast<CMessageFloat*>(message);
			_logicalPosReceived._height=maux->getFloat();
		} else if(message->getAction()== Message::CHANGE_RING)
		{
			CMessageInt* maux = static_cast<CMessageInt*>(message);
			

			// PT Creamos nuestro sistema de partículas :)
			if(_entity->isPlayer())
			{
				_entity->getName();
				Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
				_scen->createParticula(_entity->getName(),"SmokeParticles");
			}



			switch (maux->getInt())
			{
					case Logic::LogicalPosition::ANILLO_INFERIOR:
					{
						_logicalPosReceived._ring= Logic::LogicalPosition::ANILLO_INFERIOR;
						break;
					}
					case Logic::LogicalPosition::ANILLO_CENTRAL:
					{
						_logicalPosReceived._ring = Logic::LogicalPosition::ANILLO_CENTRAL;				
						break;
					}
					case Logic::LogicalPosition::ANILLO_SUPERIOR:
					{
						_logicalPosReceived._ring = Logic::LogicalPosition::ANILLO_SUPERIOR;
						break;
					}
					default:
					{
						_logicalPosReceived._ring = Logic::LogicalPosition::ANILLO_CENTRAL;
						break;
					}
			}
		}else if(message->getAction()== Message::CHANGE_BASE)
		{
			CMessageInt* maux = static_cast<CMessageInt*>(message);
			_logicalPosReceived._base= maux->getInt();	
		}
		//PEP: y ahora ya tenemos la posición lógica completa
		//es posible enviarla entera, enviar _logicalPosReceived, o enviar sólamente lo que haya cambiado respecto al tick anterior
		//eso se conseguiría de varias maneras, la primera que se me ocurre es guardar la posición lógica anterior con la que se 
		//acaba de obtener y enviar sólamente lo que haya cambiado.

		// Anotamos el vector de desplazamiento para usarlo posteriormente en 
		// el método tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
		// en el mismo ciclo sólo tendremos en cuenta el último.
		//_movement = message._vector3;
		break;
	}

} 

//---------------------------------------------------------

void CPhysicController::tick(unsigned int msecs) 
{
	// Llamar al método de la clase padre (IMPORTANTE).
	IComponent::tick(msecs);



	// Actualizar la posición y orientación de la entidad lógica usando la 
	// información proporcionada por el motor de física	
	//_entity->setPosition(_server->getControllerPosition(_controller));

	// Si estamos cayendo modificar el vector de desplazamiento para simular el 
	// efecto de la gravedad. Lo hacemos de manera sencilla y pero poco realista.
	//if (_falling) {
		//_movement += Vector3(0,-1,0);
//	}

	// Intentamos mover el controller a la posición recibida en el último mensaje 
	// de tipo AVATAR_WALK. 
	//unsigned flags = _server->moveController(_controller, _movement, msecs);

	// Actualizamos el flag que indica si estamos cayendo
	//_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);

	// Ponemos el movimiento a cero
	//_movement = Vector3::ZERO;
	/* y aquí pone la posición final, ahora sin comprobación física pero recogiendo todos los datos de los componentes de la entidad*/
	_entity->setLogicalPosition(_logicalPosReceived);
	
	Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getHeight(),_entity->getBase(),_entity->getRing());
	_entity->setPosition(newPosition);


}

//---------------------------------------------------------
/*
PxCapsuleController* CPhysicController::createController(const Map::CEntity *entityInfo)
{
	// Obtenemos la posición de la entidad. Inicialmente colocaremos el controller
	// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
	// lo atraviesa en el primer ciclo de simulación.
	Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);
	
	// Leer el volumen de colisión del controller. Por ahora sólo admitimos cápsulas.
	std::string shape = "capsule";
	if (entityInfo->hasAttribute("physic_shape")) {
		shape = entityInfo->getStringAttribute("physic_shape");
		assert(shape == "capsule");
	}

	// Leer el radio de la cápsula
	assert(entityInfo->hasAttribute("physic_radius"));
	float radius = entityInfo->getFloatAttribute("physic_radius");

	// Leer la altura de la cápsula
	assert(entityInfo->hasAttribute("physic_height"));
	float height = entityInfo->getFloatAttribute("physic_height");

	// Crear el controller de tipo cápsula
	return _server->createCapsuleController(position, radius, height, this);
} 
*/
//---------------------------------------------------------

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter)
{

}

//---------------------------------------------------------
/*
void CPhysicController::onShapeHit (const PxControllerShapeHit &hit)
{
	// Si chocamos contra una entidad estática no hacemos nada
	PxRigidDynamic* actor = hit.shape->getActor().isRigidDynamic();
	if(!actor)
		return;

	// Si chocamos contra una entidad cinemática no hacemos nada
	if (_server->isKinematic(actor))
		return;
	
	// Aplicar una fuerza a la entidad en la dirección del movimiento
	actor->addForce(hit.dir * hit.length * 1000.0f);
}*/

//---------------------------------------------------------
/*
void CPhysicController::onControllerHit (const PxControllersHit &hit)
{

}
*/
//---------------------------------------------------------



