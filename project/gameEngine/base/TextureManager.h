#pragma once

#include <unordered_map>

#include "DirectXCommon.h"
#include "SrvManager.h"

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
	void Initialize(DirectXCommon* dxCommon,SrvManager* srvManager);

	// テクスチャファイルの読み込み
	void LoadTexture(const std::string& filePath);

public: // ゲッター

	// メタデータを取得
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

	// SRVインデックスの取得
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	// テクスチャ番号からGPUハンドルを所得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);


private: // 構造体

	// テクスチャ1枚分のデータ
	struct TextureData 
	{
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediate;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

private:

	// テクスチャデータ
	std::unordered_map<std::string,TextureData> textureDatas;

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;
};

