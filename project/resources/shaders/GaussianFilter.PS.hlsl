#include "GaussianFilter.hlsli"

cbuffer GaussianFilterCB : register(b0)
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

static const float PI = 3.1415926535f;

float gauss(float x, float y, float sigma)
{
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    
    return exp(exponent) * rcp(denominator);
}


PixelShaderOutput main(VertexShaderOutput input)
{
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStep = float2(1.0 / width, 1.0 / height);

    PixelShaderOutput output;
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    output.color.a = gTexture.Sample(gSampler, input.texcoord).a;

    float weight = 0.0f;
    float kernel3x3[3][3];

    // まず重み（kernel）を計算し、合計を weight に
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            kernel3x3[x][y] = gauss(kIndex3x3[x][y].x, kIndex3x3[x][y].y, intensity);
            weight += kernel3x3[x][y];
        }
    }

    // 重み付きサンプリング（ガウスフィルタ本体）
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            float2 offset = kIndex3x3[x][y] * uvStep;
            float2 uv = input.texcoord + offset;

            float3 sample = gTexture.Sample(gSampler, uv).rgb;
            output.color.rgb += sample * kernel3x3[x][y];
        }
    }

    // 正規化
    output.color.rgb *= rcp(weight);
    return output;
}
