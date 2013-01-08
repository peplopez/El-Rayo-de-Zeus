#include "PhysicUtils.h"
//#include "DrawShapes.h"

//-----------------------------------------------------------------------------
//  ApplyForceToActor
//-----------------------------------------------------------------------------
NxVec3  ApplyForceToActor (NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength, NxForceMode eForceMode)
{
    NxVec3 forceVec = forceStrength * forceDir;

	if (actor->isDynamic())
	{
		actor->addForce (forceVec, eForceMode);
	}

    return forceVec;
}

//-----------------------------------------------------------------------------
//  ApplyTorqueToActor
//-----------------------------------------------------------------------------
void	ApplyTorqueToActor (NxActor* actor, const NxVec3& torque, NxForceMode eForceMode)
{
	if (actor->isDynamic())
	{
		actor->addTorque (torque, eForceMode);
	}
}

//-----------------------------------------------------------------------------
//  DrawForce
//-----------------------------------------------------------------------------
/*
void    DrawForce (NxActor* actor, NxVec3& forceVec, const NxVec3& color)
{
    // draw only if the force is large enough
    NxReal force = forceVec.magnitude();
    if (force < 0.1f)  return;

    forceVec = 3*forceVec/force;

    NxVec3 pos = actor->getCMassGlobalPosition();
    DrawArrow(pos, pos + forceVec, color);
}
*/

//-----------------------------------------------------------------------------
//  CalcVolumeBox
//-----------------------------------------------------------------------------
float	PhysicUtils::CalcVolumeBox (NxVec3 a_vExtends)
{
    //NOTA: La extensión de una caja representa la mitad de la longitud en cada eje
    return (a_vExtends.x * 2) * (a_vExtends.y * 2) * (a_vExtends.z * 2);
}

//-----------------------------------------------------------------------------
//  CalcVolumeCapsule
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeCapsule (float a_fRadius, float a_fHeight)
{
    // Volumen de una capsula: Pi * r^2 * (4/3*r + h)
    return NxPiF32 * a_fRadius * a_fRadius * (4.f/3.f * a_fRadius + a_fHeight);
}

//-----------------------------------------------------------------------------
//  CalcVolumeSphere
//-----------------------------------------------------------------------------
float	PhysicUtils::CalcVolumeSphere (float a_fRadius)
{
    return (4.f/3.f) * NxPiF32 * a_fRadius * a_fRadius * a_fRadius;
}

//-----------------------------------------------------------------------------
//  CalcVolumeBox
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeBox (const NXU::NxBoxShapeDesc& box)
{
    return PhysicUtils::CalcVolumeBox( box.dimensions );
}

//-----------------------------------------------------------------------------
//  CalcVolumeSphere
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeSphere (const NXU::NxSphereShapeDesc& sphere)
{
    return PhysicUtils::CalcVolumeSphere( sphere.radius );
}

//-----------------------------------------------------------------------------
//  CalcVolumeCapsule
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeCapsule (const NXU::NxCapsuleShapeDesc& capsule)
{
    //@NOTA: La altura de las capsulas de PhysX se corresponde con la mitad de la 
    // extension total de la capsula
    return PhysicUtils::CalcVolumeCapsule( capsule.radius, 2 * capsule.height );
}

//-----------------------------------------------------------------------------
//  CalcVolumeShape
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeShape (const NXU::NxShapeDesc& shape)
{
    float fResult = 0.f;
    switch ( shape.mType )
    {
        case NXU::SC_NxBoxShapeDesc:      
            fResult = CalcVolumeBox ((const NXU::NxBoxShapeDesc&) shape);    
        break;
        case NXU::SC_NxSphereShapeDesc:   
            fResult = CalcVolumeSphere ((const NXU::NxSphereShapeDesc&) shape);  
        break;
        case NXU::SC_NxCapsuleShapeDesc:  
            fResult = CalcVolumeCapsule ((const NXU::NxCapsuleShapeDesc&) shape);
        break;
        default:
        {
            // NOTA: Caso no soportado
            assert(0);
        }
    }

    return fResult;
}

//-----------------------------------------------------------------------------
//  CalcVolumeBox
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeBox (const NxBoxShapeDesc& box)
{
    return PhysicUtils::CalcVolumeBox( box.dimensions );
}

//-----------------------------------------------------------------------------
//  CalcVolumeSphere
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeSphere (const NxSphereShapeDesc& sphere)
{
    return PhysicUtils::CalcVolumeSphere( sphere.radius );
}

