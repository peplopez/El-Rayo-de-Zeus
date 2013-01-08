#include "PhysicObj.h"
#include "IPhysicModel.h"
#include "PhysicObjSimple.h"
#include "PhysicObjGeneric.h"
#include "PhysicObjCharacter.h"
#include "PhysicUtils.h"

//-----------------------------------------------------------------------------
//  Factory
//-----------------------------------------------------------------------------
IPhysicObj* CPhysicObj::Factory (CPhysicScene* pScene, const IPhysicModel* pPhysicModel)
{
    assert( pPhysicModel );
    if ( pPhysicModel == NULL ) return NULL;

    IPhysicObj* pResult = NULL;
    
    switch ( pPhysicModel->GetType() )
    {
        case IPhysicModel::MODEL_SIMPLE:
            pResult = new CPhysicObjSimple (pScene, (const CPhysicModelSimple*) pPhysicModel);
        break;
        case IPhysicModel::MODEL_GENERIC:
            pResult = new CPhysicObjGeneric (pScene, (const CPhysicModelGeneric*) pPhysicModel);
        break;
        case IPhysicModel::MODEL_CHARACTER:
			pResult = new CPhysicObjCharacter (pScene, (const CPhysicModelCharacter*) pPhysicModel);
        break;
        default:
        {
            assert(0);
        }
    }

    return pResult;
}

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicObj::CPhysicObj (void)
{
    m_bActivated = false;
	//!!
	userData = NULL;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicObj::~CPhysicObj (void)
{
    // El objeto deberia haber sido desactivado
    assert( m_bActivated == false );
	//!!
	userData = NULL;
}

//-----------------------------------------------------------------------------
//  InitObject
//-----------------------------------------------------------------------------
void    CPhysicObj::InitObject (const TInitInfo& initInfo)
{
    m_InitInfo = initInfo;
}

//-----------------------------------------------------------------------------
//  Activate
//-----------------------------------------------------------------------------
bool    CPhysicObj::Activate (void)
{
    assert( !m_bActivated );

    // NOTA: Cada clase derivada debera aqui crear sus actores y joints necesarios

    m_bActivated = true;

    return true;
}

//-----------------------------------------------------------------------------
//  Deactivate
//-----------------------------------------------------------------------------
void    CPhysicObj::Deactivate (void)
{
    // NOTA: Cada clase derivada debera liberar aqui sus actores, joints, etc...

    m_bActivated = false;
}

//-----------------------------------------------------------------------------
//  IsMoving
//-----------------------------------------------------------------------------
bool    CPhysicObj::IsMoving (void) const
{
    // NOTA: No implementado
    assert(0);
    return false;
}

//-----------------------------------------------------------------------------
//  GetWorldPosition
//-----------------------------------------------------------------------------
NxVec3  CPhysicObj::GetWorldPosition (void) const
{
    assert( m_bActivated );

    int     nNumParts = GetNumParts();
    // NOTA: Siempre tendremos que tener al menos 1 parte. 
    NxVec3  vResult = GetWorldPosition( 0 );
    // El resto de posiciones las agregamos
    for (int i = 1; i < nNumParts; i++)
    {
        vResult += GetWorldPosition( i );
    }

    return vResult / float(nNumParts);
}

//-----------------------------------------------------------------------------
//  GetWorldPosition
//-----------------------------------------------------------------------------
NxVec3  CPhysicObj::GetWorldPosition (int nPartIdx) const
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );

    return  GetActor(nPartIdx)->getGlobalPosition();
}

//-----------------------------------------------------------------------------
//  SetWorldPosition
//-----------------------------------------------------------------------------
//!!
void CPhysicObj::SetWorldPosition (const NxVec3 &vPos)
{
	NxVec3 displ = vPos - GetWorldPosition();
	for (int i=0; i<GetNumParts(); i++) {
		SetWorldPosition(i, GetWorldPosition(i) + displ);
	}
}

