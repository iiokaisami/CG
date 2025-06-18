#include "EnemyBehaviorState.h"

#include "../Enemy.h"

EnemyBehaviorState::~EnemyBehaviorState()
{
}

void EnemyBehaviorState::MotionCount(Motion _motion)
{
	if (_motion.count < _motion.maxCount)
	{
		_motion.count++;
	} else
	{
		// モーション終了 カウントリセット
		_motion.isActive = false;
		_motion.count = 0;
	}
}
