#pragma once

#include "EnemyBehaviorState.h"

/// <summary>
/// 敵の出現状態
/// </summary>
class EnemyBehaviorSpawn : public EnemyBehaviorState
{
public:

	EnemyBehaviorSpawn(NormalEnemy* _pNormalEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;

};

