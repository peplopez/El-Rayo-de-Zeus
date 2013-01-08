/**
@file PhysicEntity.cpp

Contiene la implementación del componente encargado de la representación física de las
entidades que no son character controllers.
 
@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicCharacter

@author Antonio Sánchez Ruiz-Granados
@date Octubre, 2010
*/

#include "PhysicEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/PhysicModelSimple.h"
#include "Physics/PhysicModelGeneric.h"
#include "Physics/IPhysicObj.h"

using namespace Logic;
using namespace Physics;

IMP_FACTORY(CPhysicEntity);

//---------------------------------------------------------

CPhysicEntity::CPhysicEntity() : IComponent(), _physicObj(NULL) 
{
	_physicServer = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicEntity::~CPhysicEntity() 
{
	if(_physicObj) {
		// Eliminar el objeto físico de la escena
		_physicServer->destroyObject(_physicObj);
		_physicObj = NULL;
	}
	
	_physicServer = NULL;
} 

//---------------------------------------------------------

bool CPhysicEntity::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al método de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el objeto físico asociado al componente
	_physicObj = createPhysicEntity(entityInfo);

	// Anotar su posición y rotación actual
	_transform = _entity->getTransform();

	return (_physicObj != NULL);
} 

//---------------------------------------------------------

bool CPhysicEntity::accept(const TMessage &message)
{
	return message._type == Message::SET_TRANSFORM;
}

//---------------------------------------------------------

void CPhysicEntity::process(const TMessage &message)
{

	switch(message._type)
	{
	case Message::SET_TRANSFORM:
		// Anotamos la transformación para usarla posteriormente en el método 
		// tick. De esa forma, si recibimos varios mensajes en el mismo ciclo 
		// sólo tendremos en cuenta el último
		_transform =  message._transform;
	break;
	}
}

//---------------------------------------------------------

void CPhysicEntity::tick(unsigned int msecs) 
{
	// Invocar al método de la clase padre (IMPORTANTE)
	IComponent::tick(msecs);

	// Si el objeto físico es kinemático intentamos moverlo a la posición
	// recibida en el último mensaje de tipo SET_TRANSFORM
	if (_physicObj->IsKinematic()) {
		_physicServer->move(_physicObj, _transform);
	} else {
		// Actualizar la posición y la orientación de la entidad lógica usando la 
		// información proporcionada por el motor de física
		_entity->setPosition(_physicServer->getPosition(_physicObj));
		_entity->setOrientation(_physicServer->getOrientation(_physicObj, 0));
	}
}

//---------------------------------------------------------

void CPhysicEntity::onEntityHit(const CEntity *otherEntity) 
{

}

//---------------------------------------------------------

void CPhysicEntity::onEntityTrigger(const CEntity *otherEntity, bool enter, bool leave, bool stay)
{
	// Sólo tenemos en cuenta los eventos de entrada y salida del trigger
	if (stay)
		return;

	// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a todos los
	// componentes de la entidad. 
	TMessage msg;
	if (enter) {
		msg._type = Message::TOUCHED;
	} else {
		msg._type = Message::UNTOUCHED;
	}
	msg._entity = const_cast<CEntity *> (otherEntity);

	_entity->emitMessage(msg);
}

//---------------------------------------------------------

IPhysicObj* CPhysicEntity::createPhysicEntity(const Map::CEntity *entityInfo)
{
	// Obtenemos el modelo físico que describe el tipo de entidad física
	// que debemos crear
	IPhysicModel *model = createPhysicModel(entityInfo);

	// Obtenemos el tipo de entidad física (estática, dinámica, kinemática)
	// particular que queremos crear y su posición en el mundo
	TPhysicMode mode = getPhysicMode(entityInfo);
	Vector3 position = _entity->getPosition();
	Matrix3 orientation = _entity->getOrientation();

	// Creamos la nueva entidad física
	return _physicServer->createObject(this, mode, position, orientation, model); 
}

//---------------------------------------------------------

IPhysicModel* CPhysicEntity::createPhysicModel(const Map::CEntity *entityInfo)
{
	// Obtenemos el tipo de entidad física que debemos crear
	const std::string entity = entityInfo->getStringAttribute(STR_PHYSIC_ENTITY);
	assert((entity == STR_SIMPLE || entity == STR_GENERIC) &&
			"Tipo de entidad no permitida en el componente físico");

	// Creamos el modelo correspondiente
	IPhysicModel *model = NULL;
	if (entity == STR_SIMPLE) {
		model = createPhysicModelSimple(entityInfo);
	} else if (entity == STR_GENERIC) {
		model = createPhysicModelGeneric(entityInfo);
	}

	return model;
}

//---------------------------------------------------------

