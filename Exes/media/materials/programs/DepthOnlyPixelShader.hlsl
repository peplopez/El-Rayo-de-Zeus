/*=============================================================================
	DepthOnlyPixelShader.hlsl: Depth-only pixel shader.
	Copyright 1998-2007 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

#include "Common.usf"
#include "Material.usf"
#include "VertexFactory.usf"

void Main(
#if !MATERIALBLENDING_SOLID
	FVertexFactoryInterpolants FactoryInterpolants,
	OPTIONAL_FacingSign
#endif
	out float4 OutColor : COLOR0
	)
{
#if !MATERIALBLENDING_SOLID
	FMaterialParameters MaterialParameters = GetMaterialParameters(FactoryInterpolants);
	CalcMaterialParameters(MaterialParameters, FacingSign, float3(0,0,1), float4(0,0,1,0));

	GetMaterialClipping(MaterialParameters);
#endif
	OutColor = 0;
}
