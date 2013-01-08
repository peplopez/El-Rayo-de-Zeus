//*****************************************************************************
// Creacion  : 10/05/2005
// Contenido : Clase de gestion de la escena fisica
//
//*****************************************************************************
#ifndef _PHYSIC_SCENE_H_
#define _PHYSIC_SCENE_H_

#include "Globals.h"
#include "IPhysicObj.h"
#include "NxUserContactReport.h"
#include "PhysicWaterMgr.h"

class IPhysicModel;
//!!
class IPhysicCollisionMng;

// Estructura para intersecciones
struct TIntersectInfo
{
    IPhysicObj* pPhysicObj;
    int         nPartIdx;
    NxVec3      vImpact;
    // Constructor
    TIntersectInfo (void)
    {
        pPhysicObj = NULL;
        nPartIdx = -1;
    }
};

//////////////////////////////////////////////////////////////////////////////
//  CPhysicScene
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicScene : public NxUserContactReport,
						//!!
					   public NxUserTriggerReport,
					   public NxUserControllerHitReport
{
public:
    // DEFINES ///////////////////////////////////////////////////////////////
    
    // Estructura de inicializacion
    struct  TInitInfo
    {
        std::string strSceneName;
        NxSceneDesc nxInfo;
		//!!
		bool bWaterMngEnabled;
		//!!
		IPhysicCollisionMng *pCollisionMng; // La escena no destruye este objeto al finalizar

        // Constructor
        TInitInfo (void)
        {
            nxInfo.gravity.set (0.f, -9.8f, 0.f);
            nxInfo.timeStepMethod = NX_TIMESTEP_VARIABLE;
			//!!
			bWaterMngEnabled = true;
			pCollisionMng = NULL;
        }
    };

    
public:
    // OPERACIONES ///////////////////////////////////////////////////////////

// Creacion de objetos fisicos
    IPhysicObj*     CreatePhysicObj     (const IPhysicObj::TInitInfo& initInfo);
    void            DestroyPhysicObj    (IPhysicObj* pPhysicObj);
    int             GetNumPhysicObjs    (void) const;
    IPhysicObj*     GetPhysicObj        (int nIndex);

    // Aplica un fuerza global a todos los objetos de la escena (sobre su CoM)
	void            ApplyForce          (const NxVec3& forceDir, float fForce, NxForceMode eForceMode = NX_FORCE);

    // Aplica un fuerza explosiva
    // - Si la fuerza es negativa se atrae a los objetos hacia la posicion indicada
    // - Para no limitar el radio de accion: fMaxRadios <= 0
    void            ApplyExplosion      (const NxVec3& pos, float fForce, float fMaxRadius);

    // Simulación
    void            StepSimulation      (float fDeltaTime);

    // Render. Pinta la informacion física de la escena
    //void            Render              (void);

    // Acceder al valor de la gravedad de la escena
    NxVec3          GetGravity          (void) const;

    // Acceso al objeto NxScene
    NxScene*        GetNxScene          (void)  { return m_pScene; }

    // Raycasting
    bool            CalcIntersection    (const NxRay& ray, TIntersectInfo& result, NxShapesType eShapeType = NX_ALL_SHAPES) const; 

    // Creacion de objetos simples
    NxActor*        CreateGroundPlane   (void);
    NxActor*        CreateBox           (const NxVec3& pos, const NxVec3& boxDim, const NxReal density);
    NxActor*        CreateSphere        (const NxVec3& pos, const NxReal radius, const NxReal density);
    NxActor*        CreateCapsule       (const NxVec3& pos, const NxReal height, const NxReal radius, const NxReal density);
    NxActor*        CreateTable         (const NxVec3& pos, const NxVec3& boxDim, const NxReal density);

    // Creacion de joints
    NxSphericalJoint*   CreateBodySphericalJoint    (NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis);
    NxRevoluteJoint*    CreateRevoluteJoint         (NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis);


	// INTERFAZ: NxUserContactReport
	//!!
	void    onContactNotify     ( NxContactPair& pair, NxU32 events);

	// INTERFAZ: NxUserControllerHitReport.
	//!!
	NxControllerAction  onShapeHit (const NxControllerShapeHit &hit);
	NxControllerAction  onControllerHit (const NxControllersHit &hit); 

	// INTERFAZ: NxUserTriggerReport 
	//!!
	void  onTrigger (NxShape &triggerShape, NxShape &otherShape, NxTriggerFlag status);
  

protected:
    // Constructores protegidos, porque las escenas se deben crear con CPhysicEngine::CreateScene
    friend  class   CPhysicEngine;

    // Construccion / Destruccion
    CPhysicScene  (void);
    ~CPhysicScene (void);

    // Inicializacion de escena. La realiza CPhysicEngine
    bool        InitScene           (const TInitInfo& initInfo);
    void        EndScene            (void);

private:
    // OPERACIONES ///////////////////////////////////////////////////////////

    // Funcion para realizar el test de oclusion en explosiones.
    // Devuelve true si el objeto es afectado, false en caso contrario
    bool        CheckExplosionOclusion      (const NxVec3& expPos, IPhysicObj* pObj);


    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    // Escene de PhysX
    NxScene*                    m_pScene;

    // Array de objetos fisicos
    std::vector<IPhysicObj*>    m_aPhysicObjs;

    // Informacion para depuracion visual
    float                       m_fDrawExpTimer;
    NxVec3                      m_vExpPoint;
	float						m_fExpRadius;

    // Gestor de fisica de flotacion
    CPhysicWaterMgr		        m_WaterMgr;

	// Indica si el gestor de física de flotación está activo
	//!!
	bool						m_bWaterMngEnabled; 

	// Gestor de colisiones externo
	//!!
	IPhysicCollisionMng*		m_pCollisionMng;
};


#endif // _PHYSIC_SCENE_H_