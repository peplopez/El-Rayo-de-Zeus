//*****************************************************************************
// Creacion  : 10/05/2005
// Contenido : Fichero con utilidades fisicas varias
//
//*****************************************************************************
#ifndef _PHYSIC_UTILS_H_
#define _PHYSIC_UTILS_H_

#include "NxPhysics.h"
#include "IPhysicObj.h"
#include "Tools/NxuStream2/NXU_Schema.h"

// Aplica una fuerza un actor
NxVec3      ApplyForceToActor   (NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength, NxForceMode eForceMode);

// Dibuja un vector de fuerza
//void        DrawForce           (NxActor* actor, NxVec3& forceVec, const NxVec3& color);

// Informacion de usuario para actores
struct      TActorInfo
{
    IPhysicObj* pPhysicObj;
    int         nPartIdx;
    TActorInfo (IPhysicObj* a_pPhysicObj, int a_nPartIdx)
    {
        pPhysicObj = a_pPhysicObj;
        nPartIdx = a_nPartIdx;
    }
};

namespace   PhysicUtils
{
    // Calculo de volumenes:
    // - Volumen de una caja
    float   CalcVolumeBox       (const NXU::NxBoxShapeDesc& box);
    float   CalcVolumeBox       (const NxBoxShapeDesc& box);
	float	CalcVolumeBox		(NxVec3 a_vExtends);
    // - Volumen de una esfera
    float   CalcVolumeSphere    (const NXU::NxSphereShapeDesc& sphere);
    float   CalcVolumeSphere    (const NxSphereShapeDesc& sphere);
	float	CalcVolumeSphere	(float a_fRadius);
    // - Volumen de una capsula
    float   CalcVolumeCapsule   (const NXU::NxCapsuleShapeDesc& capsule);
    float   CalcVolumeCapsule   (const NxCapsuleShapeDesc& capsule);
	float	CalcVolumeCapsule	(float a_fRadius, float a_fHeight);
    // - Volumen de cualquier shape
    float   CalcVolumeShape     (const NXU::NxShapeDesc& shape);
    float   CalcVolumeShape     (const NxShapeDesc& shape);

    // Calculo de bounding box (en coord mundo) de una shape:
    NxBox   CalcOBBox           (const NxShape&         shape);
    NxBox   CalcOBBox           (const NxBoxShape&      shapeBox);
    NxBox   CalcOBBox           (const NxSphereShape&   shapeSphere);
    NxBox   CalcOBBox           (const NxCapsuleShape&  shapeCapsule);

    // Calcula el punto de impacto de una explosion en una caja
    NxVec3  CalcExplosionImpact (const NxBox& box, const NxVec3& vExposionPos);

};

#endif // _PHYSIC_UTILS_H_