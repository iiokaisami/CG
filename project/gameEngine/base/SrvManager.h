#pragma once

#include "DirectXCommon.h"

class SrvManager
{
public:

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	// 確保関数
	uint32_t Allocate();
	bool IsAllocate();

	// 計算用関数
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// SRV生成 (テクスチャ用)
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	// SRV生成 (Structured Buffer用)
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	void PreDraw();

	// テクスチャ読み込み
	uint32_t LoadTexture(const std::string& textureFilePath);

public: // ゲッター

	// SRVのCPUディスクリプタハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32_t srvIndex) const 
	{
		if (srvIndex >= kMaxSRVCount_) {
			throw std::out_of_range("Invalid SRV index");
		}
		D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += descriptorSize_ * srvIndex;
		return handle;
	}

	// SRV用デスクリプタヒープを取得
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> GetHeap() const
	{
		return descriptorHeap_;
	}

public: // セッター

	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

private:

	DirectXCommon* dxCommon_ = nullptr;

	// 最大SRV数(最大テクスチャ数)
	static const uint32_t kMaxSRVCount_;
	//SRV用デスクリプタサイズ
	uint32_t descriptorSize_;
	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	// 次に使用するSRVインデックス
	uint32_t useIndex_ = 0;

	// テクスチャキャッシュ
	std::unordered_map<std::string, uint32_t> textureIndices_;
};

