#pragma once

#include "EnemyWaveState.h"

/// <summary>
/// 敵のウェーブステート
/// ウェーブ2
/// </summary>
class EnemyWaveStage2 : public EnemyWaveState
{
public:

	EnemyWaveStage2(EnemyManager* _pEnemyManager);

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;
};

