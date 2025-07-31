#pragma once

#include "TrapEnemyBehaviorState.h"

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

