//*****************************************************************************
// Creacion  : 22/05/2005
// Contenido : Clase para objetos físicos genericos (N actores)
//
//*****************************************************************************
#ifndef _PHYSIC_OBJ_GENERIC_H_
#define _PHYSIC_OBJ_GENERIC_H_

#include "PhysicObj.h"

class   CPhysicModelGeneric;

//////////////////////////////////////////////////////////////////////////////
//  CPhysicObjGeneric
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicObjGeneric : public CPhysicObj
{
    // NOTA: Hacemos friend a la clase padre, para que pueda llamar al constructor
    friend class CPhysicObj;

public:
    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    IPhysicObj
    //////////////////////////////////////////////////////////////////////////

    // Tipo de modelo
    EType               GetType             (void) const    { return IPhysicObj::OBJ_GENERIC; }

    // Acceso al modelo del objeto
    const IPhysicModel* GetPhysicModel      (void) const;

    // Activacion / Desactivacion
    bool                Activate            (void);
    void                Deactivate          (void);

    // Indica si el objeto esta moviendose
    bool                IsMoving            (void) const;

    // Interfaz de interaccion con partes del objeto (solo tenemos 1 parte siempre)
    int                 GetNumParts         (void) const;


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
    CPhysicObjGeneric   (CPhysicScene* pScene, const CPhysicModelGeneric* pPhysicModel);
    virtual ~CPhysicObjGeneric  (void);

private:
    // FUNCIONES PRIVADAS ////////////////////////////////////////////////////

    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    // Escena a la que pertenece
    CPhysicScene*               m_pScene;

    // Modelo fisico asociado
    const CPhysicModelGeneric*  m_pPhysicModel;

    // Array de matrices de partes visuales asociadas
    // TODO: Completar 
    // std::vector<IFrame*>     m_aVisualFrames;

    // Array de actores PhysX
    NxArray<NxActor*>           m_aActors;

    // Array de joints PhysX
    NxArray<NxJoint*>           m_aJoints;

    // Matrix auxiliar para comprobar movimiento de objetos moviles
    NxMat34                     m_LastPose;
};

#endif  // _PHYSIC_OBJ_GENERIC_H_
