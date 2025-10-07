#include "Grayscale.hlsli"

cbuffer GrayscaleCB : register(b0)
{
    uint useGrayscale;
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
    float4 texColor = gTexture.Sample(gSampler, input.texcoord);

    // まず元の色を出力
    output.color = texColor;

    if (useGrayscale == 0)
    {
        float value = dot(output.color.rgb, float3(0.2125f, 0.7154f, 0.0721f));
        output.color.rgb = float3(value, value, value);
    }
    if (useGrayscale == 1)
    {
        float3 sepia;
        sepia.r = dot(output.color.rgb, float3(0.4f, 0.75f, 0.2f));
        sepia.g = dot(output.color.rgb, float3(0.35f, 0.68f, 0.15f));
        sepia.b = dot(output.color.rgb, float3(0.25f, 0.55f, 0.1f));
        output.color.rgb = saturate(sepia);
    }
    if (useGrayscale == 2)
    {
        float3 cool;
        cool.r = output.color.rgb.r * 0.6f;
        cool.g = output.color.rgb.g * 0.8f;
        cool.b = output.color.rgb.b * 1.5f;
        output.color.rgb = saturate(cool);
    }

    // アルファ値はそのまま
    // output.color.a = texColor.a; // これは既にセット済み

    return output;
}
