#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>

class TextureLoader
{
public:

    static Microsoft::WRL::ComPtr<ID3D12Resource> LoadTextureFromFile(
        ID3D12Device* device,
        ID3D12GraphicsCommandList* commandList,
        const std::string& filePath);
};
