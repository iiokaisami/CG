#include "Object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
};

struct DirectionalLight
{
    float3 lightPosition; //ライトの位置
    float lightInvSqrRadius; //ライトがとどく距離
    float4 color; //ライトの色
    float intensity; //輝度
    
    //float4 color;//ライトの色
    //float3 direction;//ライトの向き
    //float intensity;//輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

float SmoothDistanceAttenuation
(
   float squareDistance, //ライトからの距離の2乗
   float invSqrAttRadius //ライトが届く距離の2乗の逆数
)
{
    float factor = squareDistance * invSqrAttRadius;
    float smoothFactor = saturate(1.0f - factor * factor);
    return smoothFactor;
}

float GetDistanceAttenuation
(
   float3 unnormalizedLightVector, //ライト位置とピクセル位置の差分
   float invSqrAttRadius //ライトが届く距離の2乗の逆数
)
{
    float minDist = 0.01;
    
    
    float sqrDist = dot(unnormalizedLightVector, unnormalizedLightVector);
    float attenuation = 1.0f / (max(sqrDist, minDist * minDist));
    
    attenuation *= SmoothDistanceAttenuation(sqrDist, invSqrAttRadius);
    
    return attenuation;
}


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    if (gMaterial.enableLighting != 0)
    {
        
        float att = GetDistanceAttenuation(gDirectionalLight.lightPosition, gDirectionalLight.lightInvSqrRadius);
        
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * gDirectionalLight.intensity * att;
        
        //float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        //float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        //output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    return output;
}


