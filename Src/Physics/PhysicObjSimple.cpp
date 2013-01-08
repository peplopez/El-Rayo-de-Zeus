#include "PhysicObjSimple.h"
#include "PhysicModelSimple.h"
#include "PhysicScene.h"
#include "PhysicUtils.h"

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicObjSimple::CPhysicObjSimple (CPhysicScene* pScene, const CPhysicModelSimple* pPhysicModel)
{
    m_pScene = pScene;
    m_pPhysicModel = pPhysicModel;
    m_pActor = NULL;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicObjSimple::~CPhysicObjSimple (void)
{
    if ( IsActivated() )
    {
        Deactivate();
    }
}

//-----------------------------------------------------------------------------
//  InitObject
//-----------------------------------------------------------------------------
void    CPhysicObjSimple::InitObject (const TInitInfo& initInfo)
{
    // Inicializacion de la clase padre: ----------------------------------->>>
    CPhysicObj::InitObject (initInfo);
    // ---------------------------------------------------------------------<<<

    // Inicializacion especifica:
    // - Comprobamos que nos han pasado un modelo simple
    assert (initInfo.pPhysicModel->GetType() == IPhysicModel::MODEL_SIMPLE);
}

//-----------------------------------------------------------------------------
//  GetPhysicModel
//-----------------------------------------------------------------------------
const IPhysicModel* CPhysicObjSimple::GetPhysicModel (void) const
{
    return m_pPhysicModel;
}

//-----------------------------------------------------------------------------
//  Activate
//-----------------------------------------------------------------------------
bool    CPhysicObjSimple::Activate (void)
{
    assert( !IsActivated() );

    // Creacion del actor de PhysX a partir de la descripcion del modelo
    CPhysicModelSimple* pModel = (CPhysicModelSimple*) m_InitInfo.pPhysicModel;

    // Preparar la descripción del actor
	NxActorDesc actorDesc = pModel->GetNxActorDesc();
	actorDesc.globalPose = m_InitInfo.vWorldPose;
    if ( m_InitInfo.ePhysicMode == PMODE_STATIC )
    {
        actorDesc.body = NULL;
    }

    m_pActor = m_pScene->GetNxScene()->createActor (actorDesc);

    // Añadimos la informacion de usuario
    m_pActor->userData = new TActorInfo (this, 0);

    // Establecer el flag de actor cinematico
    if ( m_InitInfo.ePhysicMode == PMODE_KINEMATIC )
        m_pActor->raiseBodyFlag (NX_BF_KINEMATIC);
    if ( m_InitInfo.ePhysicMode == PMODE_DYNAMIC )
        m_pActor->clearBodyFlag (NX_BF_KINEMATIC);

    // Activacion de la clase padre:
    return CPhysicObj::Activate ();
}

//-----------------------------------------------------------------------------
//  Deactivate
//-----------------------------------------------------------------------------
void    CPhysicObjSimple::Deactivate (void)
{
    assert( IsActivated() );
    
    // Destruir la informacion de usuario
    TActorInfo* pActorInfo = (TActorInfo*) m_pActor->userData;
    delete pActorInfo;

    // Destruccion de los actores y joints
    m_pScene->GetNxScene()->releaseActor (*m_pActor);

    // Desactivacion de la clase padre
    CPhysicObj::Deactivate ();
}

//-----------------------------------------------------------------------------
//  IsMoving
//-----------------------------------------------------------------------------
bool    CPhysicObjSimple::IsMoving (void) const
{
    assert( m_bActivated );

    return !m_pActor->isSleeping();
}

//-----------------------------------------------------------------------------
//  GetActor
//-----------------------------------------------------------------------------
NxActor*        CPhysicObjSimple::GetActor (int nPartIdx)
{
    assert( m_bActivated && (nPartIdx == 0) );
    return  m_pActor;
}

//-----------------------------------------------------------------------------
//  GetActor
//-----------------------------------------------------------------------------
const NxActor*  CPhysicObjSimple::GetActor (int nPartIdx) const
{
    assert( m_bActivated && (nPartIdx == 0) );
    return  m_pActor;
}

//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void    CPhysicObjSimple::Update (float fDeltaTime)
{
    if ( ! IsActivated() )  return;

    // TODO:
    // - Actualizacion del objeto visual asociado
}
