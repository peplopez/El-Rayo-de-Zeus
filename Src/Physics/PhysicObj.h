//*****************************************************************************
// Creacion  : 22/05/2005
// Contenido : Clase base para objetos físicos
//
//*****************************************************************************
#ifndef _PHYSIS_OBJ_H_
#define _PHYSIS_OBJ_H_

#include "IPhysicObj.h"

class   IPhysicModel;
class   CPhysicScene;

//////////////////////////////////////////////////////////////////////////////
//  CPhysicObj
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicObj : public IPhysicObj
{
public:
    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    IPhysicObj
    //////////////////////////////////////////////////////////////////////////

    // Tipo de modelo
    EType               GetType             (void) const    { return IPhysicObj::OBJ_UNKNOWN; }

    // Activacion / Desactivacion del objeto fisico
    bool                Activate            (void);
    void                Deactivate          (void);
    bool                IsActivated         (void) const    { return m_bActivated; }

    // Indica si modo de comportamiento fisico del objeto
    EPhysicMode         GetPhysicMode       (void) const    { return m_InitInfo.ePhysicMode; };
    bool                IsStatic            (void) const    { return m_InitInfo.ePhysicMode == PMODE_STATIC; };
    bool                IsKinematic         (void) const    { return m_InitInfo.ePhysicMode == PMODE_KINEMATIC; };
    bool                IsDynamic           (void) const    { return m_InitInfo.ePhysicMode == PMODE_DYNAMIC; };

    // Indica si el objeto esta moviendose
    bool                IsMoving            (void) const;
    
    // Acceso a datos de posicion y orientacion
    NxVec3              GetWorldPosition    (void) const;
    NxVec3              GetWorldPosition    (int nPartIdx) const;
	
	//!!
	void				SetWorldPosition	(const NxVec3 &vPos);
	void				SetWorldPosition	(int nPartIdx, const NxVec3 &vPos);

	//!!
	NxMat33				GetWorldOrientation	() const;
	NxMat33				GetWorldOrientation	(int nPartIdx) const;

	//!!
	void				SetWorldOrientation	(const NxMat33 &mOrient);
	void				SetWorldOrientation	(int nPartIdx, const NxMat33 &mOrient);

    // Devuelve la bbox orientada a mundo del objeto fisico y de sus partes
    NxBounds3           GetWorldBounds      (void) const;
    NxBounds3           GetWorldBounds      (int nPartIdx) const;

    // Densidad:
    void                SetDensity          (int nPartIdx, float fDensity);
    float               GetDensity          (int nPartIdx) const;
    // - Establecer la misma densidad a todas las partes
    void                SetDensityAll       (float fDensity);

    // Masa:
    void                SetMass             (int nPartIdx, float fMass);
    float               GetMass             (int nPartIdx) const;
    // - Establece la masa total repartiendola entre las partes en funcion de su volumen
    void                SetTotalMass        (float fMass);

    // Interfaz de interaccion con el objeto:
    // NOTA: El objeto debe estar activado para poder interactuar con el
    // - Aplicar un fuerza sobre el objeto (en alguna parte especifica del mismo)
    void                ApplyForce          (const NxVec3& vForce, const NxVec3& vPos, NxForceMode eForceMode = NX_FORCE);
    // - Aplicar fuerza de tipo explosivo (afecta a todas las partes del mismo)
    void                ApplyExplosion      (float fForce, const NxVec3& vPos);
    // - Asignar velocidad lineal al objeto 
    void                SetLinearVelocity   (const NxVec3& vVelocity);
    NxVec3              GetLinearVelocity   (void) const;
    // - Asignar friccion lineal 
    void                SetLinearDamping    (float fLinearDamp);
    float               GetLinearDamping    (void) const;
    // - Asignar friccion angular
    void                SetAngularDamping   (float fAngularDamp);
    float               GetAngularDamping   (void) const;

    // Interfaz de interaccion con partes del objeto (ya tenga 1 o varias)
    int                 GetNumParts         (void) const;
    // - Aplicar un fuerza sobre una parte del objeto 
    void                ApplyForceAtPart    (const NxVec3& vForce, const NxVec3& vPos, int nPartIdx, NxForceMode eForceMode = NX_FORCE);
    // - Asignar velocidad lineal al objeto 
    void                SetLinearVelocity   (const NxVec3& vVelocity, int nPartIdx);
    NxVec3              GetLinearVelocity   (int nPartIdx) const;
    // - Asignar friccion lineal 
    void                SetLinearDamping    (float fLinearDamp, int nPartIdx);
    float               GetLinearDamping    (int nPartIdx) const;
    // - Asignar friccion angular
    void                SetAngularDamping   (float fAngularDamp, int nPartIdx);
    float               GetAngularDamping   (int nPartIdx) const;

	// - Mover objetos kinemáticos
	//!!
	void				MoveWorldPosition	(const NxVec3 &vPos);
	void				MoveWorldPosition	(int nPartIdx, const NxVec3 &vPos);
	void				MoveWorldOrientation(const NxMat33 &mOrient);
	void				MoveWorldOrientation(int nPartIdx, const NxMat33 &mOrient);



    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    CPhysicObj
    //////////////////////////////////////////////////////////////////////////

    // Factoria
    static  IPhysicObj*     Factory         (CPhysicScene* pScene, const IPhysicModel* pPhysicModel);

    // Inicializacion del objeto
    virtual void            InitObject      (const TInitInfo& initInfo);

    // Actualizacion.
    virtual void            Update          (float fDeltaTime) = 0;

    // Acceso al actor de PhysX de cada parte:
    virtual NxActor*        GetActor        (int nPartIdx) = 0;
    virtual const NxActor*  GetActor        (int nPartIdx) const = 0;

protected:
    // CONSTRUCCION / DESTRUCCION ////////////////////////////////////////////
    CPhysicObj  (void);
    virtual ~CPhysicObj (void);

    // Aplicar fuerza de explosion a una parte concreta
    virtual void            ApplyExplosionPart  (int nPartIdx, float fForce, const NxVec3& vPos);


    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    // Informacion de inicializacion
    TInitInfo           m_InitInfo;

    // Flag que indica el estado de activacion del objeto
    bool                m_bActivated;

};

#endif  // _PHYSIS_OBJ_H_
