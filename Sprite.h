#pragma once

#include "MyMath.h"

class SpriteCommon;

class Sprite
{
public:

	// 初期化
	void Initialize(SpriteCommon* spriteCommon);

private:

	// spriteCommon
	SpriteCommon* spriteCommon_ = nullptr;

	// バッファリソース
	//スプライト用の頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

};

