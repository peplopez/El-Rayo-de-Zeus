//*****************************************************************************
// Creacion  : 22/05/2005
// Contenido : Clase para objetos físicos simples (1 solo actor)
//
//*****************************************************************************
#ifndef _PHYSIC_OBJ_CHARACTER_H_
#define _PHYSIC_OBJ_CHARACTER_H_

#include "PhysicObj.h"

class   CPhysicModelCharacter;

//////////////////////////////////////////////////////////////////////////////
//  CPhysicObjCharacter
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicObjCharacter : public CPhysicObj
{
    // NOTA: Hacemos friend a la clase padre, para que pueda llamar al constructor
    friend class CPhysicObj;

public:
    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    IPhysicObj
    //////////////////////////////////////////////////////////////////////////

    // Tipo de modelo
    EType               GetType             (void) const    { return IPhysicObj::OBJ_CHARACTER; }

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
    // *** INTERFAZ:    IPhysicObj
    //////////////////////////////////////////////////////////////////////////

    // Inicializacion del objeto
    void                InitObject          (const TInitInfo& initInfo);

    // Actualizacion
    void                Update              (float fDeltaTime);

    // Acceso al actor de cada parte
    NxActor*            GetActor            (int nPartIdx);
    const NxActor*      GetActor            (int nPartIdx) const;

	// Acceso al controlador
	//!!
	NxController*       GetController       ();
    const NxController* GetController       () const;

	// Mover el objeto físico
	//!!
	NxU32				Move				(const NxVec3 &vDisp, NxU32 collisionGroups, 
											 NxF32 minDist, NxF32 sharpness); 

	// Teletransportar el objeto físico
	//!!
	void				SetWorldPosition	(const NxVec3 &vPos);
	void				SetWorldOrientation	(const NxMat33 &mOrient);
	
	// Posición del controller
	//!!
	NxVec3              GetWorldPosition    (void) const;
    NxVec3              GetWorldPosition    (int nPartIdx) const;
	

protected:
    // NOTA: Declaramos el constructor protegido para que solo se llame desde la Factory
    CPhysicObjCharacter    (CPhysicScene* pScene, const CPhysicModelCharacter* pPhysicModel);
    virtual ~CPhysicObjCharacter   (void);

private:
    // FUNCIONES PRIVADAS ////////////////////////////////////////////////////

    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    // Escena a la que pertenece
    CPhysicScene*                   m_pScene;

    // Modelo fisico asociado
    const CPhysicModelCharacter*    m_pPhysicModel;

    // Frame del objeto visual asociado
    // TODO: Completar 
    // CFrame*                      m_pVisualFrame;

    // Character controller de PhysX
    NxController*                   m_pNxController;

    // Matrix auxiliar para comprobar movimiento de objetos moviles
    NxMat34                         m_LastPose;
};

#endif  // _PHYSIC_OBJ_CHARACTER_H_
