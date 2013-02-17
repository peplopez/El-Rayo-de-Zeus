/**
@file ErrorManager.cpp

Contiene la implementación del gestor de errores para PhysX.

@see Physics::CErrorManager

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "ErrorManager.h"

#include <string>
#include <iostream>

using namespace Physics;
using namespace physx;
using namespace std;

//---------------------------------------------------------

CErrorManager::CErrorManager() : PxErrorCallback() 
{ 

}

//---------------------------------------------------------

CErrorManager::~CErrorManager() 
{ 

}

//---------------------------------------------------------

void CErrorManager::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	string codeStr;

	switch(code) {
	case PxErrorCode::eNO_ERROR: codeStr = "NO_ERROR"; break;
	case PxErrorCode::eDEBUG_INFO: codeStr = "DEBUG_INFO"; break;
	case PxErrorCode::eDEBUG_WARNING: codeStr = "DEBUG_WARNING"; break;
	case PxErrorCode::eINVALID_PARAMETER: codeStr = "INVALID_PARAMETER"; break;
	case PxErrorCode::eINVALID_OPERATION: codeStr = "INVALID_OPERATION"; break;
	case PxErrorCode::eOUT_OF_MEMORY: codeStr = "OUT_OF_MEMORY"; break;
	case PxErrorCode::eINTERNAL_ERROR: codeStr = "INTERNAL_ERROR"; break;
	case PxErrorCode::eABORT: codeStr = "ABORT"; break;
	case PxErrorCode::ePERF_WARNING: codeStr = "PERF_WARNING"; break;
	case PxErrorCode::eEXCEPTION_ON_STARTUP: codeStr = "EXCEPTION_ON_STARTUP"; break;
	}

	cout << "File: " << string(file) << " Line: " << line << " Code: " << codeStr 
		 << " " << string(message) << endl; 
}

//---------------------------------------------------------