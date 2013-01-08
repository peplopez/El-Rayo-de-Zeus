#include "PhysicObjGeneric.h"
#include "PhysicModelGeneric.h"
#include "PhysicScene.h"
#include "PhysicInstancer.h"
#include "PhysicUtils.h"

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicObjGeneric::CPhysicObjGeneric (CPhysicScene* pScene, const CPhysicModelGeneric* pPhysicModel)
{
    m_pScene = pScene;
    m_pPhysicModel = pPhysicModel;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicObjGeneric::~CPhysicObjGeneric (void)
{
    if ( IsActivated() )
    {
        Deactivate();
    }
}

//-----------------------------------------------------------------------------
//  InitObject
//-----------------------------------------------------------------------------
void    CPhysicObjGeneric::InitObject (const TInitInfo& initInfo)
{
    // Inicializacion de la clase padre: ----------------------------------->>>
    CPhysicObj::InitObject (initInfo);
    // ---------------------------------------------------------------------<<<

    // Inicializacion especifica:
    // - Comprobamos que nos han pasado un modelo generico
    assert (initInfo.pPhysicModel->GetType() == IPhysicModel::MODEL_GENERIC);
}

//-----------------------------------------------------------------------------
//  GetPhysicModel
//-----------------------------------------------------------------------------
const IPhysicModel* CPhysicObjGeneric::GetPhysicModel (void) const
{
    return m_pPhysicModel;
}

//-----------------------------------------------------------------------------
//  Activate
//-----------------------------------------------------------------------------
bool    CPhysicObjGeneric::Activate (void)
{
    // Instanciamos el modelo generico usando un instanciador generico    
    CPhysicInstancer    instancer;
    TInstancedData      instancedData;
    instancer.InstanceModel( m_pPhysicModel, m_pScene, m_InitInfo.vWorldPose, &instancedData );
    
    // Copiamos los datos instanciados a nuestros vectores
    m_aActors = instancedData.aInstancedActors;
    m_aJoints = instancedData.aInstancedJoints;

    // Configuramos todos los nuevos actores
    for ( unsigned i = 0; i < m_aActors.size(); ++i )
    {
        // Añadimos la informacion de usuario
        m_aActors[i]->userData = new TActorInfo (this, i);

        // Establecer el modo de comportamiento al actor de PhysX
        switch (m_InitInfo.ePhysicMode)
        {
            case    PMODE_STATIC:
			break;
            case    PMODE_KINEMATIC:
                m_aActors[i]->raiseBodyFlag (NX_BF_KINEMATIC);
            break;
            case    PMODE_DYNAMIC:
                m_aActors[i]->clearBodyFlag (NX_BF_KINEMATIC);
            break;
        }
    }

    // Activacion de la clase padre:
    return CPhysicObj::Activate ();
}

//-----------------------------------------------------------------------------
//  Deactivate
//-----------------------------------------------------------------------------
void    CPhysicObjGeneric::Deactivate (void)
{
    // Destruccion de los actores 
    for ( unsigned i = 0; i < m_aActors.size(); ++i )
    {
        // Destruir informacion de usuario
        delete m_aActors[i]->userData;
        // Destruimos el objeto de motor
        m_pScene->GetNxScene()->releaseActor( *(m_aActors[i]) );
    }

    // Destruccion de los joints
    for ( unsigned i = 0; i < m_aJoints.size(); ++i )
    {
        m_pScene->GetNxScene()->releaseJoint( *(m_aJoints[i]) );
    }
    
    // Desactivacion de la clase padre
    CPhysicObj::Deactivate ();
}

//-----------------------------------------------------------------------------
//  IsMoving
//-----------------------------------------------------------------------------
bool    CPhysicObjGeneric::IsMoving (void) const
{
    // TODO: Completar
    assert(0);

    return false;
}

//-----------------------------------------------------------------------------
//  GetNumParts
//-----------------------------------------------------------------------------
int     CPhysicObjGeneric::GetNumParts (void) const
{
    return m_aActors.size();
}

//-----------------------------------------------------------------------------
//  GetActor
//-----------------------------------------------------------------------------
NxActor*        CPhysicObjGeneric::GetActor (int nPartIdx)
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );
    return  m_aActors[ nPartIdx ];
}

//-----------------------------------------------------------------------------
//  GetActor
//-----------------------------------------------------------------------------
const NxActor*  CPhysicObjGeneric::GetActor (int nPartIdx) const
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );
    return  m_aActors[ nPartIdx ];
}

//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void    CPhysicObjGeneric::Update (float fDeltaTime)
{
    if ( ! IsActivated() )  return;

    // TODO:
    // - Actualizacion del objeto visual asociado
}

