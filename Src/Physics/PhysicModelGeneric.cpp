#include "PhysicModelGeneric.h"
#include "PhysicUtils.h"

#include "Tools/NxuStream2/NXU_Helper.h"
#include "Tools/NxuStream2/NXU_Schema.h"
#include "Tools/NxuStream2/NXU_PhysicsExport.h"

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicModelGeneric::CPhysicModelGeneric (void)
{
    m_bInit = false;
    m_pCollection = NULL;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicModelGeneric::~CPhysicModelGeneric (void)
{
    End();    
}

//-----------------------------------------------------------------------------
//  Init
//-----------------------------------------------------------------------------
bool    CPhysicModelGeneric::Init (const TInitInfo& initInfo)
{
    bool    bOk = true;

    End();

    // Inicialiacion de dstos miembro
    if ( initInfo.pszModelFile != NULL )
        bOk = LoadModelFile( initInfo.pszModelFile );
    else
        bOk = CreateEmptyModel();

    m_bInit = true;
    if ( !bOk ) CPhysicModelGeneric::End();

    return  bOk;
}

//-----------------------------------------------------------------------------
//  Init
//-----------------------------------------------------------------------------
void    CPhysicModelGeneric::End()
{
    if (IsOk())
    {
        NXU::releaseCollection( m_pCollection );
        m_bInit = false;
    }
}

//-----------------------------------------------------------------------------
//  CreatePart
//-----------------------------------------------------------------------------
int     CPhysicModelGeneric::AddPart (const std::string& partName)
{
    NXU::NxActorDesc*   pNewActor = new NXU::NxActorDesc();
    pNewActor->name = NXU::getGlobalString( partName.c_str() );
    m_pCollection->mScenes.back()->mActors.pushBack( pNewActor );

    return m_pCollection->mScenes.back()->mActors.size() - 1;
}

//-----------------------------------------------------------------------------
//  DeletePart
//-----------------------------------------------------------------------------
void    CPhysicModelGeneric::DelPart (int nPartIdx)
{
    assert( nPartIdx < GetNumParts() );
    NxArray< NXU::NxActorDesc *>& aParts = m_pCollection->mScenes.back()->mActors;
    delete aParts[nPartIdx];
    // Movemos el ultimo elemento al hueco del eliminado
    aParts.replaceWithLast( nPartIdx );
}

//-----------------------------------------------------------------------------
//  GetNumParts
//-----------------------------------------------------------------------------
int     CPhysicModelGeneric::GetNumParts (void) const
{
    return (int)m_pCollection->mScenes.back()->mActors.size();
}

//-----------------------------------------------------------------------------
//  GetPartPose
//-----------------------------------------------------------------------------
const NxMat34&  CPhysicModelGeneric::GetPartPose (int nPartIdx) const
{
    assert( nPartIdx < GetNumParts() );
    return GetPart(nPartIdx)->globalPose;
}

//-----------------------------------------------------------------------------
//  EditPartPose
//-----------------------------------------------------------------------------
NxMat34&    CPhysicModelGeneric::EditPartPose (int nPartIdx)
{
    assert( nPartIdx < GetNumParts() );
    return EditPart(nPartIdx)->globalPose;
}

//-----------------------------------------------------------------------------
//  CreateShape
//-----------------------------------------------------------------------------
int     CPhysicModelGeneric::AddShape (int nPartIdx, NxShapeType shapeType)
{
    assert( nPartIdx < GetNumParts() );

    // Hacemos una copia de la descripcion en funcion del tipo
    NXU::NxShapeDesc* pNewShape = NULL;
    switch (shapeType)
    {
        case NX_SHAPE_BOX:      pNewShape = new NXU::NxBoxShapeDesc();          break;
        case NX_SHAPE_CAPSULE:  pNewShape = new NXU::NxCapsuleShapeDesc();      break;
        case NX_SHAPE_PLANE:    pNewShape = new NXU::NxPlaneShapeDesc();        break;
        case NX_SHAPE_SPHERE:   pNewShape = new NXU::NxSphereShapeDesc();       break;
        case NX_SHAPE_MESH:     pNewShape = new NXU::NxTriangleMeshShapeDesc(); break;
        default:                
            assert(0);  
        break;
    }

    EditPart(nPartIdx)->mShapes.pushBack (pNewShape);
    return (GetPart(nPartIdx)->mShapes.size() - 1);
}

//-----------------------------------------------------------------------------
//  DeleteShape
//-----------------------------------------------------------------------------
void    CPhysicModelGeneric::DelShape (int nPartIdx, int nShapeIdx)
{
    assert( nPartIdx < GetNumParts() );
    assert( nShapeIdx < (int)GetPart(nPartIdx)->mShapes.size() );

    NXU::NxActorDesc* pActorDesc = EditPart(nPartIdx);
    delete pActorDesc->mShapes[nShapeIdx];
    // Movemos la ultima shape al hueco libre
    pActorDesc->mShapes.replaceWithLast (nShapeIdx);
}

//-----------------------------------------------------------------------------
//  GetNumShapes
//-----------------------------------------------------------------------------
int     CPhysicModelGeneric::GetNumShapes (int nPartIdx) const
{
    assert( nPartIdx < GetNumParts() );
    return GetPart(nPartIdx)->mShapes.size();
}

//-----------------------------------------------------------------------------
//  GetShape
//-----------------------------------------------------------------------------
const NXU::NxShapeDesc* CPhysicModelGeneric::GetShape (int nPartIdx, int nShapeIdx) const
{
    assert( nPartIdx < GetNumParts() );
    assert( nShapeIdx < (int)GetPart(nPartIdx)->mShapes.size() );

    const NXU::NxActorDesc* pActorDesc = GetPart(nPartIdx);
    return pActorDesc->mShapes[nShapeIdx];
}

//-----------------------------------------------------------------------------
//  EditShape
//-----------------------------------------------------------------------------
NXU::NxShapeDesc* CPhysicModelGeneric::EditShape (int nPartIdx, int nShapeIdx)
{
    assert( nPartIdx < GetNumParts() );
    assert( nShapeIdx < (int)GetPart(nPartIdx)->mShapes.size() );

    NXU::NxActorDesc* pActorDesc = EditPart(nPartIdx);
    return pActorDesc->mShapes[nShapeIdx];
}

//-----------------------------------------------------------------------------
//  SetMass
//-----------------------------------------------------------------------------
void    CPhysicModelGeneric::SetMass (int nPartIdx, float fMass)
{
    assert( nPartIdx < GetNumParts() );
    NXU::NxActorDesc*   pActorDesc = EditPart(nPartIdx);
    pActorDesc->mBody.mass = fMass;
    // Al establecer directamente la masa, el valor de la densidad deja de ser valido
    pActorDesc->density = 0.f;
    // Tambien tenemos que invalidar los valores de masa/densidad de las shapes
    for ( uint i = 0; i < pActorDesc->mShapes.size(); ++i )
    {
        pActorDesc->mShapes[i]->mShapeMass      = fMass;
        pActorDesc->mShapes[i]->mShapeDensity   = 0.f;
    }
}

//-----------------------------------------------------------------------------
//  GetMass
//-----------------------------------------------------------------------------
float   CPhysicModelGeneric::GetMass (int nPartIdx) const
{
    assert( nPartIdx < GetNumParts() );
    return GetPart(nPartIdx)->mBody.mass;
}

//-----------------------------------------------------------------------------
//  SetTotalMass
//-----------------------------------------------------------------------------
void    CPhysicModelGeneric::SetTotalMass (float fMass)
{
    // NOTA: Distribuimos la masa total entre las partes, repartiendola de forma 
    //   proporcional al volumen 

    // Calculamos el volumen total actual
    float fCurTotalVol = CalcVolume();
    // Necesitamos tener volumen
    assert( fCurTotalVol > 0.f );

    // Distribuimos la masa total porcentualmente entre las partes
    int nNumParts = GetNumParts();
    for (int i = 0; i < nNumParts; i++)
    {
        SetMass (i, fMass * (CalcVolume(i) / fCurTotalVol));
    }
}

//-----------------------------------------------------------------------------
//  SetDensity
//-----------------------------------------------------------------------------
void    CPhysicModelGeneric::SetDensity (int nPartIdx, float fDensity)
{
    assert( nPartIdx < GetNumParts() );
    NXU::NxActorDesc*   pActorDesc = EditPart(nPartIdx);
    pActorDesc->density = fDensity;
    // NOTA: Al establecer directamente la densidad, el valor de la masa deja de ser valido
    pActorDesc->mBody.mass = 0.f;
    // Tambien tenemos que invalidar los valores de masa/densidad de las shapes
    for ( uint i = 0; i < pActorDesc->mShapes.size(); ++i )
    {
        pActorDesc->mShapes[i]->mShapeMass      = 0.f;
        pActorDesc->mShapes[i]->mShapeDensity   = fDensity;
    }
}

//-----------------------------------------------------------------------------
//  GetDensity
//-----------------------------------------------------------------------------
float   CPhysicModelGeneric::GetDensity (int nPartIdx) const
{
    assert( nPartIdx < GetNumParts() );
    return GetPart(nPartIdx)->density;
}

//-----------------------------------------------------------------------------
//  SetDensityAll
//-----------------------------------------------------------------------------
void    CPhysicModelGeneric::SetDensityAll (float fDensity)
{
    for (int i = 0; i < GetNumParts(); i++)
    {
        SetDensity (i, fDensity);
    }
}

//-----------------------------------------------------------------------------
//  CalcVolume
//-----------------------------------------------------------------------------
float   CPhysicModelGeneric::CalcVolume (int nPartIdx) const
{
    assert( nPartIdx < GetNumParts() );

    // Sumamos el volumen de todas las shapes
    const NXU::NxActorDesc* pActorDesc = GetPart(nPartIdx);
    // NOTA: Si las shapes estan solapadas el resultado no sera preciso pero 
    //   valdrá como aproximacion
    float fVolumeAcum = 0.f;
    for (int i = 0; i < GetNumShapes(nPartIdx); i++)
    {
        fVolumeAcum += PhysicUtils::CalcVolumeShape (*(pActorDesc->mShapes[i]));
    }
    return fVolumeAcum;
}

//-----------------------------------------------------------------------------
//  CalcVolume
//-----------------------------------------------------------------------------
float   CPhysicModelGeneric::CalcVolume (void) const
{
    //  Sumamos es volumen de todas las partes
    float fVolumeAcum = 0.f;
    for (int i = 0; i < GetNumParts(); i++)
    {
        fVolumeAcum += CalcVolume (i);
    }

    return fVolumeAcum;
}

//-----------------------------------------------------------------------------
//  AddJoint
//-----------------------------------------------------------------------------
int     CPhysicModelGeneric::AddJoint (NxJointType eType)
{
    // Creamos un nuevo joint
    NXU::NxJointDesc* pNewJoint = NULL;
    // - Creamos la descripcion en funcion del tipo de joint
    switch (eType)
    {
        case NX_JOINT_PRISMATIC:        pNewJoint = new NXU::NxPrismaticJointDesc();        break;
        case NX_JOINT_REVOLUTE:         pNewJoint = new NXU::NxRevoluteJointDesc();         break;
        case NX_JOINT_CYLINDRICAL:      pNewJoint = new NXU::NxCylindricalJointDesc();      break;
        case NX_JOINT_SPHERICAL:        pNewJoint = new NXU::NxSphericalJointDesc();        break;
        case NX_JOINT_POINT_ON_LINE:    pNewJoint = new NXU::NxPointOnLineJointDesc();      break;
        case NX_JOINT_POINT_IN_PLANE:   pNewJoint = new NXU::NxPointInPlaneJointDesc();     break;
        case NX_JOINT_DISTANCE:         pNewJoint = new NXU::NxDistanceJointDesc();         break;
        case NX_JOINT_PULLEY:           pNewJoint = new NXU::NxPulleyJointDesc();           break;
        case NX_JOINT_FIXED:            pNewJoint = new NXU::NxFixedJointDesc();            break;
        case NX_JOINT_D6:               pNewJoint = new NXU::NxD6JointDesc();               break;
        default:
        {
            assert(0);
        }
    }

    // Lo añadimos a nuestro vector de joints
    m_pCollection->mScenes[0]->mJoints.push_back (pNewJoint);

    return (m_pCollection->mScenes[0]->mJoints.size() - 1);
}

//-----------------------------------------------------------------------------
//  DelJoint
//-----------------------------------------------------------------------------
void    CPhysicModelGeneric::DelJoint (int nJointIdx)
{
    assert( nJointIdx < GetNumJoints() );
    NxArray< NXU::NxJointDesc *>& aJoints = m_pCollection->mScenes[0]->mJoints;
    delete aJoints[ nJointIdx ];
    // Movemos el ultimo elemento al hueco del eliminado
    aJoints.replaceWithLast( nJointIdx );
}

//-----------------------------------------------------------------------------
//  GetNumJoints
//-----------------------------------------------------------------------------
int     CPhysicModelGeneric::GetNumJoints (void) const
{
    return m_pCollection->mScenes[0]->mJoints.size();
}

//-----------------------------------------------------------------------------
//  GetJoint
//-----------------------------------------------------------------------------
const NXU::NxJointDesc* CPhysicModelGeneric::GetJoint (int nJointIdx) const
{
    assert( nJointIdx < GetNumJoints() );
    return m_pCollection->mScenes[0]->mJoints[nJointIdx];
}

//-----------------------------------------------------------------------------
//  EditJoint
//-----------------------------------------------------------------------------
NXU::NxJointDesc*   CPhysicModelGeneric::EditJoint (int nJointIdx)
{
    assert( nJointIdx < GetNumJoints() );
    return m_pCollection->mScenes[0]->mJoints[nJointIdx];
}

//-----------------------------------------------------------------------------
//  GetPart
//-----------------------------------------------------------------------------
const NXU::NxActorDesc* CPhysicModelGeneric::GetPart (int nPartIdx) const
{
    assert( nPartIdx < GetNumParts() );
    return m_pCollection->mScenes[0]->mActors[nPartIdx];
}

//-----------------------------------------------------------------------------
//  EditPart
//-----------------------------------------------------------------------------
NXU::NxActorDesc*   CPhysicModelGeneric::EditPart (int nPartIdx) 
{
    assert( nPartIdx < GetNumParts() );
    return m_pCollection->mScenes[0]->mActors[nPartIdx];
}

//-----------------------------------------------------------------------------
//  CalcBBox
//-----------------------------------------------------------------------------
NxBox   CPhysicModelGeneric::CalcBBox (void) const
{
    // TODO: Completar...
    NxBox   result;
    return  result;
}

//-----------------------------------------------------------------------------
//  LoadModelFile
//-----------------------------------------------------------------------------
bool    CPhysicModelGeneric::LoadModelFile (const char* pszFilename)
{
    bool    bOk = true;

    m_pCollection = NXU::loadCollection( pszFilename, NXU::FT_XML );
    bOk = (m_pCollection != NULL);

    return	bOk;
}

//-----------------------------------------------------------------------------
//  CreateEmptyModel
//-----------------------------------------------------------------------------
bool    CPhysicModelGeneric::CreateEmptyModel ()
{
    bool    bOk = true;

    // Por defecto tenemos que añadir una escena
    m_pCollection = NXU::createCollection( "Default", NULL );

    NxSceneDesc             desc;
    NXU::NxuPhysicsExport   exporter ( m_pCollection );
    bOk = exporter.Write( &desc, NULL );

    return  bOk;
}
