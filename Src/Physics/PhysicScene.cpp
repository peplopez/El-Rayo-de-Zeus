#include "PhysicScene.h"
#include "PhysicEngine.h"
#include "PhysicObj.h"
#include "IPhysicModel.h"
//!!
#include "IPhysicCollisionMng.h"

#include "PhysicUtils.h"
//#include "DrawShapes.h"
//#include "Graphics/GestorOpenGL.h"

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicScene::CPhysicScene (void)
{
    m_pScene = NULL;
    m_fDrawExpTimer = 0.f;
	//!!
	m_pCollisionMng = NULL;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicScene::~CPhysicScene (void)
{
    EndScene ();
}


//-----------------------------------------------------------------------------
//  InitScene
//-----------------------------------------------------------------------------
bool    CPhysicScene::InitScene (const TInitInfo& initInfo)
{
    // Create the scene
    m_pScene = CPhysicEngine::Ref().GetNxPhysicsSDK()->createScene (initInfo.nxInfo);
    if ( m_pScene == NULL ) return false;

    // Editar las propiedades del material por defecto
    NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0); 
    defaultMaterial->setRestitution (0.0f);
    defaultMaterial->setStaticFriction (0.5f);
    defaultMaterial->setDynamicFriction (0.4f);

    // CreateBox (NxVec3(0,0,0), NxVec3(10, 10, 10), 1);
    
    // Configuramos callback collisiones
	//!!
    m_pScene->setUserContactReport(this);
	m_pScene->setUserTriggerReport(this);
	m_pCollisionMng = initInfo.pCollisionMng;

    // Inicializar gestor de agua
    CPhysicWaterMgr::TInitInfo  initWaterInfo;
    initWaterInfo.pScene = this;
    initWaterInfo.fWaterLevel = Globals::g_fWaterLevel;
    initWaterInfo.fWaterDensity = 4.f;
    initWaterInfo.fFrictionAngular = 10.f;
    initWaterInfo.fFrictionLinear = 5.f;
    m_WaterMgr.Init( initWaterInfo );

	// Comprobar si debemos activar el gestor de agua
	//!!
	m_bWaterMngEnabled = initInfo.bWaterMngEnabled;

    return true;
}

//-----------------------------------------------------------------------------
//  EndScene
//-----------------------------------------------------------------------------
void    CPhysicScene::EndScene (void)
{
    // Destruir todos los objetos fisicos
    for (int i = 0; i < (int)m_aPhysicObjs.size(); i++)
    {
        delete m_aPhysicObjs[i];
    }
    m_aPhysicObjs.clear();

    // Terminar gestor de agua
    m_WaterMgr.Shutdown();

    // Liberacion de la escena PhysX
    if (m_pScene) CPhysicEngine::Ref().GetNxPhysicsSDK()->releaseScene (*m_pScene);

	//!!
	m_pCollisionMng = NULL;
}

//-----------------------------------------------------------------------------
//  CreatePhysicObj
//-----------------------------------------------------------------------------
IPhysicObj* CPhysicScene::CreatePhysicObj (const IPhysicObj::TInitInfo& initInfo)
{
    assert( m_pScene );

    // Creacion del objeto con la factoria (se creara un tipo u otro en funcion del modelto físico)
    CPhysicObj* pNewObj = (CPhysicObj*) CPhysicObj::Factory (this, initInfo.pPhysicModel);
    // Inicializacion
    pNewObj->InitObject (initInfo);
    // Lo añadimos al array
    m_aPhysicObjs.push_back (pNewObj);

    return pNewObj;
}

//-----------------------------------------------------------------------------
//  DestroyPhysicObj
//-----------------------------------------------------------------------------
void    CPhysicScene::DestroyPhysicObj (IPhysicObj* pPhysicObj)
{
    // Buscar el objeto en el array
    int     nIndex = -1;
    bool    bFound = false;
    for (int i = 0; (i < (int)m_aPhysicObjs.size()) && !bFound; i++)
    {
        if ( m_aPhysicObjs[i] == pPhysicObj )
        {
            nIndex = i;
            bFound = true;
        }
    }
    // Deberiamos haberlo encontrado
    assert( bFound );
    // Lo eliminamos
    delete pPhysicObj;
    m_aPhysicObjs.erase (m_aPhysicObjs.begin() + nIndex);
}

//-----------------------------------------------------------------------------
//  GetNumPhysicObjs
//-----------------------------------------------------------------------------
int     CPhysicScene::GetNumPhysicObjs (void) const
{
    return m_aPhysicObjs.size();
}

//-----------------------------------------------------------------------------
//  GetPhysicObj
//-----------------------------------------------------------------------------
IPhysicObj* CPhysicScene::GetPhysicObj (int nIndex)
{
    assert( nIndex < (int)m_aPhysicObjs.size() );
    return m_aPhysicObjs[nIndex];
}

