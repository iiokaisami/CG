#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <string>

class DirectXCommon;
class SrvManager;

class IPostEffectPass
{
public:

    virtual ~IPostEffectPass() = default;
    virtual void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, const std::wstring& vsPath, const std::wstring& psPath) = 0;
    virtual void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState) = 0;
  
public: // セッター
    
    virtual void SetActive(bool active) { isActive_ = active; }
    
    
public: // ゲッター

    virtual bool IsActive() const { return isActive_; }
    virtual std::string GetName() const = 0;

protected:

    bool isActive_ = true;

};