TPhysicMode CPhysicEntity::getPhysicMode(const Map::CEntity *entityInfo) {
	
	// Los controladores son entidades kinemáticas
	if (entityInfo->getStringAttribute(STR_PHYSIC_ENTITY) == STR_CONTROLLER) {
		return PM_KINEMATIC;
	}
	
	// El resto de entidades definen su tipo explícitamente
	std::string type = entityInfo->getStringAttribute(STR_PHYSIC_TYPE);
	assert(((type == STR_STATIC) || (type == STR_DYNAMIC) || (type == STR_KINEMATIC)) &&
			"Physic type no permitido");

	if (type == STR_STATIC) {
		return PM_STATIC;
	} else if (type == STR_DYNAMIC) {
		return PM_DYNAMIC;
	} else {
		return PM_KINEMATIC;
	} 
}

//---------------------------------------------------------

CPhysicModelSimple* CPhysicEntity::createPhysicModelSimple(const Map::CEntity *entityInfo)
{
	// Comprobamos que estamos creando una entidad física simple
	const std::string entity = entityInfo->getStringAttribute(STR_PHYSIC_ENTITY);
	assert(entity == STR_SIMPLE);

	// Creamos el modelo físico correspondiente
	CPhysicModelSimple *model = _physicServer->createModelSimple();

	// Creamos el volumen de colisión adecuado
	createPhysicShape(entityInfo, model, 1.0f);

	// Comprobamos si estamos definiendo un trigger
	if (entityInfo->hasAttribute(STR_PHYSIC_TRIGGER) && 
		entityInfo->getBoolAttribute(STR_PHYSIC_TRIGGER)) {
			
			// Marcamos el volumen de colisión como trigger
			_physicServer->setTrigger(model, 0, true);

			// Las entidades físicas dinámicas y kinemáticas deben tener al menos un
			// volumen de colisión que no sea un trigger. Intuitivamente este volumen
			// de colisión es el que contiene la masa de la entidad.
			// 
			// En este juego usaremos el grupo de colisión 0 para los triggers kinemáticos.
			// Para que todo funcione correctamente debemos configurar los character controller
			// para que no choquen contra el grupo 0 (método move). Además desactivamos las
			// colisiones de otros grupos contra el 0 al crear la escena.
			std::string type = entityInfo->getStringAttribute(STR_PHYSIC_TYPE);
			if ((type == STR_DYNAMIC) || (type == STR_KINEMATIC)) {
				createPhysicShape(entityInfo, model, 1.0f);
			}
	}

	// Si estamos definiendo un objeto dinámico anotamos su masa.
	if (entityInfo->hasAttribute(STR_PHYSIC_MASS)) {
		float mass = entityInfo->getFloatAttribute(STR_PHYSIC_MASS);
		_physicServer->setMass(model, mass);
	}

	// Devolvemos el modelo resultante
	return model;
}

//---------------------------------------------------------

void CPhysicEntity::createPhysicShape(const Map::CEntity *entityInfo, CPhysicModelSimple *model, float scale)
{
	// Leemos el grupo de colisión al que debe pertenecer la shape
	int group = 0;
	if (entityInfo->hasAttribute(STR_PHYSIC_COLLISION_GROUP)) {
		group = entityInfo->getIntAttribute(STR_PHYSIC_COLLISION_GROUP);
	}
	
	// Crear el volumen de colisión adecuado
	const std::string shape = entityInfo->getStringAttribute(STR_PHYSIC_SHAPE);
	assert(((shape == STR_CAPSULE) || (shape == STR_PLANE) || (shape == STR_BOX)) &&
			"Tipo de shape no permitida en un objeto físico simple");

	// Usar una capsula?
	if (shape == STR_CAPSULE) {
		float radius = entityInfo->getFloatAttribute(STR_PHYSIC_RADIUS);
		float height = entityInfo->getFloatAttribute(STR_PHYSIC_HEIGHT);
		_physicServer->createCapsuleShape(model, radius * scale, height * scale, group);

	// Usar un plano?
	} else if (shape == STR_PLANE) {
		Vector3 normal = entityInfo->getVector3Attribute(STR_PHYSIC_NORMAL);
		_physicServer->createPlaneShape(model, normal, group);
	
	// Usar una caja?
	} else if (shape == STR_BOX) {
		Vector3 dimensions = entityInfo->getVector3Attribute(STR_PHYSIC_DIMENSIONS);
		_physicServer->createBoxShape(model, dimensions * scale, group);

	} 
}

//---------------------------------------------------------

CPhysicModelGeneric* CPhysicEntity::createPhysicModelGeneric(const Map::CEntity *entityInfo)
{
	// Comprobar que estamos creando una entidad física genérica
	const std::string entity = entityInfo->getStringAttribute(STR_PHYSIC_ENTITY);
	assert(entity == STR_GENERIC);

	// Crear el modelo físico a partir de un fichero
	std::string file = entityInfo->getStringAttribute(STR_PHYSIC_FILE);
	return _physicServer->createModelGeneric(file);
}

//---------------------------------------------------------