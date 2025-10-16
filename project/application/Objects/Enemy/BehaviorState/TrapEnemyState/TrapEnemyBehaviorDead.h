#pragma once

#include "TrapEnemyBehaviorState.h"

/// <summary>
/// 罠設置型敵の死亡状態
/// </summary>
class TrapEnemyBehaviorDead : public TrapEnemyBehaviorState
{
public:

	TrapEnemyBehaviorDead(TrapEnemy* _pTrapEnemy);

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;


};

