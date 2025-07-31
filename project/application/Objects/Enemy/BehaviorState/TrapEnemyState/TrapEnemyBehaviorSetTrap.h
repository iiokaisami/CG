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

	// 罠設置フラグ
	bool isSetTrap_ = false;

};

