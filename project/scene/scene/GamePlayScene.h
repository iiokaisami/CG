#pragma once

#include <Framework.h>

#include "../scene/base/BaseScene.h"

#include "../objects/Player/Player.h"
#include "../objects/Enemy/Enemy.h"
#include "../objects/Skydome/Skydome.h"

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

	// 2Dスプライト
	std::vector<Sprite*>sprites = {};
	// 3Dオブジェクト
	std::vector<Object3d*>object3ds = {};


	// ゲームで使うモデル
	std::unique_ptr<Player> pPlayer_ = nullptr;
	//std::list<std::unique_ptr<Enemy>> pEnemy_;
	std::unique_ptr<Enemy> pEnemy_;
	std::unique_ptr<Skydome> pSkydome_ = nullptr;

};

