#pragma once

#include <Framework.h>

#include "../base/BaseScene.h"

class GamePlayScene : public BaseScene
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

private:

	CameraManager& cameraManager = CameraManager::GetInstance();
	std::shared_ptr<Camera> camera1 = std::make_shared<Camera>();
	std::shared_ptr<Camera> camera2 = std::make_shared<Camera>();
    
    uint32_t activeIndex = 0;
	Vector3 camera1Rotate = { 0.0f,0.0f,0.0f };
	Vector3 camera1Position = { 0.0f,4.0f,0.0f };
	Vector3 camera2Rotate = { 0.0f,0.0f,0.0f };
	Vector3 camera2Position = { 0.0f,4.0f,0.0f };
	
	// 2Dスプライト
	std::vector<Sprite*>sprites = {};

	// 3Dオブジェクト
	std::vector<Object3d*> object3ds = {};

	// 透明チェック
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	// 位置
	Vector3 position_ = { 0.0f,0.0f,0.0f };
};

