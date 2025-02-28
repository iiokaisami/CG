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
#include "../Enemy.h"

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

	Vector3 cameraPos_ = { 0.0f,25.0f,-32.0f };
	Vector3 cameraRot_ = { 0.65f,0.0f,0.0f };

	// 衝突判定マネージャ
	CollisionManager* collisionManager_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// 敵
	std::unique_ptr<Enemy> enemy_ = nullptr;

	// フィールド
	std::unique_ptr<Field> field_ = nullptr;
};

