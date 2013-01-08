#include "PhysicInstancer.h"
#include "PhysicModelGeneric.h"
#include "PhysicEngine.h"
#include "PhysicScene.h"

#include "Tools/NxuStream2/NXU_PhysicsInstantiator.h"
//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicInstancer::CPhysicInstancer (void)
{
    m_pResult = NULL;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicInstancer::~CPhysicInstancer (void)
{
}

//-----------------------------------------------------------------------------
//  InstanceModel
//-----------------------------------------------------------------------------
bool    CPhysicInstancer::InstanceModel (const CPhysicModelGeneric* pModel, 
                                         CPhysicScene*              pScene, 
                                         NxMat34&                   worldPose, 
                                         TInstancedData*            pResult)
{
    /*
	assert( pResult != NULL );

    bool    bOk = true;

    m_pResult = pResult;

    // Utilizamos un instanciador de la libreria Stream2
    NXU::NxuPhysicsInstantiator instancer ( pModel->GetModelData() );

    // Instanciamos la escena raiz
    instancer.instantiateScene( pModel->GetModelData()->mScenes[0]->mId, 
                                true, //ignore_plane
                                *(CPhysicEngine::Ref().GetNxPhysicsSDK()), 
                                this, 
                                pScene->GetNxScene(), 
                                &worldPose ); 

    m_pResult = NULL;

    return  bOk;
	*/

	//!! 
	assert( pResult != NULL );

    bool    bOk = true;

    m_pResult = pResult;

	NXU::instantiateCollection(pModel->GetModelData(), 
								*(CPhysicEngine::Ref().GetNxPhysicsSDK()), 
								pScene->GetNxScene(), 
								&worldPose, 
								this);
		
    m_pResult = NULL;

    return  bOk;
}

//-----------------------------------------------------------------------------
//  NXU_notifyJoint
//-----------------------------------------------------------------------------
void    CPhysicInstancer::NXU_notifyJoint (NxJoint* joint, const char* userProperties)
{
    // Añadimso el joint a los datos instanciados resultado
    m_pResult->aInstancedJoints.pushBack( joint );
}

//-----------------------------------------------------------------------------
//  NXU_notifyActor
//-----------------------------------------------------------------------------
void    CPhysicInstancer::NXU_notifyActor (NxActor* actor, const char* userProperties)
{
    // Añadimso el actor a los datos instanciados resultado
    m_pResult->aInstancedActors.pushBack( actor );
}
