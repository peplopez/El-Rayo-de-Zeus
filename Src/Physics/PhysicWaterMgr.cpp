#include "PhysicWaterMgr.h"
#include "PhysicScene.h"
#include "PhysicObj.h"
#include "PhysicUtils.h"
#include "IPhysicModel.h"

// NOTA: La densidad del agua es Kg/m^3 pero tenemos que expresarla en Kg/cm^3
const float CPhysicWaterMgr::DFT_WATER_DENSITY          = 1.f;
const float CPhysicWaterMgr::DFT_WATER_FRICTION_LINEAR  = 0.5f;
const float CPhysicWaterMgr::DFT_WATER_FRICTION_ANGULAR = 0.5f;

// Numero de divisiones para discretizar el volumen de una box
#define WATER_VOL_DIVISION  4
static  NxVec3  g_points[WATER_VOL_DIVISION * WATER_VOL_DIVISION * WATER_VOL_DIVISION];

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicWaterMgr::CPhysicWaterMgr (void)
{
    m_bInitialized = false;
    m_bActive = true;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicWaterMgr::~CPhysicWaterMgr (void)
{
    assert( !m_bInitialized );
    if ( m_bInitialized )
    {
        Shutdown();
    }
}

//-----------------------------------------------------------------------------
//  Init
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::Init (const TInitInfo& initInfo)
{
    assert( !m_bInitialized );

    m_InitInfo = initInfo;
    assert( (m_InitInfo.pScene != NULL) );

    m_fWaterLevel = initInfo.fWaterLevel;

    m_bInitialized = true;
}

//-----------------------------------------------------------------------------
//  Shutdown
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::Shutdown (void)
{
    assert( m_bInitialized );

    // Eliminamos toda la informacion de objetos en el agua
    m_aObjsInWater.clear ();

    // Destruimos los vectores auxiliares
    m_vUnderWater.clear ();
    m_vOverWater.clear ();
    m_vBoxPoints.clear ();

    m_bInitialized = false;
}

//-----------------------------------------------------------------------------
//  SetActive
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::SetActive (bool bValue)
{
    m_bActive = bValue;
}

//-----------------------------------------------------------------------------
//  IsActive
//-----------------------------------------------------------------------------
bool    CPhysicWaterMgr::IsActive (void) const
{
    return m_bActive;
}

//-----------------------------------------------------------------------------
//  SetWaterLevel
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::SetWaterLevel (float fHeight)
{
    m_fWaterLevel = fHeight;
}

//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::Update (float fDeltaTime)
{
    if ( !m_bActive )   return;

    // Lets process every object in the scene
    uint    uNumObjs = m_InitInfo.pScene->GetNumPhysicObjs ();
    for (uint i = 0 ; i < uNumObjs; i++)
    {
        CPhysicObj* pPhysicObj = (CPhysicObj*) m_InitInfo.pScene->GetPhysicObj (i);
        if ( pPhysicObj->IsActivated() && pPhysicObj->IsDynamic() )
        {
            // Process the physic object interaction with the water
            ProcessPhysicObj (pPhysicObj, fDeltaTime);
        }
    }
}

//-----------------------------------------------------------------------------
//  GetObjSituation
//-----------------------------------------------------------------------------
CPhysicWaterMgr::EObjSituation  CPhysicWaterMgr::GetObjSituation (IPhysicObj* pPhysicObj) const
{
    uint uIndex = FindObjInWater ((CPhysicObj*)pPhysicObj);
    if ( uIndex != VALOR_INVALIDO_UINT )
    {
        return m_aObjsInWater[uIndex].eSituation;
    }
    else
    {
        return OBJ_OUT_OF_WATER;
    }
}

//-----------------------------------------------------------------------------
//  AddDirPoint
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::AddDirPoint (const NxVec3& vPoint)
{
    // TODO: Completar...
}

//-----------------------------------------------------------------------------
//  GetNumDirPoints
//-----------------------------------------------------------------------------
uint    CPhysicWaterMgr::GetNumDirPoints (void) const
{
    // TODO: Completar...
    return 0;
}

//-----------------------------------------------------------------------------
//  GetDirPoint
//-----------------------------------------------------------------------------
const NxVec3*   CPhysicWaterMgr::GetDirPoint (uint uIndex) const
{
    // TODO: Completar...
    return NULL;
}

//-----------------------------------------------------------------------------
//  ClearDirPoints
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::ClearDirPoints (void)
{
    // TODO: Completar
}

//-----------------------------------------------------------------------------
//  SetWaterDragForce
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::SetWaterDragForce (float fDragForce)
{
    // TODO: Completar
}

//-----------------------------------------------------------------------------
//  GetWaterDragForce
//-----------------------------------------------------------------------------
float   CPhysicWaterMgr::GetWaterDragForce (void) const
{
    // TODO: Completar
    return 0.f;
}

//-----------------------------------------------------------------------------
//  OnDelPhysicObj
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::OnDelPhysicObj (IPhysicObj* pObjDeleted)
{
    // Comprobamos si lo tenemos en la lista de objetos en el agua
    uint uIndex = FindObjInWater ((CPhysicObj*)pObjDeleted);
    if ( uIndex != VALOR_INVALIDO_UINT )
    {
        // NOTA: Como no nos importa el orden de los elementos, movemos el ultimo
        //   a la posicion de este
        if ( uIndex < (m_aObjsInWater.size()-1) )
        {
            m_aObjsInWater[uIndex] = m_aObjsInWater.back();
        }
        m_aObjsInWater.pop_back();
    }
}

//-----------------------------------------------------------------------------
//  FindObjInWater
//-----------------------------------------------------------------------------
uint    CPhysicWaterMgr::FindObjInWater (CPhysicObj* pPhysicObj) const
{
    uint uIndex = VALOR_INVALIDO_UINT;
    bool bFound = false;
    for ( uint i = 0; (i < m_aObjsInWater.size()) && !bFound; i++ )
    {
        if ( m_aObjsInWater[i].pPhysicObj == pPhysicObj )
        {
            uIndex = i;
            bFound = true;
        }
    }
    return uIndex;
}

//-----------------------------------------------------------------------------
//  AddObjInWater
//-----------------------------------------------------------------------------
uint    CPhysicWaterMgr::AddObjInWater (CPhysicObj* pPhysicObj)
{
    assert( FindObjInWater(pPhysicObj) == VALOR_INVALIDO_UINT );

    TObjInWater tNew;
    tNew.eSituation = OBJ_OUT_OF_WATER;
    tNew.pPhysicObj = pPhysicObj;
    tNew.fObjVolume = 0.f;
    m_aObjsInWater.push_back (tNew);

    // Reducir velocidad lineal de todo el objeto al impactar en el agua
    NxVec3 vVel = pPhysicObj->GetLinearVelocity();
    pPhysicObj->SetLinearVelocity (vVel * 0.5f);

    return (m_aObjsInWater.size()-1);
}

//-----------------------------------------------------------------------------
//  DelObjInWater
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::DelObjInWater (uint uIndex)
{
    assert( uIndex < m_aObjsInWater.size() );

    // Restauramos los valores de friccion al valor por defecto 
    // NOTA: estamos suponiendo que los objetos fuera del agua no tienen ninguna friccion
    m_aObjsInWater[uIndex].pPhysicObj->SetLinearDamping (0.f);
    m_aObjsInWater[uIndex].pPhysicObj->SetAngularDamping (0.f);

    // Eliminamos el elemento del vector de objetos sumergidos
    if ( uIndex < (m_aObjsInWater.size()-1) )
    {
        m_aObjsInWater[uIndex] = m_aObjsInWater.back();
    }
    m_aObjsInWater.pop_back();
}

//-----------------------------------------------------------------------------
//  ProcessPhysicObj
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::ProcessPhysicObj (CPhysicObj* pPhysicObj, float fDeltaTime)
{
    // Buscamos la el objeto en nuestro pool por si ya estaba en el agua
    uint    uPoolIdx = FindObjInWater (pPhysicObj);

    // NOTA: Aqui comprobamos la bbox del objeto fisico, y miramos si alguno de sus
    //   vertices esta por debajo del nivel del agua
    if ( pPhysicObj->GetWorldBounds().min.y < m_fWaterLevel )
    {
        // Si no lo tenemos en el pool, lo añadimos
        if ( uPoolIdx == VALOR_INVALIDO_UINT )
        {
            uPoolIdx = AddObjInWater (pPhysicObj);
        }
        TObjInWater* pObjInWater = &(m_aObjsInWater[uPoolIdx]);

        // Calculamos el volumen sumergido
        TSumergedVolume tVolume;
        bool bInWater = CalcSumergedVolume (pObjInWater, &tVolume);

        // Si no esta totalmente fuera del agua, le aplicamos la fuerza que corresponda
        if ( bInWater )
        {
            ApplyWaterForces (pObjInWater, tVolume, fDeltaTime);
        }
    }
    else
    {
        // Si esta fuera del agua y lo tenemos en el pool, lo eliminamos
        if ( uPoolIdx != VALOR_INVALIDO_UINT )
        {
            DelObjInWater (uPoolIdx);
        }
    }
}

//-----------------------------------------------------------------------------
//  ApplyWaterForces
//-----------------------------------------------------------------------------
void    CPhysicWaterMgr::ApplyWaterForces (TObjInWater* pObjInWater, const TSumergedVolume& tSumergedVol, float fDeltaTime)
{
    CPhysicObj* pPhysicObj = pObjInWater->pPhysicObj;
    // Vamos aplicando la fuerza a cada parte sumergida
    for ( uint i = 0; i < tSumergedVol.aVolInfo.size(); i++ )
    {
        const TSumergedVolume::TInfo&   volInfo = tSumergedVol.aVolInfo[i];

        NxVec3  vForce = CalcBuoyancyForce (volInfo.fVolume);

        pPhysicObj->ApplyForceAtPart (vForce, volInfo.vCoM, volInfo.uPartIdx);
        // Aplicamos friccion lineal y angular en funcion de la profundidad de hundimiento
        pPhysicObj->SetLinearDamping (volInfo.fUnderWaterDepth * m_InitInfo.fFrictionLinear, volInfo.uPartIdx);
        pPhysicObj->SetAngularDamping (volInfo.fUnderWaterDepth * m_InitInfo.fFrictionAngular, volInfo.uPartIdx);
    }

    // NOTA: Podriamos simular la expansion del volumen desalojado, aplicando un fuerza espansiva
    //   a los objetos flotantes cercanos, en funcion de la diferencia de volumen sumergido de este
    //   objeto con respecto a su estado en el frame anterior
}

//-----------------------------------------------------------------------------
//  CalcSumergedVolume
//-----------------------------------------------------------------------------
bool    CPhysicWaterMgr::CalcSumergedVolume (TObjInWater* pObjInWater, TSumergedVolume* pResult) 
{
    // NOTA: Vamos a procesar todas las partes del objeto, y calcularemos para cada uno el 
    //   volumen de agua que desplazan, junto con el centro de masas del volumen sumergido
    int     nNumParts = pObjInWater->pPhysicObj->GetNumParts ();
    for (int i = 0; i < nNumParts; i++)
    {
        // NOTA: Aproximamos el volumen por su bbox no alineada.
        // REVIEW: Como no tenemos calculada la oriented box del actor, vamos a hacer una prueba
        //	 asumiendo que tenemos actores de 1 sola shape.
        //   En caso de tener actores con multiples shapes, podríamos simplemente repetir el calculo
        //	 de volumen sumergido por cada shape.
        NxBox	tBox = PhysicUtils::CalcOBBox( *(pObjInWater->pPhysicObj->GetActor(i)->getShapes()[0]) );
		
        // Calculamos la interseccion entre la caja y el plano
        m_vBoxPoints.clear();
        float fUnderWaterDepth = CalcBoxUnderWater (tBox, m_vBoxPoints);
        if ( fUnderWaterDepth > 0.f )
        {
            // Si el objeto esta sumergido, calculamos el volumen de agua desplazada, y el CoM
            //  de la parte donde aplicar la fuerza de flotacion
            float   fTotalVolume = pObjInWater->pPhysicObj->GetPhysicModel()->CalcVolume (i);
            NxVec3  vCoM = tBox.center;
            
            TSumergedVolume::TInfo  volInfo;
            volInfo.uPartIdx         = i;
            volInfo.fUnderWaterDepth = fUnderWaterDepth;
            volInfo.fVolume          = fUnderWaterDepth * fTotalVolume;
            // NOTA: El centro de masas, para una caja parcialmente sumergida la calculamos como
            //   promedio de todos los puntos calculados en la intesecion con el plano, y para
            //   evitar problemas de precision y saltos, lo vamos a promediar con el CoM global del objeto
            //   para que el comportamiento de flotacion sea siempre suave
            if ( fUnderWaterDepth < 1.f )
                volInfo.vCoM =  0.5f * vCoM + 0.5f * CalcCOM (pObjInWater, m_vBoxPoints);
            else
                volInfo.vCoM = vCoM;

            pResult->aVolInfo.push_back (volInfo);
        }
    }

    // Si tenemos informacion de volumenes es que estamos en el agua
    return (pResult->aVolInfo.size() > 0);
}

//-----------------------------------------------------------------------------
//  CalcBoxUnderWater
//-----------------------------------------------------------------------------
float   CPhysicWaterMgr::CalcBoxUnderWater (const NxBox& tBox, NxArray<NxVec3>& vResult)
{
    float       fUnderWaterDepth;

    // NOTA: Vamos a discretizar el volumen en la box en N divisiones a lo largo de sus ejes
    NxVec3  vBase = tBox.center - tBox.extents.x * tBox.rot.getRow(0) - 
                        tBox.extents.y * tBox.rot.getRow(1) - tBox.extents.z * tBox.rot.getRow(2);
    NxVec3  vIncI = (2.f * tBox.extents.x / float(WATER_VOL_DIVISION-1)) * tBox.rot.getRow(0);
    NxVec3  vIncJ = (2.f * tBox.extents.y / float(WATER_VOL_DIVISION-1)) * tBox.rot.getRow(1);
    NxVec3  vIncK = (2.f * tBox.extents.z / float(WATER_VOL_DIVISION-1)) * tBox.rot.getRow(2);
    int         nVolDivSqrd = WATER_VOL_DIVISION * WATER_VOL_DIVISION;
    for (uint i = 0; i < WATER_VOL_DIVISION; i++)
    {
        int         nIndexI = i * nVolDivSqrd;
        NxVec3  vBase_DespI = vBase + float(i) * vIncI;
        for (uint j = 0; j < WATER_VOL_DIVISION; j++)
        {
            int         nIndexIJ = nIndexI + j * WATER_VOL_DIVISION;
            NxVec3  vBase_DesplIJ = vBase_DespI + float(j) * vIncJ;
            for (uint k = 0; k < WATER_VOL_DIVISION; k++)
            {
                g_points[nIndexIJ + k] = vBase_DesplIJ + float(k) * vIncK;
            }
        }
    }

    // Clasificamos primero los puntos segun esten bajo el agua o encima de ella
    uint            uTopVertex = 0;
    uint            uBottomVertex = 0;
    uint            uNumPoints = WATER_VOL_DIVISION * WATER_VOL_DIVISION * WATER_VOL_DIVISION;
    m_vUnderWater.clear();
    m_vOverWater.clear();
    for ( uint i = 0; i < uNumPoints; i++ )
    {
        float fY = g_points[i].y;
        // Comprobamos si esta bajo el agua
        if ( fY < m_fWaterLevel )
            m_vUnderWater.push_back(i);
        else  
            m_vOverWater.push_back(i);
        // Comprobamos si es minimo o maximo
        if ( fY < g_points[uBottomVertex].y )   uBottomVertex = i;
        if ( fY > g_points[uTopVertex].y )      uTopVertex = i;
    }

    // Si no queda ningun punto sobre el agua, la caja esta sumergida
    // NOTA: No hace falta que metamos ningun punto en el vector resultado
    if ( m_vOverWater.empty() )
    {
        fUnderWaterDepth = 1.f;
    }
    else
    // Si no hay ningun punto bajo el agua, la caja esta fuera del agua
    if ( m_vUnderWater.empty() )
    {
        // Notificamos en el resultado que el 0% de la caja esta sumergida
        fUnderWaterDepth = 0.f;
    }
    else
    {
        // Si hay puntoa arriba y abajo la caja estara flotando
        // Metemos los puntos debajo del agua en el resultado
        vResult.reserve (m_vUnderWater.size());
        for (uint i = 0; i < m_vUnderWater.size(); i++)
        {
            vResult.push_back (g_points[m_vUnderWater[i]]);
        }

        // El porcentaje de penetracion en el agua lo calculamos en base al punto de maxima altura
        //  y el de menor altura
        float   fMaxY = g_points[uTopVertex].y;
        float   fMinY = g_points[uBottomVertex].y;
        fUnderWaterDepth = (m_fWaterLevel - fMinY) / (fMaxY - fMinY);
    }

    return fUnderWaterDepth;
}

//-----------------------------------------------------------------------------
//  CalcBoxVolume
//-----------------------------------------------------------------------------
float   CPhysicWaterMgr::CalcBoxVolume (const NxBox& tBox) const
{
    return (2.f * tBox.GetExtents().x) * (2.f * tBox.GetExtents().y) * (2.f * tBox.GetExtents().z);
}

//-----------------------------------------------------------------------------
//  CalcCOM
//-----------------------------------------------------------------------------
NxVec3  CPhysicWaterMgr::CalcCOM (TObjInWater* pObjInWater, const NxArray<NxVec3>& vPoints) const
{
    assert( vPoints.size() > 0 );
    // Vamos a calcular el punto medio, que nos dara un punto dentro del volumen convexo
    NxVec3  vAvgPoint(0,0,0);
    float   fNumPointsInv = 1.f / vPoints.size();
    for (uint i = 0; i < vPoints.size(); i++)
    {
        vAvgPoint += fNumPointsInv * vPoints[i];
    }
    return vAvgPoint;
}

//-----------------------------------------------------------------------------
//  CalcBuoyancyForce
//-----------------------------------------------------------------------------
NxVec3  CPhysicWaterMgr::CalcBuoyancyForce (float fVolume) const
{
    // NOTA: La gravedad esta en cm/s^2 y el volumen en cm^3, asi que la densidad debe ser Kg/cm^3
    return m_InitInfo.fWaterDensity * fVolume * (- m_InitInfo.pScene->GetGravity());
}
