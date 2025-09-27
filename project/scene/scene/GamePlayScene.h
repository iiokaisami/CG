#pragma once

#include <Framework.h>

#include "../base/BaseScene.h"
#include "../../application/Objects/Player/Player.h"
#include "../../application/Objects/Enemy/EnemyManager.h"
#include "../../application/Objects/Field/Field.h"
#include "../../application/Objects/Field/Wall.h"
#include "../../application/Objects/Field/Goal.h"
#include "../../gameEngine/Collider/ColliderManager.h"

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


	// カメラシェイク
	void CameraShake();

	// カメラ追尾
	void CameraFollow();


private:

	CameraManager& cameraManager = CameraManager::GetInstance();
	std::shared_ptr<Camera> camera = nullptr;
    
    uint32_t activeIndex = 0;
	Vector3 cameraRotate = { -0.9f,0.0f,0.0f };
	Vector3 cameraPosition = { 0.0f,-20.0f,0.0f };

	// 2Dスプライト
	//std::vector<Sprite*>sprites = {};

	// 衝突判定
	ColliderManager* colliderManager_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> pPlayer_ = nullptr;

	// エネミー
	std::unique_ptr<EnemyManager> pEnemyManager_ = nullptr;

	// フィールド
	std::unique_ptr<Field> pField_ = nullptr;

	// 壁
	std::vector<std::unique_ptr<Wall>> pWalls_;

	// ゴール
	std::unique_ptr<Goal> pGoal_ = nullptr;
	
	// カメラが静止時に追従する基準位置
	Vector3 cameraRestCenter_; 
	// カメラが現在静止モードかどうか
	bool cameraIsResting_ = true;

};

