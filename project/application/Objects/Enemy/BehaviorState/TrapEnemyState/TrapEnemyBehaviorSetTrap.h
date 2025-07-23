#pragma once

#include "TrapEnemyBehaviorState.h"

class TrapEnemyBehaviorSetTrap : public TrapEnemyBehaviorState
{
public:

	TrapEnemyBehaviorSetTrap(TrapEnemy* _pTrapEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;
	
	// モーションのリセット
	void ResetMotion() override;

private:

	// 罠設置のクールタイム	
	uint32_t trapCooldown_ = 0;
	// 罠設置フラグ
	bool isSetTrap_ = false;
	// クールタイムの最大値
	const uint32_t kMaxTrapCooldown = 60 * 6;
	// 次の罠がTimeBombかVignetteTrapかのフラグ
	bool isNextTrapTimeBomb_ = true;
	// クールタイム完了フラグ
	bool isTrapCooldownComplete_ = false;

};

