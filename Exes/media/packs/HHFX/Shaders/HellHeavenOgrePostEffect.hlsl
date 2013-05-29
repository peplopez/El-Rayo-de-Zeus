sampler g_scene		: register(s0);
sampler g_distord	: register(s1);

// ===================================================================================
//								BLACKANDWHITE POST FX
// ===================================================================================

// Render a quad
void mainBlackAndWhiteVS(	in		float4		iPos		: POSITION,
							out		float4		oPos		: POSITION,
							out		float2		oUV			: TEXCOORD0,

							uniform	float4x4	uWorldViewProj)
{
	// Use standardise transform, so work accord with render system specific 
	// (RS depth, requires texture flipping, etc)
	iPos.w = 1;
	oPos = mul(uWorldViewProj, iPos);

	// The input positions adjusted by texel offsets, so clean up inaccuracies
	iPos.xy = sign(iPos.xy);

	// Convert to image-space
	oUV = (float2(iPos.x, -iPos.y) + 1.0f) * 0.5f;	
}

void mainBlackAndWhitePS(	in 		float2		iUV			: TEXCOORD0,
							out		float4		oColor		: COLOR)
{
	float4 rttColor = tex2D(g_scene, iUV);
	float3 outColor = dot(rttColor.rgb, float3(0.3, 0.59, 0.11));

	oColor = float4(outColor, 1.0);
}

// ===================================================================================
//								DISTORTION POST FX
// ===================================================================================

// Render a quad
void mainDistortionVS(	in		float4		iPos		: POSITION,
						out		float4		oPos		: POSITION,
						out		float2		oUV			: TEXCOORD0,

						uniform	float4x4	uWorldViewProj)
{
	// Use standardise transform, so work accord with render system specific 
	// (RS depth, requires texture flipping, etc)
	iPos.w = 1;
	oPos = mul(uWorldViewProj, iPos);

	// The input positions adjusted by texel offsets, so clean up inaccuracies
	iPos.xy = sign(iPos.xy);

	// Convert to image-space
	oUV = (float2(iPos.x, -iPos.y) + 1.0f) * 0.5f;
}

void mainDistortionPS(	in		float2		iUV			: TEXCOORD0,
						out		float4		oColor		: COLOR)
{
	float4 distordColor = tex2D(g_distord, iUV);

	float2 offset = distordColor.rg * 0.035f;
	oColor = tex2D(g_scene, iUV + offset);

	// debug, show the scene/distortion texture alone
	//oColor = tex2D(g_scene, iUV); // scene	
	//oColor = tex2D(g_distord, iUV); // distord
}