//-----------------------------------------------------------------------------
//  StepSimulation      
//-----------------------------------------------------------------------------
void    CPhysicScene::StepSimulation (float fDeltaTime)
{
	// Tenemos que actualizar los character controllers
	CPhysicEngine::Ref().GetNxControllerMgr()->updateControllers();

    // Run collision and dynamics for delta time since the last frame
    m_pScene->simulate (fDeltaTime);    
    m_pScene->flushStream ();
    m_pScene->fetchResults (NX_RIGID_BODY_FINISHED, true);

    // Actualizacion del gestor de agua
	//!!
	if (m_bWaterMngEnabled)
		m_WaterMgr.Update( fDeltaTime );

    // REVIEW: Debug
    // fDeltaTime *= 0.3f;
    if ( m_fDrawExpTimer > 0.f )    m_fDrawExpTimer -= fDeltaTime;
}

//-----------------------------------------------------------------------------
//  CreateGroundPlane
//-----------------------------------------------------------------------------
NxActor*    CPhysicScene::CreateGroundPlane (void)
{
    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;
    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
    return m_pScene->createActor(actorDesc);
}

//-----------------------------------------------------------------------------
//  CreateBox
//-----------------------------------------------------------------------------
NxActor*    CPhysicScene::CreateBox (const NxVec3& pos, const NxVec3& boxDim, const NxReal density)
{
    // Add a single-shape actor to the scene
    NxActorDesc actorDesc;
    NxBodyDesc bodyDesc;

    // The actor has one shape, a box
    NxBoxShapeDesc boxDesc;
    boxDesc.dimensions.set (boxDim.x, boxDim.y, boxDim.z);
    boxDesc.localPose.t = NxVec3 (0, boxDim.y, 0);
    actorDesc.shapes.pushBack (&boxDesc);

    actorDesc.body = &bodyDesc;
    actorDesc.density = density;
    actorDesc.globalPose.t = pos;
    return m_pScene->createActor(actorDesc);    
}

//-----------------------------------------------------------------------------
//  CreateTable
//-----------------------------------------------------------------------------
NxActor*    CPhysicScene::CreateTable (const NxVec3& pos, const NxVec3& boxDim, const NxReal density)
{
    // Add a single-shape actor to the scene
    NxActorDesc actorDesc;
    NxBodyDesc bodyDesc;

    bodyDesc.angularVelocity = NxVec3 (10.f, 0.f, 0.f);

    // The actor has one shape, a box
    NxBoxShapeDesc boxDescTable;
    boxDescTable.dimensions.set (boxDim.x, boxDim.y * 0.2f, boxDim.z);
    boxDescTable.localPose.t = NxVec3 (0, boxDim.y, 0);
    NxBoxShapeDesc boxDescPata;
    boxDescPata.dimensions.set (boxDim.x * 0.1f, boxDim.y, boxDim.z * 0.1f);
    boxDescPata.localPose.t = NxVec3 (boxDim.x, boxDim.y * 0.5f, boxDim.z);
    NxBoxShapeDesc boxDescPata1;
    boxDescPata1.dimensions.set (boxDim.x * 0.1f, boxDim.y, boxDim.z * 0.1f);
    boxDescPata1.localPose.t = NxVec3 (-boxDim.x, boxDim.y * 0.5f, boxDim.z);
    NxBoxShapeDesc boxDescPata2;
    boxDescPata2.dimensions.set (boxDim.x * 0.1f, boxDim.y, boxDim.z * 0.1f);
    boxDescPata2.localPose.t = NxVec3 (boxDim.x, boxDim.y * 0.5f, -boxDim.z);
    NxBoxShapeDesc boxDescPata3;
    boxDescPata3.dimensions.set (boxDim.x * 0.1f, boxDim.y, boxDim.z * 0.1f);
    boxDescPata3.localPose.t = NxVec3 (-boxDim.x, boxDim.y * 0.5f, -boxDim.z);

    actorDesc.shapes.pushBack (&boxDescTable);
    actorDesc.shapes.pushBack (&boxDescPata);
    actorDesc.shapes.pushBack (&boxDescPata1);
    actorDesc.shapes.pushBack (&boxDescPata2);
    actorDesc.shapes.pushBack (&boxDescPata3);

    actorDesc.body = &bodyDesc;
    actorDesc.density = density;
    actorDesc.globalPose.t = pos;
    return m_pScene->createActor(actorDesc);    
}

