#include "Skybox.hlsli"

struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
};

ConstantBuffer<TransformationMatrix> gTransformMatrix : register(b0);

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    output.position = mul(float4(input.position, 1.0f), gTransformMatrix.WVP).xyww;
    output.texcoord = input.position.xyz;
    
    return output;
}

