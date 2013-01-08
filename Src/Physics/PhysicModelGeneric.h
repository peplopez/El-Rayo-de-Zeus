//*****************************************************************************
// Creacion  : 21/05/2005
// Contenido : Modelo físico generico (formado por N cuerpo rígido unidos por joints)
//
//*****************************************************************************
#ifndef _PHYSIC_MODEL_GENERIC_H_
#define _PHYSIC_MODEL_GENERIC_H_

#include "IPhysicModel.h"

/// FORWARD REFERENCES ///
namespace   NXU {
    class       NxuPhysicsCollection;
    class       NxActorDesc;
    class       NxShapeDesc;
    class       NxJointDesc;
}

//////////////////////////////////////////////////////////////////////////////
//  CPhysicModelGeneric
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicModelGeneric  :  public IPhysicModel
{
public:
    // DEFINES ///////////////////////////////////////////////////////////////

    // Informacion de inicializacion del modelo
    struct  TInitInfo
    {
        const char*     pszModelFile;
        // Constructor
        TInitInfo()
        :   pszModelFile (NULL)
        {}
    };

public:
    // OPERACIONES ///////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    IPhysicModel
    //////////////////////////////////////////////////////////////////////////

    // Tipo de modelo
    EType                   GetType         (void) const    { return MODEL_GENERIC; }

    // Calculo del volumen del modelo (suma de todos los volumentes de colision)
    float                   CalcVolume      (void) const;
    float                   CalcVolume      (int nPartIdx) const;

    // Devuelve la bounding box alineada en espacio local
    NxBox                   CalcBBox        (void) const;


    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    CPhysicModelGeneric
    //////////////////////////////////////////////////////////////////////////

    // Inicializacion / terminacion
    bool                    Init            (const TInitInfo& initInfo);
    bool                    IsOk            () const        { return m_bInit; }
    void                    End             ();

    // Creacion de partes rígidas:
    // NOTA: Al añadir cualquier elemento se devuelve el indice para acceder al mismo, pero este 
    //       indice dejara de ser valido en cuanto se elimine algun otro elemento del mismo tipo 
    //       del modelo
    int                     AddPart         (const std::string& partName);
    void                    DelPart         (int nPartIdx);
    int                     GetNumParts     (void) const;
    const NXU::NxActorDesc* GetPart         (int nPartIdx) const;
    NXU::NxActorDesc*       EditPart        (int nPartIdx);

    // Modelo de colision (de cada parte):
    int                     AddShape        (int nPartIdx, NxShapeType shapeType);
    void                    DelShape        (int nPartIdx, int nShapeIdx);
    int                     GetNumShapes    (int nPartIdx) const;
    const NXU::NxShapeDesc* GetShape        (int nPartIdx, int nShapeIdx) const;
    NXU::NxShapeDesc*       EditShape       (int nPartIdx, int nShapeIdx);

    // Gestion de la pose de una parte en el espacio de modelado
    const NxMat34&          GetPartPose     (int nPartIdx) const;
    NxMat34&                EditPartPose    (int nPartIdx);

    // Densidad:
    void                    SetDensity      (int nPartIdx, float fDensity);
    float                   GetDensity      (int nPartIdx) const;
    // - Establecer la misma densidad a todas las partes
    void                    SetDensityAll   (float fDensity);

    // Masa:
    void                    SetMass         (int nPartIdx, float fMass);
    float                   GetMass         (int nPartIdx) const;
    // - Establece la masa total repartiendola entre las partes en funcion de su volumen
    void                    SetTotalMass    (float fMass);

    // Gestion de conexiones entre partes (joints)
    // NOTAS: 
    //  - El punto y el eje que definen el joint se guarda en el espacio global 
    //    de modelado, para que no varie su definicion en caso de editar la pose de alguna
    int                     AddJoint        (NxJointType eType);
    void                    DelJoint        (int nJointIdx);
    int                     GetNumJoints    (void) const;
    const NXU::NxJointDesc* GetJoint        (int nJointIdx) const;
    NXU::NxJointDesc*       EditJoint       (int nJointIdx);

protected:
    friend  class   IPhysicModel;
    friend  class   CPhysicInstancer;

    // Construccion / Destruccion
    CPhysicModelGeneric     (void);
    virtual ~CPhysicModelGeneric    (void);

    // Acceso a la coleccion de datos
    NXU::NxuPhysicsCollection*  GetModelData () const   { return m_pCollection; }


private:
    // DEFINES ///////////////////////////////////////////////////////////////

    // FUNCIONES PRIVADAS ////////////////////////////////////////////////////

    // Cargar un fichero de modelo generico
    bool        LoadModelFile       (const char* pszFilename);

    // Crear un modelo vacio por defecto
    bool        CreateEmptyModel    ();


    // DATOS MIEMBRO /////////////////////////////////////////////////////////
    
    // Flag de inicializacion
    bool                        m_bInit;

    // Coleccion de datos del modelo
    NXU::NxuPhysicsCollection*  m_pCollection;

};

#endif  // _PHYSIC_MODEL_GENERIC_H_
