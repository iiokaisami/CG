#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <string>

class DirectXCommon;
class SrvManager;

class CopyPass
{
public:
    void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, const std::wstring& vsPath, const std::wstring& psPath);
    void Draw(Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> cmdList, D3D12_GPU_DESCRIPTOR_HANDLE srvHandle);

    struct GrayscaleCB
    {
        uint32_t useGrayscale;
        float padding[3]; // 16バイトに揃える
    };

public: // セッター

    void SetUseGrayscale(uint32_t enable) { grayscaleCBData_.useGrayscale = enable; }
      
private:
    DirectXCommon* dxCommon_ = nullptr;
    SrvManager* srvManager_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

    Microsoft::WRL::ComPtr<ID3D12Resource> grayscaleCB_;
    GrayscaleCB grayscaleCBData_ = { 0 }; // 初期状態はグレースケール無効
};

