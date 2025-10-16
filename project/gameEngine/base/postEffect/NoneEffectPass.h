#pragma once

#include "BasePostEffectPass.h"

#include <d3d12.h>
#include <wrl/client.h>
#include <string>

/// <summary>
/// エフェクトなしパス
/// DirectXTexを使ってリサイズするだけ
/// </summary>
class NoneEffectPass : public BasePostEffectPass
{
public:

	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, const std::wstring& vsPath, const std::wstring& psPath) override;
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState) override;
	std::string GetName() const override { return "NoneEffect"; }
	void UpdateConstantBuffer();


private:

	struct NoneEffectCB {
		float dummy; // 何もないのでダミー
	};

	
	// GPU上の定数バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;
	// マップされたCPUポインタ（cbData_を書き込む先）
	NoneEffectCB* mappedCB_ = nullptr;
	// 定数バッファのデータ
	NoneEffectCB cbData_ = {};
	
};

