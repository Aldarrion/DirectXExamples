#include "PhongLights.fx"
#include "DeferredForwardShared.fx"

Texture2D DiffuseTexture : register(t0);

SamplerState DiffuseSampler : register(s0);

cbuffer ConstantBuffer : register(b0) {
    matrix World;
    matrix View;
    matrix Projection;
    matrix NormalMatrix;
    PointLight lights[NUM_LIGHTS];
    float3 ViewPos;
}


struct VS_INPUT {
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
    float2 UV : TEXCOORD0;
};

struct PS_INPUT {
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
    float3 FragPos : POSITION;
    float2 UV : TEXCOORD1;
};


// =============
// Vertex Shader
// =============
PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Norm = mul(float4(input.Norm, 0), NormalMatrix).xyz;
    output.FragPos = mul(input.Pos, World);
    output.UV = input.UV;

    return output;
}


// ============
// Pixel shader
// ============
float4 PS(PS_INPUT input) : SV_Target {
    float3 normal = normalize(input.Norm);
    float3 viewDir = normalize(ViewPos - input.FragPos);
    float4 fragColor = DiffuseTexture.Sample(DiffuseSampler, input.UV);

    float4 finalColor = float4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        finalColor += CalcPointLight(lights[i], normal, input.FragPos, fragColor, viewDir, 1.0, SHININESS, 0.0);
    }
    finalColor.a = 1.0;

    finalColor = saturate(finalColor);

    //return float4(input.FragPos, 1);
    //return float4(viewDir, 1);
    return finalColor;
}
