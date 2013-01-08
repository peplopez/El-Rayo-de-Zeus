//*****************************************************************************
// Creacion  : 10/05/2005
// Contenido : Definiciones globales del motor de fisica
//
//*****************************************************************************
#ifndef _PHYSIC_GLOBALS_H_
#define _PHYSIC_GLOBALS_H_

#include "NxCooking.h"
#include "NxPhysics.h"
#include "NxController.h"
#include "NxBoxController.h"
#include "NxCapsuleController.h"
#include "NxControllerManager.h"
#include <stdio.h>
#include <vector>
#include "Stream.h"


typedef unsigned int    uint;

static const NxVec3     CERO = NxVec3 (0,0,0);

#define VALOR_INVALIDO_UINT     -1


#endif // _PHYSIC_GLOBALS_H_