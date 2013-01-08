#include "PhysicModelCharacter.h"
#include "PhysicEngine.h"
#include "PhysicUtils.h"
//#include "Graphics/GLObject.h"

//-----------------------------------------------------------------------------
//  Constructor
//-----------------------------------------------------------------------------
CPhysicModelCharacter::CPhysicModelCharacter (void)
{
    // Por defecto definimos un controller de tipo Box
    m_eType = NX_CONTROLLER_BOX;
}

//-----------------------------------------------------------------------------
//  Destructor
//-----------------------------------------------------------------------------
CPhysicModelCharacter::~CPhysicModelCharacter (void)
{
}

//-----------------------------------------------------------------------------
//  GetControllerType
//-----------------------------------------------------------------------------
NxControllerType    CPhysicModelCharacter::GetControllerType () const
{
    return  m_eType;
}

//-----------------------------------------------------------------------------
//  SetControllerType 
//-----------------------------------------------------------------------------
void    CPhysicModelCharacter::SetControllerType (NxControllerType eType)
{
    m_eType = eType;
}

//-----------------------------------------------------------------------------
//  EditBoxParams 
//-----------------------------------------------------------------------------
NxBoxControllerDesc&    CPhysicModelCharacter::EditBoxParams ()
{
    return  m_BoxDesc;
}

//-----------------------------------------------------------------------------
//  EditCapsuleParams 
//-----------------------------------------------------------------------------
NxCapsuleControllerDesc&    CPhysicModelCharacter::EditCapsuleParams ()
{
    return  m_CapsuleDesc;
}

//-----------------------------------------------------------------------------
//  GetNxControllerDesc
//-----------------------------------------------------------------------------
const NxControllerDesc& CPhysicModelCharacter::GetNxControllerDesc (void) const
{
    if ( m_eType == NX_CONTROLLER_BOX )
        return  m_BoxDesc;
    else
        return  m_CapsuleDesc;
}

//-----------------------------------------------------------------------------
//  CalcBBox
//-----------------------------------------------------------------------------
NxBox   CPhysicModelCharacter::CalcBBox (void) const
{
	if ( m_eType == NX_CONTROLLER_BOX )
	{
		return NxBox (NxVec3 ((NxReal) m_BoxDesc.position.x, (NxReal) m_BoxDesc.position.y, (NxReal) m_BoxDesc.position.z) + (NxVec3(0,1,0) * m_BoxDesc.extents.y), 
					        m_BoxDesc.extents, 
					        NxMat33(NX_IDENTITY_MATRIX));
	}
	else if ( m_eType == NX_CONTROLLER_CAPSULE )
	{
		return NxBox (NxVec3 ((NxReal) m_CapsuleDesc.position.x, (NxReal) m_CapsuleDesc.position.y, (NxReal) m_CapsuleDesc.position.z) + (NxVec3(0,1,0) * m_CapsuleDesc.height/2), 
					  NxVec3(m_CapsuleDesc.radius, m_CapsuleDesc.height/2, m_CapsuleDesc.radius), 
					  NxMat33(NX_IDENTITY_MATRIX));
	}
	assert(0);
	return NxBox();
}

//-----------------------------------------------------------------------------
//  CalcVolume
//-----------------------------------------------------------------------------
float   CPhysicModelCharacter::CalcVolume (void) const
{
    return CalcVolume (0);
}

//-----------------------------------------------------------------------------
//  CalcVolume
//-----------------------------------------------------------------------------
float   CPhysicModelCharacter::CalcVolume (int nPartIdx) const
{
    assert( nPartIdx == 0 );

	if ( m_eType == NX_CONTROLLER_BOX )
	{
		return PhysicUtils::CalcVolumeBox( m_BoxDesc.extents );
	}
	else if (m_eType == NX_CONTROLLER_CAPSULE)
	{
		return PhysicUtils::CalcVolumeCapsule( m_CapsuleDesc.radius, m_CapsuleDesc.height );
	}
	else
	{	
		assert(0);
	}
	
	return 0;
}

