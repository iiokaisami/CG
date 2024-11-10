#include "Particle.hlsli"


struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
};

struct ParticleForGPU
{
    float4x4 WVP;
    float4x4 World;
    float4 color;
};

StructuredBuffer<TransformationMatrix> gTransformationMatrices : register(t0);

StructuredBuffer<ParticleForGPU> gParticle : register(t0);


struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input,uint32_t instanceId : SV_InstanceID)
{
    //VertexShaderOutput output;
    //output.position = mul(input.position, gTransformationMatrices[instanceId].WVP);
    //output.texcoord = input.texcoord;
    //output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrices[instanceId].World));
    //return output;
    
    VertexShaderOutput output;
    output.position = mul(input.position, gParticle[instanceId].WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gParticle[instanceId].World));
    output.color = gParticle[instanceId].color;
    return output;
}

