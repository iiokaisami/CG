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
    output.color = gTexture.Sample(gSampler, input.texcoord);

  
    if (useGrayscale == 0)
    {
        float value = dot(output.color.rgb, float3(0.2125f, 0.7154f, 0.0721f));
        output.color.rgb = float3(value, value, value);
    }
    if (useGrayscale == 1)
    {
        output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    }
    if (useGrayscale == 2)
    {
        output.color.rgb = float3(0.5f, 0.5f, 0.5f);
    }
    

    return output;
}
