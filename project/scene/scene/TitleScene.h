#pragma once

#include <Framework.h>
#include <vector>
#include <memory>
#include <MyMath.h>
#include <Object3d.h>
#include <Vector3.h>

#include "../base/BaseScene.h"

class TitleScene : public BaseScene
{
public:

	// 初期化
	void Initialize() override;
	
	// 終了
	void Finalize() override;
	
	// 更新
	void Update() override;
	
	// 描画
	void Draw() override;

	// ライトの設定
	void SetLightSettings();

private:

	CameraManager& cameraManager = CameraManager::GetInstance();
	std::shared_ptr<Camera> camera_ = std::make_shared<Camera>();
	Vector3 cameraPosition_{};
	Vector3 cameraRotate_{};

	// 2Dスプライト
	//std::vector<Sprite*>sprites = {};

	// 3Dオブジェクト
	std::vector<Object3d*> object3ds = {};

	Vector3 position_ = { 0.0f,0.0f,-10.0f };

	// 透明チェック
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	// サウンド
	SoundData soundData_;
	SoundData soundData2_;

	bool isTerrainDraw = false;
	bool enableLighting = false;
	bool enableDirectionalLight = false;
	bool enablePointLight = false;
	bool enableSpotLight = false;

	// ライトの詳細設定用変数
	Vector3 directionalLightColor = { 1.0f, 1.0f, 1.0f };
	Vector3 directionalLightDirection = { 0.0f, -1.0f, 0.0f };
	float directionalLightIntensity = 1.0f;

	Vector3 pointLightColor = { 1.0f, 1.0f, 1.0f };
	Vector3 pointLightPosition = { 0.0f, 2.0f, 0.0f };
	float pointLightIntensity = 1.0f;
	float pointLightRadius = 10.0f;
	float pointLightDecay = 1.0f;

	Vector3 spotLightColor = { 1.0f, 1.0f, 1.0f };
	Vector3 spotLightPosition = { 2.0f, 1.25f, 0.0f };
	Vector3 spotLightDirection = Normalize({ 1.0f, 1.0f, 0.0f });
	float spotLightIntensity = 4.0f;
	float spotLightDistance = 7.0f;
	float spotLightDecay = 2.0f;
	float spotLightConsAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	float spotLightCosFalloffStart = 1.0f;
};