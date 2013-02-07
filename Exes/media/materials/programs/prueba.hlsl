sampler DiffMap0: register(s0);
sampler DiffMap1: register(s1);

float4x4 ViewProjectionMatrix;

float4 LightAmbient;
float4 MatAmbient;

float separacion;
float tiempo;

struct VsInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv0 : TEXCOORD0;
};

struct VsOutput
{
	float2 uv0 : TEXCOORD0;
	float4 color : COLOR;
	float4 position : POSITION;
};

struct PsInput
{
	float2 uv0 : TEXCOORD0;
	float4 color : COLOR;
};

VsOutput vsMain(const VsInput vsIn) {

	VsOutput vsOut;

	vsOut.position = mul(ViewProjectionMatrix, vsIn.position);
	vsOut.uv0 = vsIn.uv0;
	//vsOut.color = float4(1.0f, 1.0f, 1.0f, 0.0f);
	vsOut.color = LightAmbient * MatAmbient;

	return vsOut;
}

float4 main(const PsInput psIn) : COLOR {

	float4 texel1, texel2;
	texel1 = tex2D(DiffMap0, psIn.uv0);
	texel2 = tex2D(DiffMap0, psIn.uv0 + tiempo*separacion);
	float4 texColor = lerp(texel1, texel2, 0);
	float4 baseColor = psIn.color * texColor;
	return baseColor;
}

