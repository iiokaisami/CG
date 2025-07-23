#pragma once

#include "TrapEnemyBehaviorState.h"

class TrapEnemyBehaviorHitReact : public TrapEnemyBehaviorState
{
public:
	
	TrapEnemyBehaviorHitReact(TrapEnemy* _pTrapEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;

};

