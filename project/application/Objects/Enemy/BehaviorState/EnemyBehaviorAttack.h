#pragma once

#include "EnemyBehaviorState.h"

class EnemyBehaviorAttack : public EnemyBehaviorState
{
public:

	EnemyBehaviorAttack(NormalEnemy* _pNormalEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;


private:

	// 攻撃フラグ
	bool isAttack_ = false;
	// 攻撃クールタイム
	uint32_t attackCooldown_ = 0;

};

