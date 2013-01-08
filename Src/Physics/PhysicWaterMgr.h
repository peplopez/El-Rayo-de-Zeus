//*****************************************************************************
// Creacion  : 19/04/2005
// Contenido : Clase gestora de la fisica de zonas de agua
//
//*****************************************************************************
#ifndef _PHYSIC_WATER_MGR_H_
#define _PHYSIC_WATER_MGR_H_

#include "PhysicGlobals.h"

class   IPhysicObj;
class   CPhysicObj;
class   CPhysicScene;

//////////////////////////////////////////////////////////////////////////////
//  CPhysicWaterMgr
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicWaterMgr
{
public:
    // DEFINES ///////////////////////////////////////////////////////////////

    // Constantes con valores por defecto
    static const float  DFT_WATER_DENSITY;
    static const float  DFT_WATER_FRICTION_LINEAR;
    static const float  DFT_WATER_FRICTION_ANGULAR;

    // Estructura de inicializacion del gestor
    struct  TInitInfo
    {
        float           fWaterLevel;        // Nivel de altura del agua
        float           fWaterDensity;      // Densidad relativa del agua
        NxVec3          vWaterDespl;        // Direccion de movimiento del agua
        float           fFrictionLinear;    // Factor de friccion lineal del agua
        float           fFrictionAngular;   // Factor de friccion angular del agua
        CPhysicScene*   pScene;             // Escena fisica asociada

        // Constructor
        TInitInfo (void)
        {
            fWaterLevel         = 0;
            fWaterDensity       = DFT_WATER_DENSITY;
            fFrictionLinear     = DFT_WATER_FRICTION_LINEAR;
            fFrictionAngular    = DFT_WATER_FRICTION_ANGULAR;
            pScene              = NULL;
        }
    };

    // Estados posibles de un objeto con respecto a la zona de agua
    enum    EObjSituation
    {
        OBJ_OUT_OF_WATER = 0,
        OBJ_FLOATING,
        OBJ_SUMERGED
    };

public:
    // CONSTRUCCION / DESTRUCCION ////////////////////////////////////////////
    CPhysicWaterMgr     (void);
    ~CPhysicWaterMgr    (void);

    // Estado de un objeto en el agua
    EObjSituation   GetObjSituation     (IPhysicObj* pPhysicObj) const;

    // Cambiar el nivel de altura del agua
    void            SetWaterLevel       (float fHeight);
    float           GetWaterLevel       (void) const    { return m_fWaterLevel; }

    // Interfaz para definir direccion de movimiento del agua
    void            AddDirPoint         (const NxVec3& vPoint);
    uint            GetNumDirPoints     (void) const;
    const NxVec3*   GetDirPoint         (uint nIndex) const;
    void            ClearDirPoints      (void);
    // Definir la fuerza global de arrastre del agua
    void            SetWaterDragForce   (float fDragForce);
    float           GetWaterDragForce   (void) const;

    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ ESPECIFICO DE MOTOR
    //////////////////////////////////////////////////////////////////////////

    // Gestion de inicializacion
    void            Init                (const TInitInfo&   initInfo);
    void            Shutdown            (void);
    bool            IsInitialized       (void) const    { return m_bInitialized; }
    
    // Activacion / Desactivacion
    void            SetActive           (bool bValue);
    bool            IsActive            (void) const;

    // Actualizacion
    void            Update              (float fDeltaTime);

    // Notivicacion de eliminacion de objetos fisicos
    void            OnDelPhysicObj      (IPhysicObj* pObjDeleted);

private:
    // DEFINES ///////////////////////////////////////////////////////////////

    // Estructura con informacion asociada a objetos que estan en el agua
    struct  TObjInWater
    {
        CPhysicObj*         pPhysicObj;
        float               fObjVolume;
        EObjSituation       eSituation;
    };

    // Estructura con info sobre el volumen sumergido de cada una de las partes de un objeto
    struct  TSumergedVolume
    {
        struct  TInfo
        {
            uint        uPartIdx;
            float       fVolume;
            float       fUnderWaterDepth;
            NxVec3      vCoM;
        };
        NxArray<TInfo>  aVolInfo;
    };

    // FUNCIONES PRIVADAS ////////////////////////////////////////////////////

    // Busca la informacion asociada a un objeto
    uint            FindObjInWater      (CPhysicObj* pPhysicObj) const;

    // Añadir un elemento al pool
    uint            AddObjInWater       (CPhysicObj* pPhysicObj);

    // Eliminar un elemento del pool
    void            DelObjInWater       (uint uIndex);

    // Procesa la interaccion del objeto con el agua. Devuelve true si el objeto esta en el agua
    void            ProcessPhysicObj    (CPhysicObj* pPhysicObj, float fDeltaTime);

    // Funcion auxiliar que determina si un objeto esta cercano al nivel del agua
    // Devuelve true si el objeto esta parcial o totalmente en el agua
    bool            CalcSumergedVolume  (TObjInWater* pObjInWater, TSumergedVolume* pResult);
    
    // Funcion que aplica la fuerzas necesario en funcion del volumen sumergido:
    //  - Aplica la fuerza de flotacion al objeto procesado
    //  - Aplica una fuerza de expansiva a todos los objetos flotantes cercanos
    void            ApplyWaterForces    (TObjInWater* pObjInWater, const TSumergedVolume& tSumergedVol, float fDeltaTime);

    // Funcion auxiliar para calcular las interseccion del una caja con el plano de agua
    // NOTA: Devuelve el porcentaje de penetracion (0..1) de la caja en el agua
    float           CalcBoxUnderWater   (const NxBox& tBox, NxArray<NxVec3>& vResult);

    // Funcion auxiliar para el calculo del volumen de una caja
    float           CalcBoxVolume       (const NxBox& tBox) const;

    // Calculo del centro de masas de una nube de puntos
    NxVec3      CalcCOM             (TObjInWater* pObjInWater, const NxArray<NxVec3>& vPoints) const;

    // Calculo de la fuerza asociada a un volumen sumergido
    NxVec3      CalcBuoyancyForce   (float fVolume) const;


    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    // Informacion de inicializacion
    TInitInfo               m_InitInfo;
    float                   m_fWaterLevel;
    bool                    m_bInitialized;
    bool                    m_bActive;

    // - Vector de objetos fisicos activos que interactuan con el agua
    NxArray<TObjInWater>    m_aObjsInWater;

    // Vectores auxiliares
    NxArray<uint>           m_vUnderWater;
    NxArray<uint>           m_vOverWater;
    NxArray<NxVec3>         m_vBoxPoints;
};


#endif // _PHYSIC_WATER_MGR_H_
