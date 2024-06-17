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
    float3 direction; //ライトの向き
    float4 color; //ライトの色
    float intensity; //輝度
    float lightInnerCos; //角度減衰が起こらない範囲を表す角
    float lightOuterCos; //ライトが当たる範囲を表す角
    
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

/*float GetAngleAttenuation
(
   float cos_s, //ライト方向ベクトルと光源ベクトルの内積
   float cos_p, //内側のcos
   float cos_u //外側のcos
)
{
    float minDist = 0.01;
    
    float d = max(cos_p - cos_u, minDist);
    float t = saturate((cos_s - cos_u) / d);
    return t * t;
}*/

float GetAngleAttenuation
(
   float3 unnormalizedLightVector,
   float3 direction,
   float lightAngleScale,
   float lightAngleOffset
)
{
    
    float cd = dot(direction, unnormalizedLightVector);
    
    float attenuation = saturate(cd * lightAngleScale + lightAngleOffset);
    
    //滑らかに変化させる
    return attenuation * attenuation;
}

#define MIN_DIST (0.01)

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    if (gMaterial.enableLighting != 0)
    {
        float minDist = 0.01;
        
        float lightInvRadiusSq = 1/pow(gDirectionalLight.lightInvSqrRadius, 2.0f);
        
        float3 unnormalizedLightVector = gDirectionalLight.lightPosition;
        
        
        float3 L = normalize(unnormalizedLightVector);
       float lightAngleScale = 1.0f / max(0.001f, (gDirectionalLight.lightInnerCos - gDirectionalLight.lightOuterCos));
        float lightAngleOffset = -gDirectionalLight.lightOuterCos * lightAngleScale;
        
        float sqrDist = dot(unnormalizedLightVector, unnormalizedLightVector);
           
        float att = 1.0f / max(sqrDist, MIN_DIST * MIN_DIST);
        
        att /***/= GetAngleAttenuation(-L, gDirectionalLight.direction, lightAngleScale, lightAngleOffset);
        
        //float att = GetAngleAttenuation(dot(-L, gDirectionalLight.direction), gDirectionalLight.lightInnerCos, gDirectionalLight.lightOuterCos);
        //att /***/= GetDistanceAttenuation(unnormalizedLightVector, lightInvRadiusSq);
        
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity * att;
       
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    return output;
}


