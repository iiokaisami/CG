#include "SrvManager.h"

#include <cassert>


const uint32_t SrvManager::kMaxSRVCount_ = 512;

void SrvManager::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;

	// デスクリプタヒープの生成
	descriptorHeap_ = dxCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount_, true);
	// デスクリプタ1個分のサイズを取得して記録
	descriptorSize_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

uint32_t SrvManager::Allocate()
{
	// 上限に達していないかチェック assert
	assert(kMaxSRVCount_ > useIndex_);

	// return する番号を一旦記録しておく
	uint32_t index = useIndex_;
	// 次回の為に番号を1進める
	useIndex_++;
	// 上で記録した番号をreturn
	return index;
}

bool SrvManager::IsAllocate()
{
	if (kMaxSRVCount_ > useIndex_) {
		return true;
	}
	else {
		return false;
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	// 各項目を埋める
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = MipLevels;

	dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	// 各項目を埋める
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;

	dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::PreDraw()
{
	// 描画用DescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };
	dxCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex)
{
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}

uint32_t SrvManager::LoadTexture(const std::string& textureFilePath)
{
	// テクスチャが既にロードされているか確認
	if (textureIndices_.find(textureFilePath) != textureIndices_.end()) {
		return textureIndices_[textureFilePath];
	}

	// テクスチャリソースの読み込み
	ID3D12Resource* textureResource = nullptr;
	TextureLoader::LoadFromFile(dxCommon_->GetDevice(), textureFilePath, &textureResource);
	if (!textureResource) {
		throw std::runtime_error("Failed to load texture: " + textureFilePath);
	}

	// SRV作成
	uint32_t srvIndex = Allocate();
	CreateSRVforTexture2D(srvIndex, textureResource, textureResource->GetDesc().Format, textureResource->GetDesc().MipLevels);

	// インデックスをキャッシュに保存
	textureIndices_[textureFilePath] = srvIndex;

	return srvIndex;
}