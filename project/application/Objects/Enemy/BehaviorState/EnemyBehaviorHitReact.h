#pragma once

#include "EnemyBehaviorState.h"

class EnemyBehaviorHitReact : public EnemyBehaviorState
{
public:

	EnemyBehaviorHitReact(NormalEnemy* _pNormalEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;

};

