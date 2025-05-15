#include "PostEffectManager.h"

void PostEffectManager::AddPass(const std::string& name, std::unique_ptr<IPostEffectPass> pass) 
{
    passes_[name] = std::move(pass);
}

void PostEffectManager::SetActiveEffect(const std::string& name, bool active)
{
    auto it = passes_.find(name);

    if (it != passes_.end())
    {
        it->second->SetActive(active);
    }
}

void PostEffectManager::SetActiveEffects(const std::vector<std::string>& names) 
{
    for (auto& [n, pass] : passes_) 
    {
        pass->SetActive(std::find(names.begin(), names.end(), n) != names.end());
    }
}

void PostEffectManager::DrawAll(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState)
{
    for (auto& [_, pass] : passes_)
    {
        if (pass->IsActive()) 
        {
            pass->Draw(cmdList.Get(), inputSrvHandle, inputResource, currentState);
        }
    }
}