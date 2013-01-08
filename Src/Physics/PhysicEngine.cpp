#include "PhysicEngine.h"
#include "ErrorStream.h"
#include "Tools/NxuStream2/NXU_helper.h"

// Inicializacion del puntero del singleton
CPhysicEngine*  CPhysicEngine::m_pInstance = 0;

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicEngine::CPhysicEngine (void)
{
    // Creacion e inicializacion del objeto principal de PhysX
    int nVersion = NX_PHYSICS_SDK_VERSION;
	NxSDKCreateError errorCode;
    m_pPhysicsSDK = NxCreatePhysicsSDK (NX_PHYSICS_SDK_VERSION, Globals::g_pUserAllocator, this, NxPhysicsSDKDesc(), &errorCode);
	if (!m_pPhysicsSDK)  return;
	
	#ifdef _DEBUG
		m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
	#endif
   
    // Set the physics parameters
    m_pPhysicsSDK->setParameter (NX_SKIN_WIDTH, 0.001f);

    // Set the debug visualization parameters
    m_pPhysicsSDK->setParameter (NX_VISUALIZATION_SCALE, 1);
    m_pPhysicsSDK->setParameter (NX_VISUALIZE_COLLISION_SHAPES, 1);
    m_pPhysicsSDK->setParameter (NX_VISUALIZE_ACTOR_AXES, 1);

    // Inicializacion del motor de cooking para procesamiento de triangleMeshes
    NxInitCooking();

    // Creacion del gestor de controllers
    m_pControllerMgr = NxCreateControllerManager ( Globals::g_pUserAllocator );
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicEngine::~CPhysicEngine (void)
{
    // Liberamos el gestor de controllers
    NxReleaseControllerManager( m_pControllerMgr );

    // Liberacion del motor de fisica
    m_pPhysicsSDK->release();

    // NOTA: Al liberar el SDK, tenemos que liberar tambien la memoria persistente de la libreria de Streaming
    //       para que libere sus tablas de referencia a instancias de meshes que estuvieran cargadas
    NXU::releasePersistentMemory ();
}

//-----------------------------------------------------------------------------
//  Init
//-----------------------------------------------------------------------------
void    CPhysicEngine::Init (void)
{
    assert( m_pInstance == 0 );
    if ( ! m_pInstance )
    {
        m_pInstance = new CPhysicEngine();
    }
}

//-----------------------------------------------------------------------------
//  ShutDown
//-----------------------------------------------------------------------------
void    CPhysicEngine::ShutDown (void)
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//-----------------------------------------------------------------------------
//  Ref
//-----------------------------------------------------------------------------
CPhysicEngine&  CPhysicEngine::Ref (void)
{
    assert( m_pInstance );
    return *m_pInstance;
}

//-----------------------------------------------------------------------------
//  CreateScene
//-----------------------------------------------------------------------------
CPhysicScene*   CPhysicEngine::CreateScene (const CPhysicScene::TInitInfo& initInfo)
{
    CPhysicScene*   pNewScene = new CPhysicScene();

    // Inicializacion de la escena principal
    pNewScene->InitScene (initInfo);

    m_aScenes.push_back (pNewScene);

    return pNewScene;
}

//-----------------------------------------------------------------------------
//  DestroyScene
//-----------------------------------------------------------------------------
void    CPhysicEngine::DestroyScene (CPhysicScene* pScene)
{
    for (int i = 0; i < (int)m_aScenes.size(); i++)
    {
        if ( m_aScenes[i] == pScene )
        {
            delete pScene;
            m_aScenes.erase (m_aScenes.begin() + i);
        }
    }
}
