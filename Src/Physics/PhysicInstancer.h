#ifndef _PHYSIC_INSTANCER_H_
#define _PHYSIC_INSTANCER_H_

#include "PhysicGlobals.h"
#include "Tools/NxuStream2/NXU_Helper.h"

class   CPhysicModelGeneric;
class   CPhysicScene;


// DEFINES ///////////////////////////////////////////////////////////////

// Estructura para devolver los datos instanciados
struct  TInstancedData
{
    NxArray< NxActor* >     aInstancedActors;
    NxArray< NxJoint* >     aInstancedJoints;
};


//////////////////////////////////////////////////////////////////////////////
//    CPhysicInstancer
//
//
//////////////////////////////////////////////////////////////////////////////
class    CPhysicInstancer : public NXU_userNotify
{
public:
    // CONSTRUCCION / DESTRUCCION ////////////////////////////////////////////
    CPhysicInstancer   (void);
    ~CPhysicInstancer  (void);

    // Funcion para instanciar un modelo fisico generico en una escena
    bool    InstanceModel   (const CPhysicModelGeneric* pModel, 
                             CPhysicScene*              pScene, 
                             NxMat34&                   worldPose, 
                             TInstancedData*            pResult);


private:
    // FUNCIONES PRIVADAS ////////////////////////////////////////////////////

    /// *** INTERFAZ: NXU_userNotify 

    void    NXU_notifyJoint         (NxJoint* joint,     const char* userProperties);
    void    NXU_notifyJointFailed   (NxJointDesc& joint, const char* userProperties)    { assert(0); }
    void    NXU_notifyActor         (NxActor* actor,     const char* userProperties);
    void    NXU_notifyActorFailed   (NxActorDesc &actor, const char* userProperties)    { assert(0); }


    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    TInstancedData*     m_pResult;
};

#endif // _PHYSIC_INSTANCER_H_