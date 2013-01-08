//*****************************************************************************
// Creacion  : 21/05/2005
// Contenido : Modelo físico simple (formado por 1 cuerpo rígido)
//
//*****************************************************************************
#ifndef _PHYSIC_MODEL_SIMPLE_H_
#define _PHYSIC_MODEL_SIMPLE_H_

#include "IPhysicModel.h"

class   CGLObject;

//////////////////////////////////////////////////////////////////////////////
//  CPhysicModelSimple
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicModelSimple  :  public IPhysicModel
{
public:

    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    IPhysicModel
    //////////////////////////////////////////////////////////////////////////

    // Tipo de modelo
    EType               GetType         (void) const    { return MODEL_SIMPLE; }
    
    // Devuelve la bounding box alineada en espacio local
    NxBox               CalcBBox        (void) const;

    // Devuelve el volumen total del modelo (suma de todos los volumentes de colision)
    float               CalcVolume      (void) const;
    float               CalcVolume      (int nPartIdx) const;


    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    CPhysicModelGeneric
    //////////////////////////////////////////////////////////////////////////

    // Modelo de colision
    int                 CreateShape     (NxShapeType shapeType);
    void                DeleteShape     (int nShapeIdx);
    int                 GetNumShapes    (void) const;
    const NxShapeDesc&  GetShape        (int nShapeIdx) const;
    NxShapeDesc&        EditShape       (int nShapeIdx);
    // - Funcion adicional para añadir un objeto 3D como modelo de colision
    //void                AddShapeObj3D   (CGLObject* pObj3D);
    
    // Propiedades físicas:
    // - Masa / densidad (una propiedad determina la otra)
    void                SetDensity      (float fDensity);
    float               GetDensity      (void) const;
    void                SetMass         (float fMass);
    float               GetMass         (void) const;

    // Acceso al objetos PhysX
    NxActorDesc&        GetNxActorDesc  (void)      { return m_ActorDesc; }
    
protected:
    friend  class   IPhysicModel;

    // Construccion / Destruccion
    CPhysicModelSimple  (void);
    virtual ~CPhysicModelSimple (void);

private:
    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    // Objetos de PhysX
    NxActorDesc     m_ActorDesc;
    NxBodyDesc      m_BodyDesc;
};

#endif  // _PHYSIC_MODEL_SIMPLE_H_
