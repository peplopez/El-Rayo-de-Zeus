#include "Globals.h"

namespace Globals
{
    // User Allocator
    NxUserAllocator*          g_pUserAllocator = NULL;

    // Simulation globals
    bool                    g_bPause = false;
    bool                    g_bShadows = false;
    bool                    g_bDebugWireframeMode = false;

    CPhysicModelSimple*     g_pModelTerrain = NULL;
    CPhysicModelSimple*     g_pModelBox = NULL;
    CPhysicModelGeneric*    g_pModelGeneric = NULL;


    // Camera globals
    float                   g_fCameraAspectRatio      = 1.0f;
    NxReal                  g_fCameraSpeed            = 0.15f;
    NxVec3                  g_vCameraPos              = NxVec3(8,15,8);
    NxVec3                  g_vCameraForward          = NxVec3(1,-1,1);
    NxVec3                  g_vCameraRight            = NxVec3(-1,0,0);

    // Variables auxiliares
    NxReal                  g_fWaterLevel             = 7.f;
};