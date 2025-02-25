#pragma once

#include <Framework.h>
#include <vector>
#include <memory>
#include <MyMath.h>
#include <Object3d.h>

#include "../base/BaseScene.h"

#include "../ColliderManager.h"
#include "../Player.h"
#include "../Field.h"

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

private:

	CameraManager& cameraManager = CameraManager::GetInstance();
	std::shared_ptr<Camera> camera_ = std::make_shared<Camera>();

	Vector3 cameraPos_ = { 0.0f,4.0f,-15.0f };
	Vector3 cameraRot_ = { 0.0f,0.0f,0.0f };

	// 衝突判定マネージャ
	CollisionManager* collisionManager_ = nullptr;

	// プレイヤー
	std::shared_ptr<Player> player_ = nullptr;

	// フィールド
	std::shared_ptr<Field> field_ = nullptr;
};

