#pragma once

#include "BasePostEffectPass.h"

#include <d3d12.h>
#include <wrl/client.h>
#include <string>
#include <MyMath.h>

class VignettePass :public BasePostEffectPass {
public:
   
    void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, const std::wstring& vsPath, const std::wstring& psPath) override;
    void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState) override;
    std::string GetName() const override { return "Vignette"; }
    void UpdateConstantBuffer();

public: // セッター

    void SetStrength(float strength) { cbData_.strength = strength; }

private:
    struct VignetteCB
    {
        float strength = 1.0f;
        float padding[3];
    };

    VignetteCB cbData_ = {};
    Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;
    VignetteCB* mappedCB_ = nullptr;

};

