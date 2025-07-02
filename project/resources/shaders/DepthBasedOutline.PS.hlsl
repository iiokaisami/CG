#include "LuminaceBasedOutline.hlsli"

cbuffer OutlineCB : register(b0)
{
    float intensity;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

Texture2D<float> gDepthTexture : register(t1);
SamplerState gSamplerPoint : register(s1);

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

static const float kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

float Luminace(float3 v)
{
    return dot(v, float3(0.2125f, 0.7154f, 0.0721f));
}

PixelShaderOutput main(VertexShaderOutput input)
{
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp((float) width), rcp((float) height));
    
    float2 differnce = float2(0.0f, 0.0f);
    
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            float2 textureCoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;   
            float ndcDepth = gDepthTexture.Sample(gSamplerPoint, textureCoord);
            float4 viewSpace = mul(float4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
            float viewZ = viewSpace.z * rcp(viewSpace.w);
        
            differnce.x += viewZ * kPrewittHorizontalKernel[x][y];
            differnce.y += viewZ * kPrewittVerticalKernel[x][y];
           
        }
    }
    
    float weight = length(differnce);
    weight = saturate(weight);
    
    PixelShaderOutput output;
    output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler, input.texcoord).rgb;
    output.color.a = 1.0f;
    
    return output;
  
}