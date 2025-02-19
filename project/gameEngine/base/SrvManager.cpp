#include "SrvManager.h"

#include <cassert>
#include <comdef.h> // _com_error を使用するために必要
#include <iostream>

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

    // DirectXCommonを使ってテクスチャを読み込む
    DirectX::ScratchImage mipImages = dxCommon_->LoadTexture(textureFilePath);
    if (mipImages.GetImageCount() == 0) {
        throw std::runtime_error("Failed to load texture: " + textureFilePath);
    }

    // ミップマップのメタデータを取得
    DirectX::TexMetadata metadata = mipImages.GetMetadata();

    // SRV作成に必要なリソースを作成
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
    textureDesc.Width = static_cast<UINT>(metadata.width);
    textureDesc.Height = static_cast<UINT>(metadata.height);
    textureDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureDesc.Format = metadata.format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;


    D3D12_HEAP_PROPERTIES heapProperties{};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

    // テクスチャリソースの作成
    Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
    HRESULT hr = dxCommon_->GetDevice()->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&textureResource)
    );

    if (FAILED(hr)) {
        _com_error err(hr);
        std::wcerr << L"Failed to create texture resource. Error: " << err.ErrorMessage() << std::endl;
        throw std::runtime_error("Failed to create texture resource.");
    }

    // アップロード用ヒープの作成
    const UINT64 uploadBufferSize = GetRequiredIntermediateSize(textureResource.Get(), 0, static_cast<UINT>(metadata.mipLevels));
 
    // DirectX 12 のバッファサイズの制限を確認 追加
    const UINT64 maxBufferSize = D3D12_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024 * 1024; // 128MB
    if (uploadBufferSize > maxBufferSize) {
        throw std::runtime_error("Upload buffer size exceeds the maximum allowed size.");
    }

    Microsoft::WRL::ComPtr<ID3D12Resource> textureUploadHeap;

    //頂点リソース用のヒープ設定
    D3D12_HEAP_PROPERTIES uploadHeapProperties{};
    uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    //頂点リソースの設定
    D3D12_RESOURCE_DESC bufferResourceDesc{};
    //バッファリソース。テクスチャの場合はまた別の設定をする
    bufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferResourceDesc.Width = uploadBufferSize;
    //バッファの場合はこれらは１にする決まり
    bufferResourceDesc.Height = 1;
    bufferResourceDesc.DepthOrArraySize = 1;
    bufferResourceDesc.MipLevels = 1;
    bufferResourceDesc.SampleDesc.Count = 1;

    bufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // ここを追加
    bufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE; // 必要に応じてフラグを設定

    if (dxCommon_ == nullptr) {
        throw std::runtime_error("dxCommon_ is not initialized.");
    }

    hr = dxCommon_->GetDevice()->CreateCommittedResource(
        &uploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &bufferResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&textureUploadHeap)
    );

    if (FAILED(hr)) {
        _com_error err(hr);
        std::wcerr << L"Failed to create upload heap. Error: " << err.ErrorMessage() << std::endl;
        throw std::runtime_error("Failed to create upload heap.");
    }

    // ミップマップ付きのデータをGPUに転送
    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = mipImages.GetImages()->pixels;
    textureData.RowPitch = static_cast<LONG_PTR>(mipImages.GetImages()->rowPitch);
    textureData.SlicePitch = static_cast<LONG_PTR>(mipImages.GetImages()->slicePitch);

    // サブリソース更新
    UpdateSubresources(
        dxCommon_->GetCommandList().Get(),
        textureResource.Get(),
        textureUploadHeap.Get(),
        0,
        0,
        static_cast<UINT>(metadata.mipLevels),
        &textureData
    );

    // コピー完了後の状態を変更
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        textureResource.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );

    dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

    // SRV作成
    uint32_t srvIndex = Allocate();
    CreateSRVforTexture2D(srvIndex, textureResource.Get(), textureResource->GetDesc().Format, textureResource->GetDesc().MipLevels);

    // インデックスをキャッシュに保存
    textureIndices_[textureFilePath] = srvIndex;

    return srvIndex;
}