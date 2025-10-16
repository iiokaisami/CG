#pragma once

#include "BasePostEffectPass.h"

#include <d3d12.h>
#include <wrl/client.h>
#include <string>

/// <summary>
/// グレースケールエフェクト
/// グレースケール化するかどうかを切り替え可能
/// </summary>
class GrayscalePass : public BasePostEffectPass
{
public:
 
    void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, const std::wstring& vsPath, const std::wstring& psPath) override;
    void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState) override;
    std::string GetName() const override { return "Grayscale"; }
    void UpdateConstantBuffer();

public: // セッター

	void SetUseGrayscale(uint32_t enable) { cbData_.useGrayscale = enable; }

private:
    struct GrayscaleCB {
        uint32_t useGrayscale;
        float padding[3];
    };

    GrayscaleCB cbData_ = {};

    // GPU上の定数バッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;  
    // マップされたCPUポインタ（cbData_を書き込む先）
    GrayscaleCB* mappedCB_ = nullptr;  

};
