#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

#include "MyMath.h"

class SpriteCommon;

class Sprite
{
public:

	// 初期化
	void Initialize(SpriteCommon* spriteCommon);

	// 更新
	void Update();

	// 描画
	void Draw();

private:

	// spriteCommon
	SpriteCommon* spriteCommon_ = nullptr;

	// バッファリソース
	//スプライト用の頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_{};
	//IndexResourceを生成する
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_{};
	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_{};
	//VertexShaderで利用するTransformationMatrix用のResourceを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_{};


	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	Material* materialData_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;
	

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
};

