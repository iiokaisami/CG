#pragma once

#include "TrapEnemyBehaviorState.h"

class TrapEnemyBehaviorMove : public TrapEnemyBehaviorState
{
public:

	TrapEnemyBehaviorMove(TrapEnemy* _pTrapEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;
	
	// モーションのリセット
	void ResetMotion() override;


private:

	// 罠設置のクールタイム	
	uint32_t trapCooldown_ = 0;
	// クールタイムの最大値
	const uint32_t kMaxTrapCooldown = 60 * 3;

};

