#pragma once

#include <vector>
#include <Framework.h>

#include "Enemy.h"
#include "WaveState/EnemyWaveState.h"

class EnemyManager
{
public:

	EnemyManager() = default;
	~EnemyManager() = default;

	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// ImGui
	void ImGuiDraw();

	// 敵の初期化
	void EnemyInit(const Vector3& pos);

	// 敵のウェーブステート
	void ChangeState(std::unique_ptr<EnemyWaveState>_pState);

public: // ゲッター

	// プレイヤーとの距離のゲッター
	const std::vector<Vector3>& GetToPlayerDistance() const { return toPlayerDistance_; }

	// 敵のウェーブ切り替えフラグ
	bool IsWaveChange() const { return isWaveChange_; }

	// 全てのウェーブの敵を倒したかどうかのフラグ
	bool IsAllEnemyDefeated() const { return isAllEnemyDefeated_; }

public: // セッター

	// プレイヤーの位置をセット
	void SetPlayerPosition(const Vector3& playerPosition) { playerPosition_ = playerPosition; }

	// 全てのウェーブの敵を倒したら立てるフラグセット
	void SetAllEnemyDefeated(bool isDefeated) { isAllEnemyDefeated_ = isDefeated; }

private:

	// エネミー
	std::vector<std::unique_ptr<Enemy>> pEnemies_;
	
	// 敵の数
	uint32_t enemyCount_ = 0;

	// 敵のウェーブステート
	std::unique_ptr<EnemyWaveState> pState_ = nullptr;
	// 敵のウェーブ切り替え
	bool isWaveChange_ = false;
	// 敵のウェーブ切り替えインターバル
	int32_t waveChangeInterval_ = 120;

	// プレイヤーの位置
	Vector3 playerPosition_{};
	// プレイヤーとの距離
	std::vector<Vector3> toPlayerDistance_;

	// 全てのウェーブの敵を倒したら立てるフラグ
	bool isAllEnemyDefeated_ = false;

};

