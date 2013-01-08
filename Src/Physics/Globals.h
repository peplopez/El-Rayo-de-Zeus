//*****************************************************************************
// Creacion  : 10/05/2005
// Contenido : Variables globales de la aplicación
//
//*****************************************************************************
#ifndef _MY_GLOBALS_H_
#define _MY_GLOBALS_H_

#include "NxCooking.h"
#include "NxPhysics.h"
#include "NxController.h"

#include <vector>

class   CPhysicModelSimple;
class   CPhysicModelGeneric;

#define VALOR_INVALIDO_UINT     -1


namespace Globals
{
    extern NxUserAllocator*       g_pUserAllocator;

    // Simulation globals
    extern bool                 g_bPause;
    extern bool                 g_bShadows;
    extern bool                 g_bDebugWireframeMode;

    // Modelos fisicos
    extern CPhysicModelSimple*  g_pModelTerrain;
    extern CPhysicModelSimple*  g_pModelBox;
    extern CPhysicModelGeneric* g_pModelGeneric;

    // Camera globals
    extern float                g_fCameraAspectRatio;
    extern NxReal               g_fCameraSpeed;
    extern NxVec3               g_vCameraPos;
    extern NxVec3               g_vCameraForward;
    extern NxVec3               g_vCameraRight;

    // Variables auxiliares
    extern NxReal               g_fWaterLevel;
};

#endif // _MY_GLOBALS_H_