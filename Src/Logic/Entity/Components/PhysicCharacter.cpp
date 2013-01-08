/**
@file PhysicCharacter.cpp

Contiene la implementación del componente que se utiliza para representar jugadores y enemigos en
el mundo físico usando character controllers.

@see Logic::CPhysicCharacter
@see Logic::CPhysicEntity

@author Antonio Sánchez Ruiz-Granados
@date Octubre, 2010
*/

#include "PhysicCharacter.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/PhysicModelCharacter.h"
#include "Physics/PhysicObjCharacter.h"


using namespace Physics;
using namespace Logic;

IMP_FACTORY(CPhysicCharacter);

//---------------------------------------------------------

CPhysicCharacter::CPhysicCharacter() : CPhysicEntity(), _offsetY(0), 
								       _movement(0,0,0), _falling(false)
{

}

//---------------------------------------------------------

CPhysicCharacter::~CPhysicCharacter() 
{

} 

//---------------------------------------------------------

bool CPhysicCharacter::accept(const TMessage &message)
{
	return message._type == Message::AVATAR_WALK;
} 

//---------------------------------------------------------

void CPhysicCharacter::process(const TMessage &message)
{
	switch(message._type)
	{
	case Message::AVATAR_WALK:
		// Anotamos el vector de desplazamiento para usarlo posteriormente en 
		// el método tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
		// en el mismo ciclo sólo tendremos en cuenta el último.
		_movement = message._vector3;
		break;
	}

} 

//---------------------------------------------------------

void CPhysicCharacter::tick(unsigned int msecs) 
{
	// Llamar al método de la clase padre (IMPORTANTE).
	IComponent::tick(msecs);

	// Si estamos cayendo modificar el vector de desplazamiento para
	// simular el efecto de la gravedad
	if (_falling) {
		_movement += Vector3(0,-1,0);
	}

	// Mover el character controller y actualizar el flag de caida
	unsigned flags = _physicServer->moveCharacter((CPhysicObjCharacter *)_physicObj, _movement);
	_falling =  !(flags & NXCC_COLLISION_DOWN);
	_movement = Vector3::ZERO;
	
	// Los controllers no tienen orientación, así que sólo actualizamos su posición.
	// Es importante transformar entre posiciones lógicas y posiciones físicas.
	Vector3 physicPos = _physicServer->getPosition(_physicObj);
	_entity->setPosition(fromPhysicsToLogic(physicPos));
}

//---------------------------------------------------------

IPhysicObj* CPhysicCharacter::createPhysicEntity(const Map::CEntity *entityInfo)
{
	// Creamos la entidad física invocando al método de la clase padre.
	CPhysicObjCharacter *obj = (CPhysicObjCharacter *) CPhysicEntity::createPhysicEntity(entityInfo);

	// A continuación debemos colocar la entidad física en su sitio, ya que con la invocación
	// al método anterior no hemos tenido en cuenta la diferencia de sistemas de coordenadas.
	// Además sumaremos un pequeño desplazamiento en el eje Y para asegurarnos de que el
	// controller queda por encima del suelo y no lo atraviesa.
	Vector3 pos = fromLogicToPhysics(_physicServer->getPosition(obj)) + Vector3(0,2,0);
	_physicServer->setPosition(obj, pos);

	return obj;
} 

//---------------------------------------------------------

IPhysicModel* CPhysicCharacter::createPhysicModel(const Map::CEntity *entityInfo)
{
	// Comprobar que estamos creando una entidad física de tipo character controller
	const std::string entity = entityInfo->getStringAttribute(STR_PHYSIC_ENTITY);
	assert(entity == STR_CONTROLLER);

	// Comprobar que el volumen de colision es una capsula
	if (entityInfo->hasAttribute(STR_PHYSIC_SHAPE)) {
		const std::string shape = entityInfo->getStringAttribute(STR_PHYSIC_SHAPE);
		assert(shape == STR_CAPSULE && 
			  "Tipo de shape no permitida en un character controller");
	}

	// Crear el modelo físico
	float radius = entityInfo->getFloatAttribute(STR_PHYSIC_RADIUS);
	float height = entityInfo->getFloatAttribute(STR_PHYSIC_HEIGHT);
	CPhysicModelCharacter *model = _physicServer->createModelCapsuleCharacter(radius, height);
		
	// Calculamos el desplazamiento necesario para transformar entre el sistema de 
	// coordenadas local de PhysX, que tiene su origen en el centro de la entidad, 
	// y el de la lógica, que tiene su origen en los pies de la entidad.
	_offsetY = height / 2.0f + radius;
	
	return model;
}

//---------------------------------------------------------