//-----------------------------------------------------------------------------
//  SetWorldPosition
//-----------------------------------------------------------------------------
//!!
void CPhysicObj::SetWorldPosition (int nPartIdx, const NxVec3 &vPos)
{
	assert( m_bActivated && (nPartIdx < GetNumParts()) );

	GetActor(nPartIdx)->setGlobalPosition(vPos);
}

//-----------------------------------------------------------------------------
//  GetWorldOrientation
//-----------------------------------------------------------------------------
//!!
NxMat33	CPhysicObj::GetWorldOrientation	() const
{
	assert( m_bActivated );

	// NOTA: Siempre tendremos que tener al menos 1 parte.
	return GetWorldOrientation(0);
}

//-----------------------------------------------------------------------------
//  GetWorldOrientation
//-----------------------------------------------------------------------------
//!!
NxMat33	CPhysicObj::GetWorldOrientation	(int nPartIdx) const
{
	assert( m_bActivated && (nPartIdx < GetNumParts()) );

	return  GetActor(nPartIdx)->getGlobalOrientation();
}

//-----------------------------------------------------------------------------
//  SetWorldOrientation
//-----------------------------------------------------------------------------
//!!
void CPhysicObj::SetWorldOrientation (const NxMat33 &mOrient)
{
	assert ( m_bActivated );

	for (int i=0; i<GetNumParts(); i++) {
		SetWorldOrientation(mOrient);
	}
}

//-----------------------------------------------------------------------------
//  SetWorldOrientation
//-----------------------------------------------------------------------------
//!!
void CPhysicObj::SetWorldOrientation (int nPartIdx, const NxMat33 &mOrient)
{
	assert( m_bActivated && (nPartIdx < GetNumParts()) );

	GetActor(nPartIdx)->setGlobalOrientation(mOrient);
}

//-----------------------------------------------------------------------------
//  GetWorldBounds
//-----------------------------------------------------------------------------
NxBounds3   CPhysicObj::GetWorldBounds (void) const
{
    assert( m_bActivated );

    int         nNumParts = GetNumParts();
    // NOTA: Siempre tendremos que tener al menos 1 parte. 
    NxBounds3   result = GetWorldBounds (0);
    // El resto de bounds los combinamos con el resultado
    for (int i = 1; i < nNumParts; i++)
    {
        result.combine (GetWorldBounds(i));
    }

    return result;
}

//-----------------------------------------------------------------------------
//  GetWorldBounds
//-----------------------------------------------------------------------------
NxBounds3   CPhysicObj::GetWorldBounds (int nPartIdx) const
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );

    const NxActor*  pActor = GetActor(nPartIdx);
    // Combinamos los bounds de todas las shapes
    NxBounds3   result;
    NxBounds3   shapeBounds;

    uint        nNumShapes = pActor->getNbShapes();
    assert( nNumShapes > 0 );

    NxShape* const* pShapes = pActor->getShapes(); 
    pShapes[0]->getWorldBounds(result);
    for (uint i = 1; i < nNumShapes; i++)
    {
        pShapes[0]->getWorldBounds (shapeBounds);
        result.combine (shapeBounds);
    }

    return result;
}

//-----------------------------------------------------------------------------
//  SetMass
//-----------------------------------------------------------------------------
void    CPhysicObj::SetMass (int nPartIdx, float fMass)
{
    assert( nPartIdx < GetNumParts() );
    NxActor*    pActor = GetActor(nPartIdx);
    pActor->setMass( fMass );
}

//-----------------------------------------------------------------------------
//  GetMass
//-----------------------------------------------------------------------------
float   CPhysicObj::GetMass (int nPartIdx) const
{
    assert( nPartIdx < GetNumParts() );
    return GetActor(nPartIdx)->getMass();
}

