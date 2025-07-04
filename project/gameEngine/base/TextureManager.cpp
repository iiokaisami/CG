#include "TextureManager.h"

#include <cassert>

TextureManager* TextureManager::instance = nullptr;

uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	// SRVの数と同数
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	// 読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {
		// 読み込み済みなら早期return
		return;
	}
	// テクスチャ枚数上限チェック
	assert(srvManager_->IsAllocate());

	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr;
	if (filePathW.ends_with(L".dds"))
	{
		// DDSファイルの読み込み
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else
	{
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
		assert(SUCCEEDED(hr));
	}

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	if (DirectX::IsCompressed(image.GetMetadata().format))
	{
		// 圧縮テクスチャの場合は、ミップマップを生成しない
		mipImages = std::move(image);
	} 
	else
	{
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	}
	
	// テクスチャデータを追加
	// 追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas[filePath];

	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResource(textureData.metadata);
	textureData.intermediate = dxCommon_->UploadTextureData(textureData.resource, mipImages);
	dxCommon_->CommandPass();

	// テクスチャデータの要素番号をSRVのインデックスをする
	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	if (textureData.metadata.IsCubemap())
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0; // 最も詳細なミップレベル
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f; // 最小LODクランプ値
	}
	else
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);
	}
	srvManager_->SetIsCubeMap(textureData.metadata.IsCubemap());

	// 設定を基にSRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);

}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath)
{
	// テクスチャが存在するか確認
	auto it = textureDatas.find(filePath);
	if (it == textureDatas.end()) {
		// なかったらエラーメッセージ
		Logger::Log("Error: Texture not found for filePath: " + filePath);
		throw std::runtime_error("Texture not found for filePath: " + filePath);
	}

	// テクスチャデータの参照を取得
	TextureData& textureData = it->second;
	return textureData.metadata;
}


uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	// テクスチャが存在するか確認
	auto it = textureDatas.find(filePath);
	if (it == textureDatas.end()) {
		// なかったらエラーメッセージ
		Logger::Log("Error: Texture not found for filePath: " + filePath);
		throw std::runtime_error("Texture not found for filePath: " + filePath);
	}

	// テクスチャデータの参照を取得
	TextureData& textureData = it->second;
	return textureData.srvIndex;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath)
{

	// テクスチャが存在するか確認
	auto it = textureDatas.find(filePath);
	if (it == textureDatas.end()) {
		// なかったらエラーメッセージ
		Logger::Log("Error: Texture not found for filePath: " + filePath);
		throw std::runtime_error("Texture not found for filePath: " + filePath);
	}

	// テクスチャデータの参照を取得
	TextureData& textureData = it->second;
	return textureData.srvHandleGPU;
}