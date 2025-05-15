#pragma once

#include "IPostEffectPass.h"

#include <wrl.h>
#include <d3d12.h>
#include <string>

class DirectXCommon;
class SrvManager;

class BasePostEffectPass : public IPostEffectPass {
public:

    void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, const std::wstring& vsPath, const std::wstring& psPath) override;
    void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState) override = 0;
    void EnsureResourceState(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, ID3D12Resource* resource, D3D12_RESOURCE_STATES& currentState, D3D12_RESOURCE_STATES requiredState);

protected:

    DirectXCommon* dxCommon_ = nullptr;
    SrvManager* srvManager_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
};