//-----------------------------------------------------------------------------
//  SetTotalMass
//-----------------------------------------------------------------------------
void    CPhysicObj::SetTotalMass (float fMass)
{
    // NOTA: Distribuimos la masa total entre las partes, repartiendola de forma 
    //   proporcional al volumen 

    // Calculamos el volumen total actual
    float fCurTotalVol = m_InitInfo.pPhysicModel->CalcVolume();
    // Necesitamos tener volumen
    assert( fCurTotalVol > 0.f );

    // Distribuimos la masa total porcentualmente entre las partes
    int nNumParts = GetNumParts();
    for (int i = 0; i < nNumParts; i++)
    {
        SetMass (i, fMass * (m_InitInfo.pPhysicModel->CalcVolume(i) / fCurTotalVol));
    }
}

//-----------------------------------------------------------------------------
//  SetDensity
//-----------------------------------------------------------------------------
void    CPhysicObj::SetDensity (int nPartIdx, float fDensity)
{
    assert( nPartIdx < GetNumParts() );
    NxActor*    pActor = GetActor(nPartIdx);
    pActor->setMass( m_InitInfo.pPhysicModel->CalcVolume() * fDensity );
}

//-----------------------------------------------------------------------------
//  GetDensity
//-----------------------------------------------------------------------------
float   CPhysicObj::GetDensity (int nPartIdx) const
{
    assert( nPartIdx < GetNumParts() );
    return GetActor(nPartIdx)->getMass() / m_InitInfo.pPhysicModel->CalcVolume();
}

//-----------------------------------------------------------------------------
//  SetDensityAll
//-----------------------------------------------------------------------------
void    CPhysicObj::SetDensityAll (float fDensity)
{
    for (int i = 0; i < GetNumParts(); i++)
    {
        SetDensity (i, fDensity);
    }
}

//-----------------------------------------------------------------------------
//  ApplyForce
//-----------------------------------------------------------------------------
void    CPhysicObj::ApplyForce (const NxVec3& vForce, const NxVec3& vPos, NxForceMode eForceMode )
{
    assert( m_bActivated );

    int nNumParts = GetNumParts();
    for (int i = 0; i < nNumParts; i++)
    {
        ApplyForceAtPart (vForce, vPos, i, eForceMode);
    }
}

//-----------------------------------------------------------------------------
//  ApplyExplosion
//-----------------------------------------------------------------------------
void    CPhysicObj::ApplyExplosion (float fForce, const NxVec3& vPos)
{
    assert( m_bActivated );

    int nNumParts = GetNumParts();
    for (int i = 0; i < nNumParts; i++)
    {
        ApplyExplosionPart (i, fForce, vPos);
    }
}


//-----------------------------------------------------------------------------
//  ApplyExplosionPart
//-----------------------------------------------------------------------------
void    CPhysicObj::ApplyExplosionPart (int nPartIdx, float fForce, const NxVec3& vPos)
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );

	// Solo podemos aplicar fuerzas a objetos físicos dinámicos
	if (!IsDynamic())	return;

    NxActor*    pActor = GetActor( nPartIdx );

    // Aplicamos una fuerza en cada shape, proporcional a su volumen.
    // NOTA: Para que fuese mas realista realmente habria que modular la 
    //       fuerza en funcion de la superficie de proyeccion de cada shape
    //       hacia el punto de origen de la explosion, para aproximar lo que
    //       seria la superficie de impacto.
    uint    nNumShapes = pActor->getNbShapes();
    assert( nNumShapes > 0 );

    NxShape* const* pShapes = pActor->getShapes(); 
    for (uint i = 0; i < nNumShapes; i++)
    {
        NxShape*    pShape = pShapes[i];
        // - Calcular BBox de la shape
        NxBox       shapeBox = PhysicUtils::CalcOBBox( *pShape );
        // - Calcular punto de impacto de la explosion
        NxVec3      vImpactPoint = PhysicUtils::CalcExplosionImpact( shapeBox, vPos );
        //NxVec3      vImpactPoint = shapeBox.GetCenter();
        // - Modular la fuerza de la explosion por el area expuesta de la shape
        float       fVolume     = (shapeBox.extents * 2.f).magnitudeSquared();            
        float       fForceMod   = fForce * fVolume;
        // - Modular a su vez en funcion de la distancia al centro de la explosion
        //   usando una funcion exponencial de atenuacion
        NxVec3      vImpactDir  = vImpactPoint - vPos;
        float       fDist       = vImpactDir.normalize();
        // NOTA: vamos a marcar una zona de radio minimo donde se aplique la fuerza maxima
        //static float s_fMinDist = 0.1f;
        //fDist += s_fMinDist;
        static float s_fAttenuationFactor = 0.05f;
        fForceMod *= exp( -s_fAttenuationFactor * fDist );
        // - Aplicar la fuerza en el punto de impacto
        pActor->addForceAtPos( vImpactDir * fForceMod, vImpactPoint, NX_IMPULSE );
    }
}

