sampler g_image : register(s0);

#ifndef	LIGHT_NUMBER
#	define	LIGHT_NUMBER	1
#endif

// ===================================================================================
//										POINT
// ===================================================================================

void mainPointVS(inout float4 pos : POSITION,
				 uniform float4x4 g_WorldViewProj)
{
	// force w to one, we get a float4 stream as input, but the 'w' component is undefined
	pos = mul(g_WorldViewProj, float4(pos.xyz, 1));
}

void mainPointPS(out float4 color : COLOR)
{
	color = float4(1, 1, 1, 1);
}

// ===================================================================================
//										SOLID
// ===================================================================================

void mainSolidVS(inout float4 pos : POSITION,
				 inout float4 color : TEXCOORD0,
				 uniform float4x4 g_WorldViewProj)
{
	// force w to one, we get a float4 stream as input, but the 'w' component is undefined
	pos = mul(g_WorldViewProj, float4(pos.xyz, 1));
}

void mainSolidPS(in float4 inColor : TEXCOORD0, 
				 out float4 color : COLOR)
{
	color = inColor;
}

// ===================================================================================
//										TEXTURED
// ===================================================================================

void mainTexturedVS(inout float4 pos : POSITION,
					inout float4 color : TEXCOORD0,
					inout float2 uv : TEXCOORD1,
					uniform float4x4 g_WorldViewProj)
{
	// force w to one, we get a float4 stream as input, but the 'w' component is undefined
	pos = mul(g_WorldViewProj, float4(pos.xyz, 1));
}

void mainTexturedPS(in float4 iColor : TEXCOORD0,
					in float2 uv : TEXCOORD1,
					out float4 color : COLOR)
{
	color = tex2D(g_image, uv) * iColor;
	//color.rgb = saturate(pow(color.rgb, 2.2));
}

void mainTexturedPSAlphaWeight(	in float4 iColor : TEXCOORD0,
							in float2 uv : TEXCOORD1,
							out float4 color : COLOR)
{
	float4	c = tex2D(g_image, uv) * iColor;
	color = c * c.w;
	//color.rgb = saturate(pow(color.rgb, 2.2));
}

// shader for distortion texture:
void mainTexturedDistordPS(	in	float4	iColor 	: TEXCOORD0,
							in	float2	iUV		: TEXCOORD1,
							out	float4	oColor	: COLOR)
{
	// scale the color to be from negative to positive
	oColor = (tex2D(g_image, iUV) - 0.5f) * iColor;
	//oColor.rgb = saturate(pow(oColor.rgb, 2.2));
}							

// ===================================================================================
//									LIGHTINGTEXTURED
// ===================================================================================

#if 1
void mainLightingTexturedVS(in		float3		iNormal							: NORMAL,
							inout	float4		ioPosition						: POSITION,
							inout	float4		ioColor							: TEXCOORD0,
							inout	float2		ioUV							: TEXCOORD1,
							out		float3		oNormal							: TEXCOORD2,
							out		float3		oViewDir						: TEXCOORD3,
							out		float3		oLightDir[LIGHT_NUMBER]			: TEXCOORD4,

							uniform	float4x4	g_WorldViewProj,
							uniform	float4x4	g_World,
							uniform	float3		g_CameraPos,
							uniform	float3		g_LightPos[LIGHT_NUMBER])
{
	oNormal = mul(g_World, iNormal);

	// force w to one, we get a float4 stream as input, but the 'w' component is undefined
	float4 pos = float4(ioPosition.xyz, 1);
	float3 worldPos = mul(g_World, pos);
	ioPosition = mul(g_WorldViewProj, pos);
	
	for (int i = 0; i < LIGHT_NUMBER; ++i)
		oLightDir[i] = g_LightPos[i] - worldPos;

	oViewDir = g_CameraPos - worldPos;
}

void mainLightingTexturedPS(in 		float4 		iColor 							: TEXCOORD0,
							in 		float2 		iUV 							: TEXCOORD1,
							in		float3		iNormal							: TEXCOORD2,
							in		float3		iViewDir						: TEXCOORD3,
							out 	float4 		oColor 							: COLOR,
							in		float3		iLightDir[LIGHT_NUMBER]			: TEXCOORD4,

							uniform	float4		g_AmbiantColor,
							uniform	float4		g_DiffuseColor[LIGHT_NUMBER],
							uniform	float4		g_SpecularColor[LIGHT_NUMBER])
{
	iNormal = normalize(iNormal);
	iViewDir = normalize(iViewDir);
	
	float4 diffuse = float4(0, 0, 0, 0);
	float4 specular = float4(0, 0, 0, 0);
	
	for (int i = 0; i < LIGHT_NUMBER; ++i)
	{
		float lightDist = length(iLightDir[i]);
		float luminosity = 1; // should change with lightDist
		
		float3 lDir = normalize(iLightDir[i]);
		
		float diffuseIntensity = saturate(dot(iNormal, lDir));
		float3 specularVec = (2 * diffuseIntensity * iNormal) - lDir;
		float specularIntensity = pow(saturate(dot(specularVec, lDir)), 25);
		
		diffuse += diffuseIntensity * g_DiffuseColor[i] * luminosity;
		specular += specularIntensity * g_SpecularColor[i] * luminosity;
	}
	oColor = g_AmbiantColor + diffuse + specular;
}

#endif
