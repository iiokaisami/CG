#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

#include "MyMath.h"
#include "TextureManager.h"

class SpriteCommon;

class Sprite
{
public:

	// 初期化
	void Initialize(SpriteCommon* spriteCommon, std::string textureFilePath);

	// 更新
	void Update();

	// 描画
	void Draw(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);


	//-----------ゲッター----------//
	const Vector2& GetPosition()const { return position_; }
	float GetRotation() const { return rotation_; }

	const Vector4& GetColor() const { return materialData_->color; }

	const Vector2& GetSize()const { return size_; }

	const Vector2& GetAnchorPoint()const { return anchorPoint_; }

	const bool& IsFlipX()const { return isFlipX_; }
	const bool& IsFlipY()const { return isFlipY_; }

	const Vector2& GetTextureLeftTop()const { return textureLeftTop_; }
	const Vector2& GetTextureSize()const { return textureSize_; }

	//-----------セッター----------//
	void SetPosition(const Vector2& position) { position_ = position; }
	void SetRotation(float rotation) { rotation_ = rotation; }

	void SetColor(const Vector4& color) { materialData_->color = color; }

	void SetSize(const Vector2& size) { size_ = size; }

	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

	void SetFlipX(const bool& IsFlipX) { isFlipX_ = IsFlipX; }
	void SetFlipY(const bool& IsFlipY) { isFlipY_ = IsFlipY; }

	void SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }
	void SetTextureSize(const Vector2& textureSize) { textureSize_ = textureSize; }

private:

	// テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

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
	
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};

	// テクスチャ番号
	uint32_t textureIndex = 0;



	Transform transform_;

	//平行光源用のリソース
	DirectionalLight* directionalLightData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;

	Vector2 position_ = { 0.0f,50.0f };
	float rotation_ = 0.0f;

	Vector2 size_ = { 640.0f,360.0f };

	// アンカーポイント
	Vector2 anchorPoint_ = { 0.0f,0.0f };

	// 左右フリップ
	bool isFlipX_ = false;
	// 上下フリップ
	bool isFlipY_ = false;

	// テクスチャ左上座標
	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	// テクスチャ切り出しサイズ
	Vector2 textureSize_ = { 100.0f,100.0f };

	
};

