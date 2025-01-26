#pragma once

#include "MyMath.h"
#include "TextureManager.h"
#include "CameraManager.h"

class Object3dCommon;
class Model;


class Object3d
{
public:

	// 初期化
	void Initialize(const std::string& filePath);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

public: // セッター

	void SetCamera(std::shared_ptr<Camera> camera) { camera_ = camera; }

	void SetModel(Model* model) { model_ = model; }

	void SetModel(const std::string& filePath);

	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

	void SetPosition(const Vector3& translate) { transform_.translate = translate; }

public: // ゲッター

	const Vector3& GetScale() const { return transform_.scale; }

	const Vector3& GetRotate() const { return transform_.rotate; }

	const Vector3& GetPosition() const { return transform_.translate; }

private:

	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInvTranspose;
	};

	struct DirectionalLight
	{
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	struct CameraForGPU
	{
		Vector3 worldPosition;
	};


	// 座標変換行列データ生成
	void CreateTransformationMatrixData();

	// 平行光源生成
	void CreateDirectionalLight();

	// カメラ
	void CreateCamera();

private:

	Object3dCommon* object3dCommon_ = nullptr;
	
	std::shared_ptr<Camera> camera_ = nullptr;

	Model* model_ = nullptr;
	
	//TransformationMatrix用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_{};
	// 平行光源リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_{};
	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_{};


	//// バッファリソース内のデータを指すポインタ
	TransformationMatrix* transformationMatrixData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	CameraForGPU* cameraData_ = nullptr;

	//// バッファリソースの使い道を補足するバッファビュー
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Transform transform_{};
};

