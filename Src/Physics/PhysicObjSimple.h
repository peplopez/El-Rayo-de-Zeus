//*****************************************************************************
// Creacion  : 22/05/2005
// Contenido : Clase para objetos físicos simples (1 solo actor)
//
//*****************************************************************************
#ifndef _PHYSIC_OBJ_SIMPLE_H_
#define _PHYSIC_OBJ_SIMPLE_H_

#include "PhysicObj.h"

class   CPhysicModelSimple;

//////////////////////////////////////////////////////////////////////////////
//  CPhysicObjSimple
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicObjSimple : public CPhysicObj
{
    // NOTA: Hacemos friend a la clase padre, para que pueda llamar al constructor
    friend class CPhysicObj;

public:
    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    IPhysicObj
    //////////////////////////////////////////////////////////////////////////

    // Tipo de modelo
    EType               GetType             (void) const    { return IPhysicObj::OBJ_SIMPLE; }

    // Acceso al modelo del objeto
    const IPhysicModel* GetPhysicModel      (void) const;

    // Activacion / Desactivacion
    bool                Activate            (void);
    void                Deactivate          (void);

    // Indica si el objeto esta moviendose
    bool                IsMoving            (void) const;

    // Interfaz de interaccion con partes del objeto (solo tenemos 1 parte siempre)
    int                 GetNumParts         (void) const    { return 1; };

    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    CPhysicObj
    //////////////////////////////////////////////////////////////////////////

    // Inicializacion del objeto
    void                InitObject          (const TInitInfo& initInfo);

    // Actualizacion
    void                Update              (float fDeltaTime);

    // Acceso al actor de cada parte
    NxActor*            GetActor            (int nPartIdx);
    const NxActor*      GetActor            (int nPartIdx) const;


protected:
    // NOTA: Declaramos el constructor protegido para que solo se llame desde la Factory
    CPhysicObjSimple    (CPhysicScene* pScene, const CPhysicModelSimple* pPhysicModel);
    virtual ~CPhysicObjSimple   (void);

private:
    // FUNCIONES PRIVADAS ////////////////////////////////////////////////////

    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    // Escena a la que pertenece
    CPhysicScene*               m_pScene;

    // Modelo fisico asociado
    const CPhysicModelSimple*   m_pPhysicModel;

    // Frame del objeto visual asociado
    // TODO: Completar 
    // CFrame*                  m_pVisualFrame;

    // Actor PhysX
    NxActor*                    m_pActor;

    // Matrix auxiliar para comprobar movimiento de objetos moviles
    NxMat34                     m_LastPose;
};

#endif  // _PHYSIC_OBJ_SIMPLE_H_