//-----------------------------------------------------------------------------
//  CreateSphere
//-----------------------------------------------------------------------------
NxActor*    CPhysicScene::CreateSphere (const NxVec3& pos, const NxReal radius, const NxReal density)
{
    // Add a single-shape actor to the scene
    NxActorDesc actorDesc;
    NxBodyDesc bodyDesc;

    // The actor has one shape, a sphere
    NxSphereShapeDesc sphereDesc;
    sphereDesc.radius = radius;
    sphereDesc.localPose.t = NxVec3 (0, radius, 0);
    actorDesc.shapes.pushBack (&sphereDesc);

    actorDesc.body = &bodyDesc;
    actorDesc.density = density;
    actorDesc.globalPose.t = pos;
    return m_pScene->createActor(actorDesc);    
}

//-----------------------------------------------------------------------------
//  CreateCapsule
//-----------------------------------------------------------------------------
NxActor*    CPhysicScene::CreateCapsule (const NxVec3& pos, const NxReal height, const NxReal radius, const NxReal density)
{
    // Add a single-shape actor to the scene
    NxActorDesc actorDesc;
    NxBodyDesc bodyDesc;

    bodyDesc.linearVelocity = NxVec3 (1.f, 0.f, 0.f);
    bodyDesc.massLocalPose.t = NxVec3 (0.f, 1.f, 0.f);

    // The actor has one shape, a capsule
    NxCapsuleShapeDesc capsuleDesc;
    capsuleDesc.height = height;
    capsuleDesc.radius = radius;
    capsuleDesc.localPose.t = NxVec3 (0, radius + 0.5f*height, 0);
    actorDesc.shapes.pushBack (&capsuleDesc);

    actorDesc.body = &bodyDesc;
    actorDesc.density = density;
    actorDesc.globalPose.t = pos;
    return m_pScene->createActor(actorDesc);    
}

//-----------------------------------------------------------------------------
//  CreateBodySphericalJoint
//-----------------------------------------------------------------------------
NxSphericalJoint* CPhysicScene::CreateBodySphericalJoint(NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis)
{
    NxSphericalJointDesc sphericalDesc;
    sphericalDesc.actor[0] = a0;
    sphericalDesc.actor[1] = a1;
    sphericalDesc.setGlobalAnchor(globalAnchor);
    sphericalDesc.setGlobalAxis(globalAxis);

    sphericalDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
    sphericalDesc.twistLimit.low.value = -(NxReal)0.025*NxPi;
    sphericalDesc.twistLimit.low.hardness = 0.5;
    sphericalDesc.twistLimit.low.restitution = 0.5;
    sphericalDesc.twistLimit.high.value = (NxReal)0.025*NxPi;
    sphericalDesc.twistLimit.high.hardness = 0.5;
    sphericalDesc.twistLimit.high.restitution = 0.5;

    sphericalDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
    sphericalDesc.swingLimit.value = (NxReal)0.25*NxPi;
    sphericalDesc.swingLimit.hardness = 0.5;
    sphericalDesc.swingLimit.restitution = 0.5;

    sphericalDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
    sphericalDesc.twistSpring.spring = 0.5;
    sphericalDesc.twistSpring.damper = 1;

    sphericalDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
    sphericalDesc.swingSpring.spring = 0.5;
    sphericalDesc.swingSpring.damper = 1;

    //  sphericalDesc.flags |= NX_SJF_JOINT_SPRING_ENABLED;
    //  sphericalDesc.jointSpring.spring = 0.5;
    //  sphericalDesc.jointSpring.damper = 1;

    sphericalDesc.projectionDistance = (NxReal)0.15;
    sphericalDesc.projectionMode = NX_JPM_POINT_MINDIST;

    return (NxSphericalJoint*)m_pScene->createJoint(sphericalDesc);
}

//-----------------------------------------------------------------------------
//  CreateRevoluteJoint
//-----------------------------------------------------------------------------
NxRevoluteJoint* CPhysicScene::CreateRevoluteJoint(NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis)
{
    NxRevoluteJointDesc revDesc;
    revDesc.actor[0] = a0;
    revDesc.actor[1] = a1;
    revDesc.setGlobalAnchor(globalAnchor);
    revDesc.setGlobalAxis(globalAxis);

    return (NxRevoluteJoint*)m_pScene->createJoint(revDesc);
}

//-----------------------------------------------------------------------------
//  GetGravity
//-----------------------------------------------------------------------------
NxVec3  CPhysicScene::GetGravity (void) const
{
    NxVec3  result;
    m_pScene->getGravity (result);

    return result;
}

