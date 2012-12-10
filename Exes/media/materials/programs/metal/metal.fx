/******************************************************************************
HLSL Eye Shader
by Rob Galanakis
robg@robg3d.com
www.robg3d.com

The metal shader simulates reflective metals. It has a 24-bit specular map, dual specular highlights 
(one tight and saturated, one broad and duller), and a mask for reflection (shinier areas get 
non-blurry reflections). Despite the number of maps required, this is a relatively cheap shader.

diffuseLight Texture: This is your color texture, it should be mostly void of lighting detail. 
It holds an ambient occlusion map in the alpha.

normal Texture: This is a normal map.

specularRefl Texture: This is the 24-bit specular. It should be quite saturated in color. The 
alpha holds the reflection mask, where lighter tones correspond to higher reflectivity.

ambCube Texture: This is a diffusely convolved cube map, for ambient lighting.

reflCube Texture: This a "regular" cube map, for reflections in the ambient lighting pass.

specColor: This is the color of the specular highlights.

******************************************************************************/

texture normalTexture : Normal
<
	string ResourceName = "oshHelm_n.dds";
	string ResourceType = "2D";
>;
 
sampler2D normalSampler = sampler_state
{
	Texture = <normalTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

texture specReflTexture
<
	string ResourceName = "oshHelm_sr.dds";
	string ResourceType = "2D";
>;
 
sampler2D specReflSampler = sampler_state
{
	Texture = <specReflTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};
 
texture diffuseLightTexture : Diffuse
<
	string ResourceName = "oshHelm_d.dds";
	string ResourceType = "2D";
>;
 
sampler2D diffuseLightSampler = sampler_state
{
	Texture = <diffuseLightTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};
 
texture ambCubeTexture : environment
<
	string ResourceName = "cubeCathDiff.dds";
	string ResourceType = "Cube";
>;
 
samplerCUBE ambCubeSampler = sampler_state
{
	Texture = <ambCubeTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};
 
texture reflCubeTexture : environment
<
	string ResourceName = "cubeCathRefl.dds";
	string ResourceType = "Cube";
>;
 
samplerCUBE reflCubeSampler = sampler_state
{
	Texture = <reflCubeTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};
 
/////Tweakables/////////////

//controls the level of ambient/reflection cube blending
float envBlendFactor	: SPECULAR
<
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 30;
	float UIStep = 0.1;
> = 3.0;
 
//shininess of the inner highlight
float glossInner	: SPECULARPOWER
<
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 128.0;
	float UIStep = 1.0;
> = 30.0;

//shininess of the outer highlight
float glossOuter	: SPECULARPOWER
<
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 128.0;
	float UIStep = 1.0;
> = 6.0;

//multiplier for specular Texture
float specMultInner	: SPECULAR
<
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 20.0;
	float UIStep = 0.1;
> = 1.0;

float specMultOuter	: SPECULAR
<
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 20.0;
	float UIStep = 0.1;
> = 1.0;

//Controls how much diffuse lighting there is (1 is none at all)
float diffuseFactor	: SPECULAR
<
	string UIWidget = "slider";
	float UIMin = -1.0;
	float UIMax = 1.0;
	float UIStep = 0.05;
> = 0.5;

/************** light info **************/ 
float4 ambColor : AMBIENT
<
	string UIName = "Ambient Color";
> = {1.0f, 1.0f, 1.0f, 1.0f };

float4 light1Pos : POSITION 
< 
	string UIName = "Light 1 Position"; 
	string Object = "PointLight"; 
	string Space = "World"; 
> = {100.0f, 100.0f, 100.0f, 0.0f}; 

float4 light1Color : LIGHTCOLOR 
< 
> = { 1.0f, 1.0f, 1.0f, 1.0f}; 

//---------------------------------- 
float4x4 wvp : WorldViewProjection < string UIWidget = "None"; >;  
float4x4 worldI : WorldInverse < string UIWidget = "None"; >;  
float4x4 worldIT : WorldInverseTranspose < string UIWidget = "None"; >;  
float4x4 viewInv : ViewInverse < string UIWidget = "None"; >;  
float4x4 world : World < string UIWidget = "None"; >;  

// input from application 
struct a2v { 
	float4 position		: POSITION; 
	float2 texCoord		: TEXCOORD0; 
	float3 tangent		: TANGENT; 
	float3 binormal		: BINORMAL; 
	float3 normal		: NORMAL; 

}; 

// output to fragment program 
struct v2f { 
        float4 position    		: POSITION; 
        float2 texCoord			: TEXCOORD0;
        float3 eyeVec	    	: TEXCOORD1;
        float3 lightVec    		: TEXCOORD2;
        float3 worldNormal   	: TEXCOORD3; 
		float3 worldTangent  	: TEXCOORD4; 
        float3 worldBinormal 	: TEXCOORD5; 

}; 

////////////Ambient Pass///////////////////////////

//Ambient and Self-Illum Pass Vertex Shader
v2f av(a2v In, uniform float4 lightPosition) 
{ 
	v2f Out = (v2f)0;	//zero out the shader
    Out.position = mul(In.position, wvp);				//transform vert position to homogeneous clip space 

	float3 worldSpacePos = mul(In.position, world);	//world space position of vertex
	Out.eyeVec = viewInv[3] - worldSpacePos;	//get the eye vector
    Out.texCoord.xy = In.texCoord.xy;	//Pass through texture coordinates
    Out.worldNormal = mul(In.normal, worldIT).xyz;		//compute world space normal 
    Out.worldBinormal = mul(In.binormal, worldIT).xyz;	//compute world space binormal 
    Out.worldTangent = mul(In.tangent, worldIT).xyz;		//compute world space tangent 

    return Out; 
} 

//Ambient and Self-Illum Pass Pixel Shader
float4 af(v2f In, uniform float4 lightColor) : COLOR 
{ 
	float4 diffuseLightTexture = tex2D(diffuseLightSampler, In.texCoord.xy);
	float4	specReflTexture = tex2D(specReflSampler, In.texCoord.xy);
	
	float4 normalTexture = tex2D(normalSampler, In.texCoord.xy);
	normalTexture = normalTexture * 2 - 1;		//expand to -1 to 1 range 
    float3 Nn = normalize(In.worldNormal); 		//input the vectors required for tangent to world space transform 
    float3 Tn = normalize(In.worldTangent); 
    float3 Bn = normalize(In.worldBinormal);
    float3	E = normalize(In.eyeVec); //Normalize the eye vector
    
    //Perform the tangent to world space transformation
	normalTexture = float4((Nn * normalTexture.z) + (normalTexture.x * Tn + normalTexture.y * -Bn), normalTexture.w);	 //create a per-pixel normal for Y up 
	float3 N = normalize(normalTexture.rgb); 		//normalized the normal vector 

    E = -E;		//negate eye vector for proper reflections 
    float3 reflVector = reflect(E, N);		//Compute the reflection vector 
    
	float envBlend = pow(specReflTexture.a, envBlendFactor);
	
	float3 normalReflection = lerp(N, reflVector, envBlend); 
	
	float4 ambCubeTexture = texCUBE(ambCubeSampler, normalReflection.xyz);
	float4 reflCubeTexture = texCUBE(reflCubeSampler, normalReflection.xyz);
	
	float3 blendedCubes = lerp(ambCubeTexture.rgb, reflCubeTexture.rgb, envBlend); 
	
	float3 cubeTexAmb = blendedCubes * ambColor * diffuseLightTexture.rgb * diffuseLightTexture.a;

	float4 ret =  float4(cubeTexAmb, 1);
	return ret; 
} 

////////////Light (Multi) Pass///////////////////////////
//Multi-pass vertex shader
v2f v(a2v In, uniform float4 lightPosition) 
{ 
	v2f Out = (v2f)0; 
    Out.position = mul(In.position, wvp);				//transform vert position to homogeneous clip space 
	
	Out.texCoord = In.texCoord;	//pass through UV coordinates

    float3 worldSpacePos = mul(In.position, world);	//world space position of vertex
    Out.lightVec = lightPosition - worldSpacePos; //world space light vector (from light, to vertex position)
    Out.eyeVec = viewInv[3] - worldSpacePos; //eye vector in world space (from eye, to vertex position)
    
    Out.worldNormal = mul(In.normal, worldIT).xyz;		//compute world space normal 
    Out.worldBinormal = mul(In.binormal, worldIT).xyz;	//compute world space binormal 
    Out.worldTangent = mul(In.tangent, worldIT).xyz;		//compute world space tangent Out.worldNormal = mul(In.normal, world).xyz;		//compute world space normal by multiplying normal with world matrix

    return Out; 
}  

//Multi-Pass Pixel Shader
float4 f(v2f In,
	uniform float4 lightColor
	) : COLOR 
{ 

	//texture declarations
	float4 diffuseLightTexture = tex2D(diffuseLightSampler, In.texCoord.xy);
	float4	specReflTexture = tex2D(specReflSampler, In.texCoord.xy);
	
	float4 normalTexture = tex2D(normalSampler, In.texCoord.xy);

	float3 L = normalize(In.lightVec.xyz);		//normalized light vector 
    float3 E = normalize(In.eyeVec.xyz);		//normalized eye vector 
	float3 H = normalize(L + E);				//Normalized half angle vector
	
	//input the vectors required for tangent to world space transform 
	float3 Nn = In.worldNormal;
    float3 Tn = In.worldTangent; 
    float3 Bn = In.worldBinormal;
    
    //figure out N (normal)
    normalTexture = normalTexture * 2 - 1;	//expand to -1, 1
    normalTexture = float4((Nn * normalTexture.z) + (normalTexture.x * Tn + normalTexture.y * -Bn), 1.0);
	float3	N = normalize(normalTexture.rgb);	//overlay detail texture

	//diffuse calculations
	float	NdotL = dot(N, L);
	float	NdotLmod = saturate(NdotL - diffuseFactor);
	float3	diffuseComp = diffuseLightTexture.rgb * NdotLmod;
	
	//specular calculations
	float	NdotH = saturate(dot(N, H));
	float	specularLevelInner = pow(NdotH, glossInner);
	float3	specInner = specularLevelInner * specMultInner * specReflTexture.rgb;
	
	float	specularLevelOuter = pow(NdotH, glossOuter);
	float3	specOuter = specularLevelOuter * specMultOuter * diffuseLightTexture.rgb;
	
	float3	specComp = (specInner + specOuter) * specularLevelOuter;
	
	//lightTexture and final addition
	float3	diffuseSpec = diffuseComp + specComp;
	float3	diffuseSpecLight = diffuseSpec * light1Color * diffuseLightTexture.a;
	float4	ret =  float4(diffuseSpecLight.xyz, 1);		//Sets w value of the float4 to 1 (a float 4 must be the output of the pixel shader)	
	
	return ret; 
	}
technique Complete 
{  
	pass ambient  
    {		 
	VertexShader = compile vs_2_0 av(light1Pos); 
	PixelShader = compile ps_2_0 af(light1Color); 
	}  

	pass light1  
    {		 
	VertexShader = compile vs_2_0 v(light1Pos); 
	ZEnable = true; 
	ZWriteEnable = false; 
	ZFunc = LessEqual;
	CullMode = none; 
	AlphaBlendEnable = true; 
	SrcBlend = One;
	DestBlend = One;
	PixelShader = compile ps_2_0 f(light1Color); 
	}
}  
