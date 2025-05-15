#pragma once

#include "IPostEffectPass.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

class PostEffectManager 
{
public:

    void AddPass(const std::string& name, std::unique_ptr<IPostEffectPass> pass);
    // 1つのみ有効
    void SetActiveEffect(const std::string& name, bool active);
    // 複数可
    void SetActiveEffects(const std::vector<std::string>& names); 
    void DrawAll(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState);

    void SetNoneEffect(std::unique_ptr<IPostEffectPass> pass);

    template<typename T>
    T* GetPassAs(const std::string& name)
    {
        auto it = passes_.find(name);
        if (it != passes_.end()) {
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }

private:

    std::unordered_map<std::string, std::unique_ptr<IPostEffectPass>> passes_;

    std::unique_ptr<IPostEffectPass> noneEffect_;

};
