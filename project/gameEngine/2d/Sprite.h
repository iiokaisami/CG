
#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

#include "../../externals/DirectXTex/DirectXTex.h"
#include "../../externals/DirectXTex/d3dx12.h"

#include "MyMath.h"
#include "TextureManager.h"

class SpriteCommon;


/// <summary>
/// スプライト
/// </summary>
class Sprite
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureFilePath">テクスチャファイルパス</param>
	/// <param name="position">座標</param>
	/// <param name="color">色</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	void Initialize(std::string textureFilePath,Vector2 position, Vector4 color = { (1) , (1), (1), (1) }, Vector2 anchorpoint = { 0.0f,0.0f });

	// 更新
	void Update();

	// 描画
	void Draw();


public:// ゲッター

	// 座標取得
	const Vector2& GetPosition()const { return position_; }
	// 回転取得
	float GetRotation() const { return rotation_; }
	// 色取得
	const Vector4& GetColor() const { return materialData_->color; }
	// サイズ取得
	const Vector2& GetSize()const { return size_; }
	// アンカーポイント取得
	const Vector2& GetAnchorPoint()const { return anchorPoint_; }
	// フリップX取得
	const bool& IsFlipX()const { return isFlipX_; }
	// フリップY取得
	const bool& IsFlipY()const { return isFlipY_; }
	// テクスチャ左上座標取得
	const Vector2& GetTextureLeftTop()const { return textureLeftTop_; }
	// テクスチャ切り出しサイズ取得
	const Vector2& GetTextureSize()const { return textureSize_; }

public:// セッター

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector2& position) { position_ = position; }
	
	/// <summary>
	/// 回転設定
	/// </summary>
	///	<param name="rotation">回転角</param>
	void SetRotation(float rotation) { rotation_ = rotation; }

	/// <summary>
	/// 色設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { materialData_->color = color; }

	/// <summary>
	/// 色変更設定
	/// </summary>
	/// <param name="color">色変更量</param>
	void SetColorChange(const Vector4& color);

	/// <summary>
	/// サイズ設定
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const Vector2& size) { size_ = size; }

	/// <summary>
	/// アンカーポイント設定
	/// </summary>
	/// <param name="anchorPoint">アンカーポイント</param>
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

	/// <summary>
	/// フリップX設定
	/// </summary>
	/// <param name="IsFlipX">フリップXフラグ</param>
	void SetFlipX(const bool& IsFlipX) { isFlipX_ = IsFlipX; }
	
	/// <summary>
	/// フリップY設定
	/// </summary>
	/// <param name="IsFlipY">フリップYフラグ</param>
	void SetFlipY(const bool& IsFlipY) { isFlipY_ = IsFlipY; }

	/// <summary>
	/// テクスチャ左上座標設定
	/// </summary>
	/// <param name="textureLeftTop">テクスチャ左上座標</param>
	void SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }
	
	/// <summary>
	/// テクスチャ切り出しサイズ設定
	/// </summary>
	/// 
	void SetTextureSize(const Vector2& textureSize) { textureSize_ = textureSize; }

private:

	// テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

private: // 構造体、関数

	struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Material
	{
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};


private:

	// spriteCommon
	SpriteCommon* spriteCommon_ = nullptr;
	std::string textureFilePath_;

	// 定数バッファの宣言
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;

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

	Vector4 color_{};
};
