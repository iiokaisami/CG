#include "BoxFilter.hlsli"

cbuffer BoxFilterCB : register(b0)
{
    float intensity;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

static const float2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float kKernel3x3[3][3] =
{
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
};

PixelShaderOutput main(VertexShaderOutput input)
{
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp((float) width), rcp((float) height));
    float2 uvStep = float2(1.0 / width, 1.0 / height);
    
    PixelShaderOutput output;
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    output.color.a = gTexture.Sample(gSampler, input.texcoord).a;
    
    float weightSum = 0.0;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float2 offset = float2(x, y) * uvStep;
            float2 coord = input.texcoord + offset;
            float dist = length(float2(x, y)); // 原点からの距離
            float weight = exp(-dist * intensity * 2.0); // 強度で減衰

            output.color.rgb += gTexture.Sample(gSampler, coord).rgb * weight;
            weightSum += weight;
        }
    }

    output.color.rgb /= weightSum;
    
    return output;
}
