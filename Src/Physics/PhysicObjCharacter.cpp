#include "PhysicObjCharacter.h"
#include "PhysicModelCharacter.h"
#include "PhysicEngine.h"
#include "PhysicUtils.h"

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicObjCharacter::CPhysicObjCharacter (CPhysicScene* pScene, const CPhysicModelCharacter* pPhysicModel)
{
    m_pScene = pScene;
    m_pPhysicModel = pPhysicModel;
    m_pNxController = NULL;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicObjCharacter::~CPhysicObjCharacter (void)
{
    if ( IsActivated() )
    {
        Deactivate();
    }
}

//-----------------------------------------------------------------------------
//  InitObject
//-----------------------------------------------------------------------------
void    CPhysicObjCharacter::InitObject (const TInitInfo& initInfo)
{
    // Inicializacion de la clase padre: ----------------------------------->>>
    CPhysicObj::InitObject (initInfo);
    // ---------------------------------------------------------------------<<<

    // Inicializacion especifica:
    // - Comprobamos que nos han pasado un modelo Character
    assert (initInfo.pPhysicModel->GetType() == IPhysicModel::MODEL_CHARACTER);
}

//-----------------------------------------------------------------------------
//  GetPhysicModel
//-----------------------------------------------------------------------------
const IPhysicModel* CPhysicObjCharacter::GetPhysicModel (void) const
{
    return m_pPhysicModel;
}

//-----------------------------------------------------------------------------
//  Activate
//-----------------------------------------------------------------------------
bool    CPhysicObjCharacter::Activate (void)
{
    assert( !IsActivated() );

    // Creacion del actor de PhysX a partir de la descripcion del modelo
    CPhysicModelCharacter*  pModel = (CPhysicModelCharacter*) m_InitInfo.pPhysicModel;

	//!!
	((NxControllerDesc &) pModel->GetNxControllerDesc()).callback = m_pScene;

    NxControllerManager*    pManager = CPhysicEngine::Ref().GetNxControllerMgr();
    m_pNxController = pManager->createController (m_pScene->GetNxScene(), 
                                                  pModel->GetNxControllerDesc());

    // Posicionamos el objeto en la pose de inicializacion
    NxExtendedVec3  extPos( m_InitInfo.vWorldPose.t.x, 
                            m_InitInfo.vWorldPose.t.y, 
                            m_InitInfo.vWorldPose.t.z );
    m_pNxController->setPosition (extPos);

    // Añadimos la informacion de usuario
    m_pNxController->getActor()->userData = new TActorInfo (this, 0);

    // Activacion de la clase padre:
    return CPhysicObj::Activate ();
}

//-----------------------------------------------------------------------------
//  Deactivate
//-----------------------------------------------------------------------------
void    CPhysicObjCharacter::Deactivate (void)
{
    assert( IsActivated() );

    // Destruir la informacion de usuario
    TActorInfo* pActorInfo = (TActorInfo*) m_pNxController->getActor()->userData;
    delete pActorInfo;

    // Destruccion del controller
    CPhysicEngine::Ref().GetNxControllerMgr()->releaseController (*m_pNxController);

    // Desactivacion de la clase padre
    CPhysicObj::Deactivate ();
}

//-----------------------------------------------------------------------------
//  IsMoving
//-----------------------------------------------------------------------------
bool    CPhysicObjCharacter::IsMoving (void) const
{
    assert( m_bActivated );

    // TODO: Completar 
    return false;
}

//-----------------------------------------------------------------------------
//  GetActor
//-----------------------------------------------------------------------------
NxActor*        CPhysicObjCharacter::GetActor (int nPartIdx)
{
    assert( m_bActivated && (nPartIdx == 0) );
    return  m_pNxController->getActor();
}

//-----------------------------------------------------------------------------
//  GetActor
//-----------------------------------------------------------------------------
const NxActor*  CPhysicObjCharacter::GetActor (int nPartIdx) const
{
    assert( m_bActivated && (nPartIdx == 0) );
    return  m_pNxController->getActor();
}

//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void    CPhysicObjCharacter::Update (float fDeltaTime)
{
    if ( ! IsActivated() )  return;

    // TODO:
    // - Actualizacion del objeto visual asociado
}

//-----------------------------------------------------------------------------
//  GetController
//-----------------------------------------------------------------------------
//!!
NxController* CPhysicObjCharacter::GetController ()
{
	return m_pNxController;
}

//-----------------------------------------------------------------------------
//  GetController
//-----------------------------------------------------------------------------
//!!
const NxController* CPhysicObjCharacter::GetController () const
{
	return m_pNxController;
}

//-----------------------------------------------------------------------------
//  Move
//-----------------------------------------------------------------------------
//!!
NxU32 CPhysicObjCharacter::Move	(const NxVec3 &vDisp, NxU32 collisionGroups, NxF32 minDist, 
								 NxF32 sharpness)
{
	assert( m_bActivated );	

	NxU32 collisionFlags;
	m_pNxController->move(vDisp, collisionGroups, minDist, collisionFlags, sharpness);
	return collisionFlags;
}

//-----------------------------------------------------------------------------
//  SetWoldPosition
//-----------------------------------------------------------------------------
//!!
void CPhysicObjCharacter::SetWorldPosition (const NxVec3 &vPos)
{
	assert( m_bActivated );

	m_pNxController->setPosition(NxExtendedVec3(vPos.x, vPos.y, vPos.z));
}

//-----------------------------------------------------------------------------
//  SetWorldOrientation
//-----------------------------------------------------------------------------
//!!
void CPhysicObjCharacter::SetWorldOrientation (const NxMat33 &mOrient)
{
	// Los character controllers no tienen orientación
	assert(0);
}

//-----------------------------------------------------------------------------
//  GetWorldPosition
//-----------------------------------------------------------------------------
NxVec3 CPhysicObjCharacter::GetWorldPosition (void) const
{
	NxExtendedVec3 pos = m_pNxController->getPosition();
	return NxVec3((NxReal) pos.x, (NxReal) pos.y, (NxReal) pos.z);
}

//-----------------------------------------------------------------------------
//  GetWorldPosition
//-----------------------------------------------------------------------------
NxVec3 CPhysicObjCharacter::GetWorldPosition (int nPartIdx) const
{
	assert(nPartIdx == 0);

	return GetWorldPosition();
}