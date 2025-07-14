#include "Object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
    int phongReflection;
    int halfphongReflection;
    int pointLight;
    int spotLight;
    int enableTexture;
    float environmentStrength;
};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
};

struct SpotLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
    float cosFalloffStart;
};

struct Enviroment
{
    int enable;
    float strength;
};


ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<SpotLight> gSpotLight : register(b4);
Texture2D<float4> gTexture : register(t0);
TextureCube<float4> gEnvironmentTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformeduv = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformeduv.xy);
    
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    float RtoE = dot(reflectLight, toEye);
    float specularPow = pow(saturate(RtoE), gMaterial.shininess);
    
    float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
    float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
    
    float3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    float3 specularDirectionalLight = gMaterial.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
    
    
    if (gMaterial.enableLighting != 0)
    {
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    else if (gMaterial.phongReflection != 0)
    {
        output.color.rgb = diffuseDirectionalLight + specularDirectionalLight;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else if (gMaterial.halfphongReflection != 0)
    {
        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);
        float3 specular = gMaterial.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
        
        output.color.rgb = diffuseDirectionalLight + specularPow;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else if (gMaterial.pointLight)
    { 
        // ポイントライト
        float3 diffusePointLight = { 0.0f, 0.0f, 0.0f };
        float3 specularPointLight = { 0.0f, 0.0f, 0.0f };
        
        // 光源から物体への方向
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        // 反射の計算
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float3 reflectLight = reflect(pointLightDirection, normalize(input.normal));
        float3 halfVector = normalize(-pointLightDirection + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess);
            
        float NdotL = dot(normalize(input.normal), -pointLightDirection);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float distance = length(gPointLight.position - input.worldPosition);
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        // 拡散反射
        diffusePointLight += gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
        // 鏡面反射
        float3 specularColor = { 1.0f, 1.0f, 1.0f };
        specularPointLight += gPointLight.color.rgb * gPointLight.intensity * specularPow * specularColor * factor;
    
        output.color.rgb = diffusePointLight + specularPointLight;

    }
    else if (gMaterial.spotLight)
    {
        // スポットライト
        float3 diffuseSpotLight = { 0.0f, 0.0f, 0.0f };
        float3 specularSpotLight = { 0.0f, 0.0f, 0.0f };
        
        // 光源から物体表面への方向
        float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        // 反射の計算
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float3 reflectLight = reflect(spotLightDirectionOnSurface, normalize(input.normal));
        float3 halfVector = normalize(-spotLightDirectionOnSurface + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess);
            
        float NdotL = dot(normalize(input.normal), -spotLightDirectionOnSurface);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float distance = length(gSpotLight.position - input.worldPosition);
        float attenuationFactor = pow(saturate(-distance / gSpotLight.distance + 1.0f), gSpotLight.decay);
        // フォールオフ
        float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
        float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
        
        // 拡散反射
        diffuseSpotLight += gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * cos * gSpotLight.intensity * attenuationFactor * falloffFactor;
        // 鏡面反射
        float3 specularColor = { 1.0f, 1.0f, 1.0f };
        specularSpotLight += gSpotLight.color.rgb * gSpotLight.intensity * specularPow * specularColor * attenuationFactor * falloffFactor;
     
        output.color.rgb = diffuseSpotLight + specularSpotLight;
    }
    else if (gMaterial.enableTexture && gMaterial.environmentStrength > 0.0f)
    {
        // 環境マップ
        output.color.rgb = float3(0.0f, 0.0f, 0.0f);
        
        float3 cameraToPosition = normalize(gCamera.worldPosition - input.worldPosition);
        float3 reflectedVector = reflect(-cameraToPosition, normalize(input.normal));
        float4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);

        // 映り込みの強さで加算ブレンド
        output.color.rgb += environmentColor.rgb * gMaterial.environmentStrength;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }  
    
    return output;
}
