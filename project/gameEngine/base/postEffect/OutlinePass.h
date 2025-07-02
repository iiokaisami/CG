#pragma once

#include "BasePostEffectPass.h"

#include <d3d12.h>
#include <wrl/client.h>
#include <string>

class OutlinePass : public BasePostEffectPass
{
public:

	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, const std::wstring& vsPath, const std::wstring& psPath) override;
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState) override;
	void UpdateConstantBuffer();

public: // セッター

	void SetIntensity(float intensity) { cbData_.intensity = intensity; }

public: // ゲッター

	std::string GetName() const override { return "Outline"; }

private:

	struct OutlineCB
	{
		float intensity;
		float padding[3];
	};

	OutlineCB cbData_ = {};
	// GPU上の定数バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;
	// マップされたCPUポインタ（cbData_を書き込む先）
	OutlineCB* mappedCB_ = nullptr;

};

