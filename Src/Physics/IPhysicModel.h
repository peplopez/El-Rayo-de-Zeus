//*****************************************************************************
// Creacion  : 14/05/2004
// Contenido : Interfaz de definicion de modelos fisicos
//
//*****************************************************************************
#ifndef _IPHYSIC_MODEL_H_
#define _IPHYSIC_MODEL_H_

#include "PhysicGlobals.h"

//////////////////////////////////////////////////////////////////////////////
//  IPhysicModel
//
//
//////////////////////////////////////////////////////////////////////////////
class   IPhysicModel 
{
public:
    // DEFINES ///////////////////////////////////////////////////////////////

    // Enumerado con los posibles tipos de modelos
    enum    EType
    {
        MODEL_UNKNOWN   = -1,
        MODEL_SIMPLE    = 0,
        MODEL_GENERIC,
        MODEL_CLOTH,
        MODEL_RAGDOLL,
        MODEL_VEHICLE,
        MODEL_CHARACTER
    };

public:
    // CONSTRUCCION / DESTRUCCION ////////////////////////////////////////////
    virtual ~IPhysicModel (void) {};


    // OPERACIONES ///////////////////////////////////////////////////////////

    // Factoria
    static  IPhysicModel*   Factory             (EType eType);

    // Tipo de modelo
    virtual EType           GetType             (void) const = 0;

    // Devuelve la bounding box alineada en espacio local
    virtual NxBox           CalcBBox            (void) const = 0;

    // Devuelve el volumen total del modelo (suma de todos los volumentes de colision)
    virtual float           CalcVolume          (void) const = 0;
    virtual float           CalcVolume          (int nPartIdx) const = 0;


};


#endif // _IPHYSIC_MODEL_H_
