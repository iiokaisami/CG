#pragma once

#include "EnemyBehaviorState.h"

class EnemyBehaviorDead : public EnemyBehaviorState
{
public:

	EnemyBehaviorDead(Enemy* _pEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;

};

