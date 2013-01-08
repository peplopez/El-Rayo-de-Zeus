//!!

//*****************************************************************************
// Creacion  : 03/11/2010
// Contenido : Interfaz para el gestor de colisiones
//
//*****************************************************************************
#ifndef _IPHYSIC_COLLISION_MNG_H_
#define _IPHYSIC_COLLISION_MNG_H_

#include "PhysicUtils.h"

#include <NxController.h>
#include <NxUserContactReport.h>

class IPhysicObj;
class CPhysicObjCharacter;

// Información de colisión entre un character y un objeto físico cualquiera 
class CharacterObjHit 
{
public:
	CPhysicObjCharacter*	pCharacter;		// Controlador
	TActorInfo*				pObjInfo;		// Objeto tocado
	NxExtendedVec3			worldPos;		// Posición de contacto en coordenadas del mundo
	NxVec3					worldNormal;	// Normal al contacto en coordenadas del mundo
	NxVec3					dir;			// Dirección del movimiento
	NxF32					length;			// Longitud del movimiento

	CharacterObjHit(const NxControllerShapeHit &hit) : worldPos(hit.worldPos),
		worldNormal(hit.worldNormal), dir(hit.dir), length(hit.length)
	{
		TActorInfo *actorInfo = (TActorInfo *) hit.controller->getActor()->userData;
		pCharacter = (CPhysicObjCharacter*) actorInfo->pPhysicObj;
		pObjInfo = (TActorInfo *) hit.shape->getActor().userData;
	}
};

// Información de colisión entre dos characters 
class CharactersHit 
{
public:
	CPhysicObjCharacter*	character;		// Controlador actual
	CPhysicObjCharacter*	other;			// Controlador tocado

	CharactersHit(const NxControllersHit &hit)
	{
		TActorInfo *actorInfo = (TActorInfo *) hit.controller->getActor()->userData;
		character =  (CPhysicObjCharacter*) actorInfo->pPhysicObj;
		actorInfo = (TActorInfo *) hit.other->getActor()->userData;
		other = (CPhysicObjCharacter*) actorInfo->pPhysicObj;
	}
};

// Información de contacto entre dos objetos físicos
class ContactPair
{
public:
	TActorInfo*				objsInfo[2];		// Objetos en contacto
	NxConstContactStream	stream;				// Ver NxContactStreamIterator
	NxVec3					sumNormalForce;		// Fuerza normal total
	NxVec3					sumFrictionForce;	// Fuerza tangencial total
	bool					isDeletedActor[2];  // Indica si el actor correspondiente ha sido borrado

	ContactPair(const NxContactPair &pair) : stream(pair.stream), 
		sumNormalForce(pair.sumNormalForce), sumFrictionForce(pair.sumFrictionForce)
	{
		for (int i=0; i<2; i++) {
			objsInfo[i] = (TActorInfo*) pair.actors[i]->userData;
			isDeletedActor[i] = pair.isDeletedActor[i];
		}
	}
};

// Interfaz para recibir notificaciones de colisiones
class IPhysicCollisionMng 
{
public:

	// Choque entre un character y un objeto
	virtual NxControllerAction  onCharacterObjHit (const CharacterObjHit &hit) = 0;
   
	// Choque entre dos characters 
	virtual NxControllerAction  onCharactersHit (const CharactersHit &hit) = 0; 

	// Interacción con un trigger 
	virtual void  onTrigger (const TActorInfo &triggerInfo, const TActorInfo &otherInfo, NxTriggerFlag status) = 0;
  
	// Dos objetos entran en contacto
	virtual void  onContactNotify (ContactPair &pair, NxU32 events) = 0; 
};

#endif
