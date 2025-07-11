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

	void SetEnvironmentMapHandle(D3D12_GPU_DESCRIPTOR_HANDLE handle, bool useEnvironmentMap);

public: // ゲッター

	const Vector3& GetScale() const { return transform_.scale; }

	const Vector3& GetRotate() const { return transform_.rotate; }

	const Vector3& GetPosition() const { return transform_.translate; }

	std::string GetModel() const;

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
		bool enable;
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

	struct PointLight
	{
		Vector4 color;
		Vector3 position;
		float intensity;
		float radius;
		float decay;
		bool enable;
	};

	struct SpotLight
	{
		Vector4 color;
		Vector3 position;
		Vector3 direction;
		float intensity;
		float distance;
		float decay;
		float consAngle;
		float cosFalloffStart;
		bool enable;
	};

	struct Environment
	{
		bool enable;
		float strength;
	};

public: // ライト等

	// 座標変換行列データ生成
	void CreateTransformationMatrixData();

	// 平行光源生成
	void CreateDirectionalLight();
	// 平行光源
	void SetDirectionalLight(const DirectionalLight& directionalLight) { *directionalLightData_ = directionalLight; }
	DirectionalLight GetDirectionalLight() { return *directionalLightData_; }
	// 平行光源向のき
	void SetDirectionalLightDirection(const Vector3& direction) { directionalLightData_->direction = direction; }
	// 平行光源の色
	void SetDirectionalLightColor(const Vector4& color) { directionalLightData_->color = color; }
	// 平行光源の強さ
	void SetDirectionalLightIntensity(float intensity) { directionalLightData_->intensity = intensity; }
	// ライトオンオフ
	void SetDirectionalLightEnable(bool enable) { directionalLightData_->enable = enable; }

	// カメラ
	void CreateCamera();

	// ポイントライト
	void CreatePointLight();
	//ポイントライト
	void SetPointLight(const PointLight& pointLight) { *pointLightData_ = pointLight; }
	PointLight GetPointLight() { return *pointLightData_; }
	//ポイントライトの位置
	void SetPointLightPosition(const Vector3& position) { pointLightData_->position = position; }
	//ポイントライトの色
	void SetPointLightColor(const Vector4& color) { pointLightData_->color = color; }
	//ポイントライトの強さ
	void SetPointLightIntensity(float intensity) { pointLightData_->intensity = intensity; }
	//ポイントライトの半径
	void SetPointLightRadius(float radius) { pointLightData_->radius = radius; }
	float GetPointLightRadius() { return pointLightData_->radius; }
	//ポイントライトの減衰率
	void SetPointLightDecay(float decay) { pointLightData_->decay = decay; }
	float GetPointLightDecay() { return pointLightData_->decay; }
	//ポイントライトのオンオフ
	void SetPointLightEnable(bool enable) { pointLightData_->enable = enable; }

	//スポットライト
	void CreateSpotLight();
	void SetSpotLight(const SpotLight& spotLight) { *spotLightData_ = spotLight; }
	SpotLight GetSpotLight() { return *spotLightData_; }
	//スポットライトの位置
	void SetSpotLightPosition(const Vector3& position) { spotLightData_->position = position; }
	//スポットライトの向き
	void SetSpotLightDirection(const Vector3& direction) { spotLightData_->direction = direction; }
	//スポットライトの色
	void SetSpotLightColor(const Vector4& color) { spotLightData_->color = color; }
	//スポットライトの強さ
	void SetSpotLightIntensity(float intensity) { spotLightData_->intensity = intensity; }
	//スポットライトの距離
	void SetSpotLightDistance(float distance) { spotLightData_->distance = distance; }
	//スポットライトの減衰率
	void SetSpotLightDecay(float decay) { spotLightData_->decay = decay; }
	//スポットライトのコーンの角度
	void SetSpotLightConsAngle(float consAngle) { spotLightData_->consAngle = consAngle; }
	void SetSpotLightCosFalloffStart(float cosFalloffStart) { spotLightData_->cosFalloffStart = cosFalloffStart; }
	//スポットライトのオンオフ
	void SetSpotLightEnable(bool enable) { spotLightData_->enable = enable; }

	//ライトのオンオフ
	void SetLighting(bool enable) { enableLighting = enable; }

	// 環境マップ
	void CreateEnvironment();
	void IsEnvironment(bool enable) { environmentData_->enable = enable; }
	void SetEnvironmentStrength(float strength) { environmentData_->strength = strength; }

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
	// ポイントライトリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_{};
	// スポットライトリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource_{};
	// 環境マップリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> environmentResource_{};


	//// バッファリソース内のデータを指すポインタ
	TransformationMatrix* transformationMatrixData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	CameraForGPU* cameraData_ = nullptr;
	PointLight* pointLightData_ = nullptr;
	SpotLight* spotLightData_ = nullptr;
	Environment* environmentData_ = nullptr;

	D3D12_GPU_DESCRIPTOR_HANDLE environmentMapHandle_ = { 0 };

	Transform transform_{};

	//ライトのオンオフ
	bool enableLighting = false;

	std::string modelFilePath_;
};

