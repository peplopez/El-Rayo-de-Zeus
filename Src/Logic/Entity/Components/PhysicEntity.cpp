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


namespace Logic {

	IMP_FACTORY(CPhysicEntity);

	//---------------------------------------------------------

	void  CPhysicEntity::onTrigger (Physics::IObserver* other, bool enter) 
	{
		// Construimos un mensaje de tipo TOUCHED o UNTOUCHED 
		// y lo enviamos a todos los componentes de la entidad.

		CMessageUInt* txMsg = new CMessageUInt();		
			txMsg->setType( Message::TRIGGER ); 	
			txMsg->setAction( 
				enter ? 
				Message::TRIGGER_ENTER : 
				Message::TRIGGER_EXIT
			);			
			txMsg->setUInt( static_cast<CPhysic*>(other)->getEntity()->getEntityID() );
		_entity->emitMessage(txMsg);

		// FRS: Adjuntamos el entityID para aquellos componentes que necesitan
		// conocer la entidad que ha entrado en este trigger para causarle
		// su efecto correspondiente (p.e enviarle un LIFE_MODIFIER desde CLifeModifier)
		// EntityID mejor que CEntity* para evitar que el receptor del mensaje 
		// se encuentre con un puntero a una entidad borrada (muerta)

	} // onTrigger

	
	
	//---------------------------------------------------------

	//Physics::CActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
	//{
	//	// Leer el tipo de entidad		
	//	assert( entityInfo->hasAttribute("physicType") );
	//		const std::string physicType= entityInfo->getStringAttribute("physicType");
	//		assert(physicType == "rigid");

	//	// Crear el tipo de entidad adecuada
	//	if(physicType == "rigid")
	//		return createRigid(entityInfo);
	//	//else if (physicType == "plane")  // TODO FRS Descomentar si queremos crear actores de tipo plano
	//	//	return createPlane(entityInfo);	// Esta funcion se implementa en la práctica de física
	//	else
	//		return 0;
	//}	
	
	// TODO Distinguir entre static, dynamic y kinematic
	// De momento, CPhysicEntity == rigid + static + square + trigger
	//Physics::CActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo)
	//{
	//	// Leer la posición de la entidad
	//	const TLogicalPosition logicPos = _entity->getLogicalPosition();
	//
	//	// RIGID TYPE: Leer el tipo de entidad: estáticos, dinámico o cinemático
	//	assert(entityInfo->hasAttribute("physicRigidType"));
	//		const std::string physicRigidType = entityInfo->getStringAttribute("physicRigidType");
	//		assert( (physicRigidType == "static") );//|| (physicRigidType == "dynamic") || (physicRigidType == "kinematic"));

	//	// RIGID SHAPE: Leer la forma (shape)
	//	assert(entityInfo->hasAttribute("physicRigidShape"));
	//		const std::string physicRigidShape= entityInfo->getStringAttribute("physicRigidShape");
	//		assert(physicRigidShape== "square");

	//	// TRIGGER: Leer si es un trigger (por defecto no)
	//	bool isTrigger = false;
	//	if (entityInfo->hasAttribute("physicTrigger"))
	//		isTrigger = entityInfo->getBoolAttribute("physicTrigger");

	//	// CREATE STATIC
	//	if (physicRigidType == "static") 
	//		if (physicRigidShape== "square") {
	//						
	//			// Leer el ancho del angular box
	//			assert(entityInfo->hasAttribute("physicWidth")); 
	//				float physicWidth = entityInfo->getFloatAttribute("physicWidth");

	//			// Leer la altura del angular box
	//			assert(entityInfo->hasAttribute("physicHeight"));
	//				float physicHeight = entityInfo->getFloatAttribute("physicHeight");

	//			// Crear un área cuadrado estática
	//			return _server->createActor(logicPos, physicWidth, physicHeight, isTrigger, this);
	//		}

	//	return 0;

	//} // createRigid


}