//-----------------------------------------------------------------------------
//  CalcVolumeCapsule
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeCapsule (const NxCapsuleShapeDesc& capsule)
{
    //@NOTA: La altura de las capsulas de PhysX se corresponde con la mitad de la 
    // extension total de la capsula
    return PhysicUtils::CalcVolumeCapsule( capsule.radius, 2 * capsule.height );
}

//-----------------------------------------------------------------------------
//  CalcVolumeShape
//-----------------------------------------------------------------------------
float   PhysicUtils::CalcVolumeShape (const NxShapeDesc& shape)
{
    float fResult = 0.f;
    switch ( shape.getType() )
    {
        case NX_SHAPE_BOX:      
            fResult = CalcVolumeBox ((const NxBoxShapeDesc&) shape);    
        break;
        case NX_SHAPE_SPHERE:   
            fResult = CalcVolumeSphere ((const NxSphereShapeDesc&) shape);  
        break;
        case NX_SHAPE_CAPSULE:  
            fResult = CalcVolumeCapsule ((const NxCapsuleShapeDesc&) shape);
        break;
        default:
        {
            // NOTA: Caso no soportado
            assert(0);
        }
    }

    return fResult;
}

//-----------------------------------------------------------------------------
//  CalcVolumeShape
//-----------------------------------------------------------------------------
NxBox   PhysicUtils::CalcOBBox (const NxBoxShape&   shapeBox)
{
    NxBox   box;
    shapeBox.getWorldOBB( box );
    return  box;
}

//-----------------------------------------------------------------------------
//  CalcVolumeShape
//-----------------------------------------------------------------------------
NxBox   PhysicUtils::CalcOBBox (const NxSphereShape&    shapeSphere)
{
    return  NxBox( shapeSphere.getGlobalPosition(), NxVec3(shapeSphere.getRadius()), NxMat33(NX_IDENTITY_MATRIX) );
}

//-----------------------------------------------------------------------------
//  CalcVolumeShape
//-----------------------------------------------------------------------------
NxBox   PhysicUtils::CalcOBBox (const NxCapsuleShape&   shapeCapsule)
{
    NxCapsule   capsule;
    shapeCapsule.getWorldCapsule( capsule );
    NxBox       box;
    NxComputeBoxAroundCapsule( capsule, box );

    return  box;
}

//-----------------------------------------------------------------------------
//  CalcOBBox
//-----------------------------------------------------------------------------
NxBox   PhysicUtils::CalcOBBox (const NxShape& shape)
{
    NxBox   result;
    switch ( shape.getType() )
    {
        case NX_SHAPE_BOX:
            result  = CalcOBBox ((const NxBoxShape&) shape);    
        break;
        case NX_SHAPE_SPHERE:   
            result  = CalcOBBox ((const NxSphereShape&) shape);  
        break;
        case NX_SHAPE_CAPSULE:  
            result  = CalcOBBox ((const NxCapsuleShape&) shape);
        break;
        default:
        {
            // NOTA: Caso no soportado
            assert(0);
        }
    }

    return  result;
}



//-----------------------------------------------------------------------------
//  CalcExplosionImpact
//-----------------------------------------------------------------------------
NxVec3  PhysicUtils::CalcExplosionImpact (const NxBox& box, const NxVec3& vExplosionPos)
{
    // 1. Transformar la posicion de la explosion al espacio de la caja
    NxMat34     tBoxMatrix;
    NxVec3      vPosLocal;
    tBoxMatrix.M = box.GetRot();
    tBoxMatrix.t = box.GetCenter();
    tBoxMatrix.multiplyByInverseRT (vExplosionPos, vPosLocal);
    // 2. Ahora, para determiar el punto de impacto, basta con clampear las coordenadas al volumen de la BBox
    NxVec3      vBoxExtents = box.GetExtents();
    vPosLocal.x = NxMath::clamp (vPosLocal.x, vBoxExtents.x, -vBoxExtents.x);
    vPosLocal.y = NxMath::clamp (vPosLocal.y, vBoxExtents.y, -vBoxExtents.y);
    vPosLocal.z = NxMath::clamp (vPosLocal.z, vBoxExtents.z, -vBoxExtents.z);
    // 3. Transformamos el punto de impacto de nuevo a espacio mundo
    NxVec3      vImpactPos;
    tBoxMatrix.multiply (vPosLocal, vImpactPos);
    
    return  vImpactPos; 
}
