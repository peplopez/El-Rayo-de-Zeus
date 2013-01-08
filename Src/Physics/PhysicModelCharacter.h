//*****************************************************************************
// Creacion  : 21/05/2005
// Contenido : Modelo físico para characters
//
//*****************************************************************************
#ifndef _PHYSIC_MODEL_CHARACTER_H_
#define _PHYSIC_MODEL_CHARACTER_H_

#include "IPhysicModel.h"

class   CGLObject;

//////////////////////////////////////////////////////////////////////////////
//  CPhysicModelCharacter
//
//
//////////////////////////////////////////////////////////////////////////////
class   CPhysicModelCharacter  :  public IPhysicModel
{
public:

    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    IPhysicModel
    //////////////////////////////////////////////////////////////////////////

    // Tipo de modelo
    EType               GetType             (void) const    { return MODEL_CHARACTER; }

    // Devuelve la bounding box alineada en espacio local
    NxBox               CalcBBox            (void) const;

    // Devuelve el volumen total del modelo (suma de todos los volumentes de colision)
    float               CalcVolume          (void) const;
    float               CalcVolume          (int nPartIdx) const;

    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ:    CPhysicModelCharacter
    //////////////////////////////////////////////////////////////////////////

    // - Definir un modelo de tipo caja
    NxControllerType            GetControllerType   () const;
    void                        SetControllerType   (NxControllerType eType);
    NxBoxControllerDesc&        EditBoxParams       ();
    NxCapsuleControllerDesc&    EditCapsuleParams   ();

    // Acceso al objeto PhysX
    const NxControllerDesc&     GetNxControllerDesc (void) const;
    
protected:
    friend  class   IPhysicModel;

    // Construccion / Destruccion
    CPhysicModelCharacter  (void);
    virtual ~CPhysicModelCharacter (void);

private:
    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    NxControllerType        m_eType;

    // Objetos de PhysX
    NxBoxControllerDesc     m_BoxDesc;
    NxCapsuleControllerDesc m_CapsuleDesc;
};

#endif  // _PHYSIC_MODEL_Character_H_
