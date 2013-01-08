//*****************************************************************************
// Creacion  : 21/05/2005
// Contenido : Clase principal del engine. Singleton
//
//*****************************************************************************
#ifndef _PHYSIC_ENGINE_H_
#define _PHYSIC_ENGINE_H_

#include "PhysicGlobals.h"
#include "PhysicScene.h"

//////////////////////////////////////////////////////////////////////////////
//    CPhysicEngine
//
//
//////////////////////////////////////////////////////////////////////////////
class    CPhysicEngine : public NxUserOutputStream
{
public:
    // Gestion del singleton
    static  void            Init                    (void);
    static  void            ShutDown                (void);
    static  CPhysicEngine&  Ref                     (void);

    // Gestion de escenas físicas
    CPhysicScene*           CreateScene             (const CPhysicScene::TInitInfo& initInfo);
    void                    DestroyScene            (CPhysicScene* pScene);

    //!! GetNumScenes y GetScene estaban sin implementar
	NxU32                   GetNumScenes            (void) const { return m_aScenes.size(); };
	CPhysicScene*           GetScene                (int nIndex) 
	{ 
		assert( nIndex < (int) m_aScenes.size() ); 
		return m_aScenes[nIndex];  
	};

    // Acceso al SDK de PhysX
    NxPhysicsSDK*           GetNxPhysicsSDK         (void)  { return m_pPhysicsSDK; };

    // Acceso al gestor de controller de PhysX
    NxControllerManager*    GetNxControllerMgr      (void)  { return m_pControllerMgr; };

    //////////////////////////////////////////////////////////////////////////
    // *** INTERFAZ: NxUserOutputStream 
    //////////////////////////////////////////////////////////////////////////

    void                    reportError             (NxErrorCode code, const char * message, const char *file, int line)
    {
		printf( "NXERROR: File:%s Line:%d : %s\n", file, line, message );
    };

    NxAssertResponse        reportAssertViolation   (const char * message, const char *file, int line)
    {
		printf( "NXASSERT: File:%s Line:%d : %s\n", file, line, message );
        return  NX_AR_CONTINUE;
    };
    void                    print                   (const char * message)
    {
        printf( message );
		printf( "\n" );
    }

protected:
    // Como es un singleron, los contructores son protegidos
    CPhysicEngine   (void);
    ~CPhysicEngine  (void);

private:
    // DEFINES ///////////////////////////////////////////////////////////////

    // FUNCIONES PRIVADAS ////////////////////////////////////////////////////

private:
    // DATOS MIEMBRO /////////////////////////////////////////////////////////

    // Interfaz principal del motor PhysX
    NxPhysicsSDK*               m_pPhysicsSDK;

    // Gestor de Controllers de PhysX
    NxControllerManager*        m_pControllerMgr;

    // Vector de escenas    
    std::vector<CPhysicScene*>  m_aScenes;

    // Puntero a la instancia unica del engine
    static  CPhysicEngine*      m_pInstance;
};

#endif // _PHYSIC_ENGINE_H_