#include "PhysicModelSimple.h"
#include "PhysicEngine.h"
#include "PhysicUtils.h"
//#include "Graphics/GLObject.h"

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicModelSimple::CPhysicModelSimple (void)
{
    // Asociamos la descripcion dinámica al actorDesc
    m_ActorDesc.body = &m_BodyDesc;
    m_BodyDesc.mass = 1;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicModelSimple::~CPhysicModelSimple (void)
{
    // Destruir todas las shapes
    for (int i = 0; i < (int)m_ActorDesc.shapes.size(); i++)
    {
        delete m_ActorDesc.shapes[i];
    }
    m_ActorDesc.shapes.clear();
}

//-----------------------------------------------------------------------------
//  CreateShape
//-----------------------------------------------------------------------------
int     CPhysicModelSimple::CreateShape (NxShapeType shapeType)
{
    // Hacemos una copia de la descripcion en funcion del tipo
    NxShapeDesc* pNewShape = NULL;
    switch (shapeType)
    {
        case NX_SHAPE_BOX:      pNewShape = new NxBoxShapeDesc();           break;
        case NX_SHAPE_CAPSULE:  pNewShape = new NxCapsuleShapeDesc();       break;
        case NX_SHAPE_PLANE:    pNewShape = new NxPlaneShapeDesc();         break;
        case NX_SHAPE_SPHERE:   pNewShape = new NxSphereShapeDesc();        break;
        case NX_SHAPE_MESH:     pNewShape = new NxTriangleMeshShapeDesc();  break;
        default:                
            assert(0);  
        break;
    }

    m_ActorDesc.shapes.pushBack (pNewShape);
    return (m_ActorDesc.shapes.size() - 1);
}

//-----------------------------------------------------------------------------
//  DeleteShape
//-----------------------------------------------------------------------------
void    CPhysicModelSimple::DeleteShape (int nShapeIdx)
{
    assert( nShapeIdx < (int)m_ActorDesc.shapes.size() );

    delete m_ActorDesc.shapes[nShapeIdx];
    m_ActorDesc.shapes.erase (m_ActorDesc.shapes.begin() + nShapeIdx, 
                              m_ActorDesc.shapes.begin() + nShapeIdx);
}

//-----------------------------------------------------------------------------
//  GetNumShapes
//-----------------------------------------------------------------------------
int     CPhysicModelSimple::GetNumShapes (void) const
{
    return m_ActorDesc.shapes.size();
}

//-----------------------------------------------------------------------------
//  GetShape
//-----------------------------------------------------------------------------
const NxShapeDesc&  CPhysicModelSimple::GetShape (int nShapeIdx) const
{
    assert( nShapeIdx < (int)m_ActorDesc.shapes.size() );
    return *(m_ActorDesc.shapes[nShapeIdx]);
}

//-----------------------------------------------------------------------------
//  EditShape
//-----------------------------------------------------------------------------
NxShapeDesc& CPhysicModelSimple::EditShape (int nShapeIdx)
{
    assert( nShapeIdx < (int)m_ActorDesc.shapes.size() );
    return *(m_ActorDesc.shapes[nShapeIdx]);
}

//-----------------------------------------------------------------------------
//  SetMass
//-----------------------------------------------------------------------------
void    CPhysicModelSimple::SetMass (float fMass)
{
    m_BodyDesc.mass = fMass;
    // NOTA: Al establecer directamente la masa, el valor de la densidad deja de ser valido
    m_ActorDesc.density = 0.f;
}

//-----------------------------------------------------------------------------
//  GetMass
//-----------------------------------------------------------------------------
float   CPhysicModelSimple::GetMass (void) const
{
    return m_BodyDesc.mass;
}

//-----------------------------------------------------------------------------
//  SetDensity
//-----------------------------------------------------------------------------
void    CPhysicModelSimple::SetDensity (float fDensity)
{
    m_ActorDesc.density = fDensity;
    // NOTA: Al establecer directamente la masa, el valor de la densidad deja de ser valido
    m_BodyDesc.mass = 0.f;
}

//-----------------------------------------------------------------------------
//  CalcBBox
//-----------------------------------------------------------------------------
NxBox   CPhysicModelSimple::CalcBBox (void) const
{
    NxBounds3   bounds;
    for (int i = 0; i < (int)m_ActorDesc.shapes.size(); i++)
    {
        NxShapeDesc* pShapeDesc = m_ActorDesc.shapes[i];
        switch (pShapeDesc->getType())
        {
            case NX_SHAPE_BOX:
			{
				NxBoxShapeDesc* pBoxShape = (NxBoxShapeDesc*) pShapeDesc;
				NxBox shapeBBox (pBoxShape->localPose.t, pBoxShape->dimensions, pBoxShape->localPose.M);
				NxBounds3 shapeBounds;
				shapeBounds.boundsOfOBB( shapeBBox.rot, shapeBBox.center, shapeBBox.extents );
				bounds.combine (shapeBounds);
			}
            break;
            case NX_SHAPE_SPHERE:
			{
				NxSphereShapeDesc* pSphereShape = (NxSphereShapeDesc*) pShapeDesc;
				NxBox shapeBBox (pSphereShape->localPose.t, NxVec3(pSphereShape->radius), pSphereShape->localPose.M);
				NxBounds3 shapeBounds;
				shapeBounds.boundsOfOBB( shapeBBox.rot, shapeBBox.center, shapeBBox.extents );
				bounds.combine (shapeBounds);
			}
            break;
            default:
                // Caso no soportado
                assert(0);
            break;
        }
    }

    NxBox   result;
    bounds.getCenter (result.center);
    bounds.getExtents (result.extents);
    return result;
}

//-----------------------------------------------------------------------------
//  CalcVolume
//-----------------------------------------------------------------------------
float   CPhysicModelSimple::CalcVolume (void) const
{
    return CalcVolume (0);
}

//-----------------------------------------------------------------------------
//  CalcVolume
//-----------------------------------------------------------------------------
float   CPhysicModelSimple::CalcVolume (int nPartIdx) const
{
    assert( nPartIdx == 0 );

    // Sumamos el volumen de todas las shapes
    // NOTA: Si las shapes estan solapadas el resultado no sera preciso pero 
    //   valdrá como aproximacion
    float fVolumeAcum = 0.f;
    for (int i = 0; i < (int)m_ActorDesc.shapes.size(); i++)
    {
        fVolumeAcum += PhysicUtils::CalcVolumeShape (*(m_ActorDesc.shapes[i]));
    }
    return fVolumeAcum;
}

/*
void    CPhysicModelSimple::AddShapeObj3D (CGLObject* pObj3D)
{
    // NOTA: Como no tenemos los vertices en un solo array, sino en strips, tendremos
    //   que generar una shape por cada strip. 
    //   Lo mas optimo habria sido tener todos los vertices en un solo array y 
    //   luego tener un array de indices que defina los triangulos.

    // Contabilizamos triangulos y vertices
    TTriangleStrip* pTriStrip = pObj3D->m_tStrips.ptGetHead();
    int	nVertices = 0;
    int	nTriangles = 0;
    while ( pTriStrip != NULL )
    {
        nVertices  += pTriStrip->nTriangles + 2;
        nTriangles += pTriStrip->nTriangles;
        // Pasamos al siguiente strip
        pTriStrip = pObj3D->m_tStrips.ptGetNext();
    }
  
    // Creamos un vector para vertices y otro apra triangulos
    TVertex*	pVertices = new TVertex[nVertices];
    NxU32*		pTriangles = new NxU32[3 * nTriangles];

    // COpiamos los datos en los vectores
    int nBaseVertex = 0;
    int nBaseIndex = 0;
    pTriStrip = pObj3D->m_tStrips.ptGetHead();
    while ( pTriStrip != NULL )
    {
        // Copiar vertices
        memcpy( pVertices + nBaseVertex, pTriStrip->ptVertex, sizeof(TVertex) * (pTriStrip->nTriangles + 2) );
        // Copiamos los indices de los triangulos
        // NOTA: Hay que tener cuidado al convertir el tristrip a triangulos puesto
        //  que la orientacion de los triangulas viene alternada por definicion de Tristrip
        for ( UINT i = 0; i < pTriStrip->nTriangles; i++ )
        {
            if ( (i % 2) != 0 )
            {
                pTriangles[nBaseIndex + 3*i]     = nBaseVertex + i;
                pTriangles[nBaseIndex + 3*i+1]   = nBaseVertex + i + 1;
                pTriangles[nBaseIndex + 3*i+2]   = nBaseVertex + i + 2;
            }
            else
            {
                pTriangles[nBaseIndex + 3*i]     = nBaseVertex + i;
                pTriangles[nBaseIndex + 3*i+1]   = nBaseVertex + i + 2;
                pTriangles[nBaseIndex + 3*i+2]   = nBaseVertex + i + 1;
            }
        }
        // Avanzar indices base
        nBaseVertex += pTriStrip->nTriangles + 2;
        nBaseIndex  += 3 * pTriStrip->nTriangles;
        // Pasamos al siguiente strip
        pTriStrip = pObj3D->m_tStrips.ptGetNext();
    }

    // Definimos la descripcion de la malla
    NxTriangleMeshDesc terrainDesc;
    terrainDesc.numVertices         = nVertices;
    terrainDesc.numTriangles        = nTriangles;
    terrainDesc.pointStrideBytes    = sizeof(TVertex);
    terrainDesc.triangleStrideBytes = 3 * sizeof(NxU32);
    terrainDesc.points              = pVertices;
    terrainDesc.triangles           = pTriangles;
    terrainDesc.flags               = 0;

    // Añadimos una shape de tipo mesh a partir de la descripcion
    int                         nShapeIdx = CreateShape (NX_SHAPE_MESH);
    NxTriangleMeshShapeDesc&    meshShape = (NxTriangleMeshShapeDesc&) EditShape (nShapeIdx);
    // Cooking de la malla de triangulos
    MemoryWriteBuffer           buf;
    bool                        status = NxCookTriangleMesh (terrainDesc, buf);
    meshShape.meshData = CPhysicEngine::Ref().GetNxPhysicsSDK()->createTriangleMesh (MemoryReadBuffer(buf.data));

    // Destruimos el array de indices
    delete pTriangles;
}
*/