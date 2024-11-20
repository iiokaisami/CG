#pragma once

#include "DirectXCommon.h"

class TextureManager
{
private:
	static TextureManager* instance;

	

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = default;

public:

	// シングルトンインスタンスの取得
	static TextureManager* GetInstance();
	// 終了
	void Finalize();


	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	// テクスチャファイルの読み込み
	void LoadTexture(const std::string& filePath);

	//-----------ゲッター----------//
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	// テクスチャ番号からGPUハンドルを所得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

	// メタデータを取得
	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);

private:

	// テクスチャ1枚分のデータ
	struct TextureData 
	{
		std::string filePath;
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediate;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	// テクスチャデータ
	std::vector<TextureData> textureDatas;

	DirectXCommon* dxCommon_;

	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;
};

