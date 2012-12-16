void main
            (
            float4 position : POSITION,
            float2 uv : TEXCOORD0,

            out float4 outPosition : POSITION,
            out float2 outUv : TEXCOORD0,
            out float4 outColor : COLOR,

            uniform float4x4 worldViewProj,
            uniform float4 ambientColor
            )
        {
            outPosition = mul(worldViewProj, position);
            outUv = uv;
            outColor = ambientColor;
        } 
