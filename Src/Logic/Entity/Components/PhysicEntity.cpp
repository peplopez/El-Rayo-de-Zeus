/**
@file PhysicEntity.h

Contiene la implementación del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/MessageUInt.h"

#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/Actor.h"

//#define _USE_MATH_DEFINES
//#include <math.h>


namespace Logic {

	IMP_FACTORY(CPhysicEntity);

	//---------------------------------------------------------

	CPhysicEntity::CPhysicEntity() : IComponent(), IObserver(), _physicActor(0), _movement()
	{
		_server = Physics::CServer::getSingletonPtr();
	}

	//---------------------------------------------------------

	CPhysicEntity::~CPhysicEntity() 
	{
		if (_physicActor) {
			_physicActor->release();
			_physicActor = 0;
		}
		_server = 0;
	} 

	////---------------------------------------------------------

	bool CPhysicEntity::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		// Invocar al método de la clase padre
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_physicActor = createActor(entityInfo); // Crear el actor asociado al componente

		return true;
	} 

	//---------------------------------------------------------

	bool CPhysicEntity::accept(const CMessage *message)
	{
		return message->getType() == Message::KINEMATIC_MOVE;
	}

	//---------------------------------------------------------

	void CPhysicEntity::process(CMessage *message)
	{
		// TODO Implementar cuando tengamos kinematics
		// Acumulamos el vector de desplazamiento para usarlo posteriormente en  el método tick.
		//_movement += message._vector3;

	}

	//---------------------------------------------------------
	
	// TODO Impl. cuando tengamos dinamicas o cinématicas
	// De momento sólo tenemos estáticas
	//void CPhysicEntity::tick(unsigned int msecs) 
	//{
	//	// Invocar al método de la clase padre (IMPORTANTE)
	//	IComponent::tick(msecs);

	//	// Si es una entidad estática no hacemos nada
	//	PxRigidDynamic *dinActor = _actor->isRigidDynamic();
	//	if (!dinActor) 
	//		return; /

	//	// Actualizar la posición y la orientación de la entidad lógica usando la 
	//	// información proporcionada por el motor de física	
	//	_entity->setTransform(_server->getActorTransform(dinActor));

	//	// Si el objeto físico es cinemático intentamos moverlo de acuerdo 
	//	// a los mensajes KINEMATIC_MOVE recibidos 
	//	if (_server->isKinematic(dinActor)) {
	//		_server->moveKinematicActor(dinActor, _movement);
	//		_movement = Vector3::ZERO;
	//	} 
	//}

	//---------------------------------------------------------

	// TODO FRS Descomentar si queremos crear actores de tipo plano
	// Habría que recoger el physicRigidShapey distinguir entre "rigid" y "plane"
	// (recomendado assert en lugar de opciones por defecto para detectar errores en el map.txt)
	Physics::CActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
	{
		// Leer el tipo de entidad		
		assert( entityInfo->hasAttribute("physicType") );
			const std::string physicType= entityInfo->getStringAttribute("physicType");
			assert(physicType == "rigid");

		// Crear el tipo de entidad adecuada
		if(physicType == "rigid")
			return createRigid(entityInfo);
		//else if (physicType == "plane") 
		//	return createPlane(entityInfo);		
		else
			return 0;
	}	

	// TODO implementar si necesitamos planos
	//Physics::CActor* CPhysicEntity::createPlane(const Map::CEntity *entityInfo)
	//{
	//	// La posición de la entidad es un punto del plano
	//	const Vector3 point = _entity->getPosition();
	//
	//	// Leer el vector normal al plano
	//	assert(entityInfo->hasAttribute("physic_normal"));
	//	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");

	//	// Leer el grupo de colisión (por defecto grupo 0)
	//	int group = 0;
	//	if (entityInfo->hasAttribute("physic_group"))
	//		group = entityInfo->getIntAttribute("physic_group");
 //
	//	// Crear el plano
	//	return _server->createPlane(point, normal, group, this);
	//}


	// TODO Distinguir entre static, dynamic y kinematic
	// De momento, CPhysicEntity == static + trigger
	Physics::CActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo)
	{
		// Leer la posición de la entidad
		const TLogicalPosition logicPos = _entity->getLogicalPosition();
	
		// RIGID TYPE: Leer el tipo de entidad: estáticos, dinámico o cinemático
		assert(entityInfo->hasAttribute("physicRigidType"));
			const std::string physicRigidType = entityInfo->getStringAttribute("physicRigidType");
			assert( (physicRigidType == "static") );//|| (physicRigidType == "dynamic") || (physicRigidType == "kinematic"));

		// RIGID SHAPE: Leer la forma (shape)
		assert(entityInfo->hasAttribute("physicRigidShape"));
			const std::string physicRigidShape= entityInfo->getStringAttribute("physicRigidShape");
			assert(physicRigidShape== "square");

		// TRIGGER: Leer si es un trigger (por defecto no)
		bool isTrigger = false;
		if (entityInfo->hasAttribute("physicTrigger"))
			isTrigger = entityInfo->getBoolAttribute("physicTrigger");

		// TODO Impl. si usamoso grupos en un futuro
		// Leer el grupo de colisión (por defecto 0)
		//int group = 0;
		//if (entityInfo->hasAttribute("physicGroup"))
		//	group = entityInfo->getIntAttribute("physicGroup");

		// CREATE STATIC
		if (physicRigidType == "static") {
			if (physicRigidShape== "square") {
							
				// Leer el ancho del angular box
				assert(entityInfo->hasAttribute("physicWidth")); 
					float physicWidth = entityInfo->getFloatAttribute("physicWidth");

				// Leer la altura del angular box
				assert(entityInfo->hasAttribute("physicHeight"));
					float physicHeight = entityInfo->getFloatAttribute("physicHeight");

				// Crear un área cuadrado estática
				return _server->createActor(logicPos, physicWidth, physicHeight, isTrigger, this);
			}
		
		// TODO DYNAMIC / KINEMATIC
		//} else {
		//	// Leer la masa (por defecto 0)
		//	float mass = 0;
		//	if (entityInfo->hasAttribute("physic_mass"))
		//		mass = entityInfo->getFloatAttribute("physic_mass");
		//
		//	// Leer si se trata de un actor cinemático
		//	bool kinematic = (physicRigidType == "kinematic");

		//	if (physicRigidShape== "box") {
		//		// Leer las dimensiones de la caja
		//		assert(entityInfo->hasAttribute("physic_dimensions"));
		//		const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
		//	
		//		// Crear una caja dinámica
		//		return _server->createDynamicBox(position, physicDimensions, mass, kinematic, trigger, group, this);
		//	}


		} else
			return 0;
	}

	//---------------------------------------------------------

	void  CPhysicEntity::onTrigger (Physics::IObserver* other, bool enter) 
	{
		// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
		// todos los componentes de la entidad. 
		CMessageUInt* txMsg = new CMessageUInt();		
			txMsg->setType( Message::TMessageType::TRIGGER ); 	
			txMsg->setAction( 
				enter ? 
				Message::TActionType::TRIGGER_ENTER : 
				Message::TActionType::TRIGGER_EXIT
			);
			// HACK FRS para hacer dynamic_cast mejor sigo guardando el TEntityID en el IObserver!!
			txMsg->setUInt( dynamic_cast<IComponent*>(other)->getEntity()->getEntityID() );
		_entity->emitMessage(txMsg);

		// FRS: Adjuntamos el entityID para aquellos componentes que necesitan
		// conocer la entidad que ha entrado en este trigger para causarle
		// su efecto correspondiente (p.e CLifeModifier)
		// EntityID mejor que CEntity* para evitar que el receptor del mensaje 
		// se encuentre con un puntero a una entidad borrada (muerta)

	} // onTrigger

	



}
