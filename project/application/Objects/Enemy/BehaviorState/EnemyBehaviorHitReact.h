#pragma once

#include "EnemyBehaviorState.h"

class EnemyBehaviorHitReact : public EnemyBehaviorState
{
public:

	EnemyBehaviorHitReact(Enemy* _pEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

};

