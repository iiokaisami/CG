#pragma once

#include <Framework.h>
#include <vector>
#include <memory>
#include <MyMath.h>
#include <Object3d.h>
#include <Vector3.h>

#include "../../gameEngine/baseScene/BaseScene.h"

#include "../../FadeTransition.h"

class ClearScene : public BaseScene
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
	std::shared_ptr<Camera> camera_ = std::make_shared<Camera>();
	Vector3 cameraPosition_{};
	Vector3 cameraRotate_{};

	// 2Dスプライト
	std::vector<Sprite*>sprites = {};

	// 遷移
	std::unique_ptr<FadeTransition> transition_ = nullptr;
	bool isTransitioning_ = false;

	// 3Dオブジェクト
	std::vector<Object3d*> object3ds = {};
	Vector3 position_ = { 0.0f,0.0f,-10.0f };
	Vector3 scale_{};
	Vector3 rotate_{};

};

