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
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
Texture2D<float4> gTexture : register(t0);
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
        float3 pointLigttDirection = normalize(input.worldPosition - gPointLight.position);
     
        float3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity;
        float3 specularPointLight = gMaterial.color.rgb * gPointLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);    
        
        output.color.rgb = diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight;

    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    return output;
}


