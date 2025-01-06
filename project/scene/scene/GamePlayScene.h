#pragma once

#include <Framework.h>

#include "../scene/base/BaseScene.h"
#include "../GamePlayState/GamePlayState.h"

#include "../collider/CollisionManager.h"

#include "../objects/Player/Player.h"
#include "../objects/Enemy/Enemy.h"
#include "../objects/Skydome/Skydome.h"
#include "../objects/Ground/Ground.h"

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

	// 敵の初期化
	void EnemyInit();
	// 敵の発生コマンド
	std::stringstream enemyPopCommands1;
	std::stringstream enemyPopCommands2;
	std::stringstream enemyPopCommands3;
	// 敵の発生コマンドの読み込み
	void LoadEnemyPopData1();
	void LoadEnemyPopData2();
	void LoadEnemyPopData3();
	// 敵の発生コマンドの更新
	void UpdateEnemyPopCommands1();
	void UpdateEnemyPopCommands2();
	void UpdateEnemyPopCommands3();


	// ステートパターン
	void ChangeState(std::unique_ptr<GamePlayState> _pState);

public: // ゲッター

	// プレイヤーの位置
	Vector3 GetPlayerPos() { return pPlayer_->GetPosition(); }

	// 敵
	std::list<Enemy*>& GetEnemy() { return pEnemy_; }

	// 敵フェーズ切り替え
	bool GetIsEnemyPhaseChange() { return isEnemyPhaseChange_; }

public: // セッター

	// クリアフラグ
	void SetClearFlag(bool _isClear) { isClear_ = _isClear; }

private:

	CameraManager& cameraManager = CameraManager::GetInstance();
	std::shared_ptr<Camera> camera_ = std::make_shared<Camera>();
	CollisionManager* collisionManager_ = nullptr; // 衝突判定マネージャ

	// 2Dスプライト
	std::vector<Sprite*>sprites = {};
	// 3Dオブジェクト
	std::vector<Object3d*>object3ds = {};


	// ゲームで使うモデル
	std::unique_ptr<Player> pPlayer_ = nullptr;
	//std::unique_ptr<Enemy> pEnemy_;
	std::unique_ptr<Skydome> pSkydome_ = nullptr;
	std::unique_ptr<Ground> pGround_ = nullptr;
	std::list<Enemy*> pEnemy_;

	// 敵の位置
	Vector3 enemyPosition_{};
	// 敵機中フラグ
	bool isEnemyWaiting_ = true;
	// 敵待機タイマー
	int32_t enemyWaitingTimer_ = 9;
	// 敵の数
	int32_t enemyCount_ = 0;
	// 敵フェーズ切り替え
	bool isEnemyPhaseChange_ = false;
	// 敵フェーズ切り替えインターバル
	int32_t enemyPhaseChangeInterval_ = 120;

	// ステート
	std::unique_ptr<GamePlayState> pState_ = nullptr;

	bool isClear_ = false;

};

