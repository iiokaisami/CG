#pragma once

#include "TrapEnemyBehaviorState.h"

/// <summary>
/// 罠設置型敵の出現状態
/// </summary>
class TrapEnemyBehaviorSpawn : public TrapEnemyBehaviorState
{
public:

	TrapEnemyBehaviorSpawn(TrapEnemy* _pTrapEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;
	
	// モーションのリセット
	void ResetMotion() override;

};

