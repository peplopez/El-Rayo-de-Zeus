//*****************************************************************************
// Creacion  : 22/05/2005
// Contenido : Interfaz para objetos físicos
//
//*****************************************************************************
#ifndef _IPHYSIC_OBJ_H_
#define _IPHYSIC_OBJ_H_

#include "PhysicGlobals.h"

class   IPhysicModel;

//////////////////////////////////////////////////////////////////////////////
//  IPhysicObj
//
//
//////////////////////////////////////////////////////////////////////////////
class   IPhysicObj
{
public:
    // DEFINES ///////////////////////////////////////////////////////////////

    // Enumerado con los posibles tipos de objetos físicos
    enum    EType
    {
        OBJ_UNKNOWN = -1,
        OBJ_SIMPLE  = 0,
        OBJ_GENERIC,
        OBJ_CLOTH,
        OBJ_RAGDOLL,
        OBJ_VEHICLE,
        OBJ_CHARACTER

    };

    // Modo fisico de comportamiento del objeto
    enum    EPhysicMode
    {
        PMODE_STATIC = 0,
        PMODE_KINEMATIC,
        PMODE_DYNAMIC
    };

    // Estructura de inicializacion
    struct  TInitInfo
    {
        const IPhysicModel* pPhysicModel;
        NxMat34             vWorldPose;     // Posicion inicial en el mundo
        EPhysicMode         ePhysicMode;    // Modo de comportamiendo fisico 

        // Constructor
        TInitInfo (void)
        {
            pPhysicModel = NULL;
            ePhysicMode = PMODE_DYNAMIC;
        }
    };

public:
    // CONSTRUCCION / DESTRUCCION ////////////////////////////////////////////
    virtual ~IPhysicObj (void) { };


    // OPERACIONES ///////////////////////////////////////////////////////////

    // Tipo de objeto
    virtual EType               GetType             (void) const = 0;

    // Acceso al modelo del objeto
    virtual const IPhysicModel* GetPhysicModel      (void) const = 0;

    // Activacion / Desactivacion del objeto fisico
    // NOTA: La activacion puede fallar, en funcion de la disponibilidad de recursos
    virtual bool                Activate            (void) = 0;
    virtual void                Deactivate          (void) = 0;
    virtual bool                IsActivated         (void) const = 0;    

    // Indica el modo de funcionamiento
    virtual EPhysicMode         GetPhysicMode       (void) const = 0;
    virtual bool                IsStatic            (void) const = 0;
    virtual bool                IsKinematic         (void) const = 0;
    virtual bool                IsDynamic           (void) const = 0;

    // Indica si el objeto esta moviendose
    virtual bool                IsMoving            (void) const = 0;

    // Acceso a datos de posicion y orientacion
    virtual NxVec3              GetWorldPosition    (void) const = 0;
    virtual NxVec3              GetWorldPosition    (int nPartIdx) const = 0;

	//!!
	virtual void				SetWorldPosition	(const NxVec3 &vPos) = 0;
	virtual void				SetWorldPosition	(int nPartIdx, const NxVec3 &vPos) = 0;

	//!!
	virtual NxMat33				GetWorldOrientation	() const = 0;
	virtual NxMat33				GetWorldOrientation	(int nPartIdx) const = 0;

	//!!
	virtual void				SetWorldOrientation	(const NxMat33 &mOrient) = 0;
	virtual void				SetWorldOrientation	(int nPartIdx, const NxMat33 &mOrient) = 0;

    // Devuelve la bbox orientada a mundo del objeto fisico
    virtual NxBounds3           GetWorldBounds      (void) const = 0;
    virtual NxBounds3           GetWorldBounds      (int nPartIdx) const = 0;

    // Densidad:
    virtual void                SetDensity          (int nPartIdx, float fDensity) = 0;
    virtual float               GetDensity          (int nPartIdx) const = 0;
    // - Establecer la misma densidad a todas las partes
    virtual void                SetDensityAll       (float fDensity) = 0;

    // Masa:
    virtual void                SetMass             (int nPartIdx, float fMass) = 0;
    virtual float               GetMass             (int nPartIdx) const = 0;
    // - Establece la masa total repartiendola entre las partes en funcion de su volumen
    virtual void                SetTotalMass        (float fMass) = 0;

    // Interfaz de interaccion con el objeto:
    // NOTA: El objeto debe estar activado para poder interactuar con el
    // - Aplicar un fuerza sobre el objeto 
	virtual void                ApplyForce			(const NxVec3& vForce, const NxVec3& vPos, NxForceMode eForceMode = NX_FORCE ) = 0;
    // - Aplicar fuerza de tipo explosivo
    virtual void                ApplyExplosion      (float fForce, const NxVec3& vPos) = 0;
    // - Asignar velocidad lineal al objeto 
    virtual void                SetLinearVelocity   (const NxVec3& vVelocity) = 0;
    virtual NxVec3              GetLinearVelocity   (void) const = 0;
    // - Asignar friccion lineal a todo el objeto
    virtual void                SetLinearDamping    (float fLinearDamp) = 0;
    virtual float               GetLinearDamping    (void) const = 0;
    // - Asignar friccion angular a todo el objeto
    virtual void                SetAngularDamping   (float fAngularDamp) = 0;
    virtual float               GetAngularDamping   (void) const = 0;

    // Interfaz de interaccion con partes del objeto (ya tenga 1 o varias)
    virtual int                 GetNumParts         (void) const = 0;
    // - Aplicar un fuerza sobre una parte del objeto 
    virtual void                ApplyForceAtPart    (const NxVec3& vForce, const NxVec3& vPos, int nPartIdx, NxForceMode eForceMode = NX_FORCE) = 0;
    // - Asignar velocidad lineal al objeto 
    virtual void                SetLinearVelocity   (const NxVec3& vVelocity, int nPartIdx) = 0;
    virtual NxVec3              GetLinearVelocity   (int nPartIdx) const = 0;
    // - Asignar friccion lineal 
    virtual void                SetLinearDamping    (float fLinearDamp, int nPartIdx) = 0;
    virtual float               GetLinearDamping    (int nPartIdx) const = 0;
    // - Asignar friccion angular
    virtual void                SetAngularDamping   (float fAngularDamp, int nPartIdx) = 0;
    virtual float               GetAngularDamping   (int nPartIdx) const = 0;

	// - Mover objetos kinemáticos
	//!!
	virtual void				MoveWorldPosition	(const NxVec3 &vPos) = 0;
	virtual void				MoveWorldPosition	(int nPartIdx, const NxVec3 &vPos) = 0;
	virtual void				MoveWorldOrientation(const NxMat33 &mOrient) = 0;
	virtual void				MoveWorldOrientation(int nPartIdx, const NxMat33 &mOrient) = 0;

	// Datos de usuario. Con este puntero el usuario puede relacionar los objetos
	// físicos con el resto de objetos de su aplicación.
	//!!
	void* userData;
};

#endif  // _IPHYSIC_OBJ_H_
