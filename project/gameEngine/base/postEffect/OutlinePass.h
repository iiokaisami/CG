#pragma once

#include "BasePostEffectPass.h"

#include <Mymath.h>
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

	void SetIntensity(Matrix4x4 matrix) { cbData_.projectionInverse = matrix; }

	void SetDepthResource(ID3D12Resource* resource) { depthResource_ = resource; }

public: // ゲッター

	std::string GetName() const override { return "Outline"; }

private:

	struct OutlineMaterialCB
	{
		Matrix4x4 projectionInverse;
	};

	OutlineMaterialCB cbData_ = {};
	// GPU上の定数バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;
	// マップされたCPUポインタ（cbData_を書き込む先）
	OutlineMaterialCB* mappedCB_ = nullptr;

	// 深度バッファリソースへのポインタ
	ID3D12Resource* depthResource_ = nullptr;

	// 深度バッファSRVのSRVインデックス
	uint32_t depthSrvIndex_ = 0;

	// 深度バッファの現在のリソース状態管理
	D3D12_RESOURCE_STATES depthResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;

};

/*

D3D12_SHADER_RESOURCE_VIEW_DESC depthTextureSrvDesc{};
depthTextureSrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
depthTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
depthTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
depthTextureSrvDesc.Texture2D.MipLevels = 1;
device->CreateShaderResourceView(depthStencilResource.Get(), &depthTextureSrvDesc, 新しいdescriptorHandle);

// PointFilter用のSamplerはFilterにD3D12_FILTER_MIN_MAG_MIP_POINTを指定
staticSamplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
// 後はCBuffer用のResourceを作りProjectionInverseをセットする


// DepthをShaderで読むためにStateを変更する
// 最初のシーンのdepthを書き込んでいるからDEPTH_WRITEになっているので変更する
Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
// 読み込んでOutlineを書いたら、次のフレームでまたdepthを書き込む必要があるので元に戻す
Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;

*/