//-----------------------------------------------------------------------------
//  ApplyForce
//-----------------------------------------------------------------------------
void    CPhysicScene::ApplyForce (const NxVec3& forceDir, float fForce, NxForceMode eForceMode)
{
    int nbActors = m_pScene->getNbActors();
    NxActor** actors = m_pScene->getActors();
    while (nbActors--)
    {
        NxActor* actor = *actors++;
        ApplyForceToActor (actor, forceDir, fForce, eForceMode);
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void    CPhysicScene::ApplyExplosion (const NxVec3& pos, float fForce, float fMaxRadius)
{
    // Vamos a recorrer todos los objetos para aplicarles la explosion en caso de que proceda
    for ( int i = 0; i < (int)m_aPhysicObjs.size(); ++i )
    {
        IPhysicObj* pObj = m_aPhysicObjs[i];

        // Comprobar si el objeto es afectado por la explosion
        // - Solo queremos objetos activados y que sean dinamicos
        bool    bAffected = (pObj->IsActivated() && pObj->IsDynamic());
        // - Comprobamos ademas que cae dentro del radio de accion
        if ( fMaxRadius > 0.f )
        {
            bAffected &= ((pObj->GetWorldPosition()-pos).magnitudeSquared() < (fMaxRadius*fMaxRadius));
        }
        // - Hacer test de oclusion
        bAffected &= CheckExplosionOclusion (pos, pObj);

        // Si es afectado, le aplicamos la fuerza explosiva
        if ( bAffected )
        {
            pObj->ApplyExplosion( fForce, pos );
        }
    }

    // REVIEW: Pintado de debug para la explosion
    m_fDrawExpTimer = 5.f;
    m_vExpPoint = pos;
	m_fExpRadius = fMaxRadius;
}

//-----------------------------------------------------------------------------
//  CheckExplosionOclusion
//-----------------------------------------------------------------------------
bool    CPhysicScene::CheckExplosionOclusion (const NxVec3& expPos, IPhysicObj* pObj)
{
    // TODO: Completar...
    return true;
}

//-----------------------------------------------------------------------------
//  CalcIntersection
//-----------------------------------------------------------------------------
bool    CPhysicScene::CalcIntersection (const NxRay& worldRay, TIntersectInfo& result, NxShapesType eShapeType) const
{
    NxRaycastHit    hit;
    bool            bHayHit = (m_pScene->raycastClosestShape (worldRay, eShapeType, hit) != NULL);
    if ( bHayHit )
    {
        TActorInfo* pActorInfo = (TActorInfo*) hit.shape->getActor().userData;
        result.pPhysicObj = pActorInfo->pPhysicObj;
        result.nPartIdx = pActorInfo->nPartIdx;
        result.vImpact = hit.worldImpact;
    }

    return bHayHit;
}

//-----------------------------------------------------------------------------
//  Render
//-----------------------------------------------------------------------------
/*
void    CPhysicScene::Render (void)
{
    CGestorOpenGL::Ref().nSetRenderMode (RENDER_MODE_FLAT);

    // Render all the actors in the scene
    int nbActors = m_pScene->getNbActors();
    NxActor** actors = m_pScene->getActors();
    while (nbActors--)
    {
        NxActor* actor = *actors++;
        DrawActor (actor);

        // Handle shadows
        if ( Globals::g_bShadows )
        {
            DrawActorShadow (actor);
        }
    }

    // REVIEW: Render Debug
    if ( m_fDrawExpTimer > 0.f )
    {
        NxSphere    sphere( m_vExpPoint, m_fExpRadius );
        NxVec3      color( 1.f, 1.f, 1.f );
        DrawWireSphere(&sphere, color);
    }
}
*/
//-----------------------------------------------------------------------------
//  onContactNotify
//-----------------------------------------------------------------------------
//!!
void    CPhysicScene::onContactNotify( NxContactPair& pair, NxU32 events)
{
    if (!m_pCollisionMng)
		return;

	m_pCollisionMng->onContactNotify(ContactPair(pair), events);
}

//-----------------------------------------------------------------------------
//  onShapeHit
//-----------------------------------------------------------------------------
//!!
NxControllerAction  CPhysicScene::onShapeHit (const NxControllerShapeHit &hit)
{
	if (!m_pCollisionMng)
		return NX_ACTION_NONE;
	
	return m_pCollisionMng->onCharacterObjHit(CharacterObjHit(hit));
}

//-----------------------------------------------------------------------------
//  onControllerHit
//-----------------------------------------------------------------------------
//!!
NxControllerAction  CPhysicScene::onControllerHit (const NxControllersHit &hit)
{
	if (!m_pCollisionMng)
		return NX_ACTION_NONE;
	
	return m_pCollisionMng->onCharactersHit(CharactersHit(hit));
}

//-----------------------------------------------------------------------------
//  onTrigger
//-----------------------------------------------------------------------------
//!!
void  CPhysicScene::onTrigger (NxShape &triggerShape, NxShape &otherShape, NxTriggerFlag status)
{
	if (!m_pCollisionMng)
		return;
		
	TActorInfo *triggerInfo = (TActorInfo*) triggerShape.getActor().userData;
	TActorInfo *otherInfo = (TActorInfo*) otherShape.getActor().userData;
	m_pCollisionMng->onTrigger(*triggerInfo, *otherInfo, status);
}
