#pragma once

#include "EnemyBehaviorState.h"

/// <summary>
/// 敵の行動ステート：死亡
/// </summary>
class EnemyBehaviorDead : public EnemyBehaviorState
{
public:

	EnemyBehaviorDead(NormalEnemy* _pNormalEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;

};