//-----------------------------------------------------------------------------
//  SetLinearVelocity
//-----------------------------------------------------------------------------
void    CPhysicObj::SetLinearVelocity (const NxVec3& vVelocity)
{
    assert( m_bActivated );

    int nNumParts = GetNumParts();
    for (int i = 0; i < nNumParts; i++)
    {
        SetLinearVelocity (vVelocity, i);
    }
}

//-----------------------------------------------------------------------------
//  GetLinearVelocity
//-----------------------------------------------------------------------------
NxVec3  CPhysicObj::GetLinearVelocity (void) const
{
    assert( m_bActivated );

    // NOTA: Por defecto devolvemos la velocidad media de todas las partes
    NxVec3  result = CERO;
    int     nNumParts = GetNumParts();
    float   fNumPartsInv = 1.f / float(nNumParts);
    for (int i = 0; i < nNumParts; i++)
    {
        result += fNumPartsInv * GetLinearVelocity (i);
    }

    return result;
}

//-----------------------------------------------------------------------------
//  SetLinearDamping
//-----------------------------------------------------------------------------
void    CPhysicObj::SetLinearDamping (float fLinearDamp)
{
    assert( m_bActivated );

    int nNumParts = GetNumParts();
    for (int i = 0; i < nNumParts; i++)
    {
        SetLinearDamping (fLinearDamp, i);
    }
}

//-----------------------------------------------------------------------------
//  GetLinearDamping
//-----------------------------------------------------------------------------
float   CPhysicObj::GetLinearDamping (void) const
{
    assert( m_bActivated );

    // NOTA: Por defecto devolvemos el valor medio de todas las partes
    float   result = 0.f;
    int     nNumParts = GetNumParts();
    float   fNumPartsInv = 1.f / float(nNumParts);
    for (int i = 0; i < nNumParts; i++)
    {
        result += fNumPartsInv * GetLinearDamping (i);
    }

    return result;
}

//-----------------------------------------------------------------------------
//  SetAngularDamping
//-----------------------------------------------------------------------------
void    CPhysicObj::SetAngularDamping (float fAngularDamp)
{
    assert( m_bActivated );

    int nNumParts = GetNumParts();
    for (int i = 0; i < nNumParts; i++)
    {
        SetAngularDamping (fAngularDamp, i);
    }
}

//-----------------------------------------------------------------------------
//  GetAngularDamping
//-----------------------------------------------------------------------------
float   CPhysicObj::GetAngularDamping (void) const
{
    assert( m_bActivated );

    // NOTA: Por defecto devolvemos el valor medio de todas las partes
    float   result = 0.f;
    int     nNumParts = GetNumParts();
    float   fNumPartsInv = 1.f / float(nNumParts);
    for (int i = 0; i < nNumParts; i++)
    {
        result += fNumPartsInv * GetAngularDamping (i);
    }

    return result;
}

