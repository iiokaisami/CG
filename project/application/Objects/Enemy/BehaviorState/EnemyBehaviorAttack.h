#pragma once

#include "EnemyBehaviorState.h"

class EnemyBehaviorAttack : public EnemyBehaviorState
{
public:

	EnemyBehaviorAttack(Enemy* _pEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

};

