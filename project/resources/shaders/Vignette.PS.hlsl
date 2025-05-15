#include "Vignette.hlsli"

cbuffer VignetteCB : register(b0)
{
    float intensity; // 0.0〜1.0程度で調整可能
    float3 padding;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);

    float2 coord = input.texcoord - 0.5f;
    float dist = length(coord);
    float vignette = 1.0f - saturate(dist * intensity * 2.0f);

    output.color.rgb *= vignette;

    return output;
}