//-----------------------------------------------------------------------------
//  GetNumParts
//-----------------------------------------------------------------------------
int     CPhysicObj::GetNumParts (void) const
{
    // NOTA: No implementado. Lo tienen que implementar todas las clases derivadas
    assert(0);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////
//  Operaciones sobre partes
//////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//  ApplyForceAtPart
//-----------------------------------------------------------------------------
void    CPhysicObj::ApplyForceAtPart (const NxVec3& vForce, const NxVec3& vPos, int nPartIdx, NxForceMode eForceMode)
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );

	// Solo podemos aplicar fuerzas a objetos físicos dinámicos
	if (!IsDynamic())	return;

    GetActor(nPartIdx)->addForceAtPos (vForce, vPos, eForceMode);
}

//-----------------------------------------------------------------------------
//  SetLinearVelocity
//-----------------------------------------------------------------------------
void    CPhysicObj::SetLinearVelocity (const NxVec3& vVelocity, int nPartIdx)
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );

    GetActor(nPartIdx)->setLinearVelocity (vVelocity);
}

//-----------------------------------------------------------------------------
//  GetLinearVelocity
//-----------------------------------------------------------------------------
NxVec3  CPhysicObj::GetLinearVelocity (int nPartIdx) const
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );

    return GetActor(nPartIdx)->getLinearVelocity();
}

//-----------------------------------------------------------------------------
//  SetLinearDamping
//-----------------------------------------------------------------------------
void    CPhysicObj::SetLinearDamping (float fLinearDamp, int nPartIdx)
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );
    GetActor(nPartIdx)->setLinearDamping (fLinearDamp);
}

//-----------------------------------------------------------------------------
//  GetLinearDamping
//-----------------------------------------------------------------------------
float   CPhysicObj::GetLinearDamping (int nPartIdx) const
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );
    return GetActor(nPartIdx)->getLinearDamping();
}

//-----------------------------------------------------------------------------
//  SetAngularDamping
//-----------------------------------------------------------------------------
void    CPhysicObj::SetAngularDamping (float fAngularDamp, int nPartIdx)
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );
    GetActor(nPartIdx)->setAngularDamping (fAngularDamp);
}

//-----------------------------------------------------------------------------
//  GetAngularDamping
//-----------------------------------------------------------------------------
float   CPhysicObj::GetAngularDamping (int nPartIdx) const
{
    assert( m_bActivated && (nPartIdx < GetNumParts()) );
    return GetActor(nPartIdx)->getAngularDamping();
}

//-----------------------------------------------------------------------------
//  MoveWorldPosition
//-----------------------------------------------------------------------------
//!!
void	CPhysicObj::MoveWorldPosition	(const NxVec3 &vPos)
{
	assert( m_bActivated && IsKinematic());

	for (int i=0; i<GetNumParts(); i++) {
		MoveWorldPosition(i, vPos);
	}
}

//-----------------------------------------------------------------------------
//  MoveWorldPosition
//-----------------------------------------------------------------------------
//!!
void	CPhysicObj::MoveWorldPosition	(int nPartIdx, const NxVec3 &vPos)
{
	assert( m_bActivated && IsKinematic() && (nPartIdx < GetNumParts()));

	GetActor(nPartIdx)->moveGlobalPosition(vPos);
}

//-----------------------------------------------------------------------------
//  MoveWorldOrientation
//-----------------------------------------------------------------------------
//!!
void	CPhysicObj::MoveWorldOrientation(const NxMat33 &mOrient)
{
	assert( m_bActivated && IsKinematic());

	for (int i=0; i<GetNumParts(); i++) {
		MoveWorldOrientation(i, mOrient);
	}
}

//-----------------------------------------------------------------------------
//  MoveWorldOrientation
//-----------------------------------------------------------------------------
//!!
void	CPhysicObj::MoveWorldOrientation(int nPartIdx, const NxMat33 &mOrient)
{
	assert( m_bActivated && IsKinematic() && (nPartIdx < GetNumParts()));

	GetActor(nPartIdx)->moveGlobalOrientation(mOrient);
}
