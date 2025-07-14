#pragma once

#include "EnemyBehaviorState.h"

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

