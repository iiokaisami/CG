#include "EnemyBehaviorState.h"

#include "../NormalEnemy.h"

EnemyBehaviorState::~EnemyBehaviorState()
{
}

void EnemyBehaviorState::MotionCount(Motion& _motion)
{
	if (_motion.count < _motion.maxCount)
	{
		_motion.count++;
	}
	else
	{
		// モーション終了 カウントリセット
		_motion.isActive = false;
		_motion.count = 0;
	}
}

void EnemyBehaviorState::TransformUpdate(NormalEnemy* _pNormalEnemy)
{
	if (_pNormalEnemy)
	{
		motion_.transform.position = _pNormalEnemy->GetPosition();
		motion_.transform.rotation = _pNormalEnemy->GetRotation();
		motion_.transform.scale = _pNormalEnemy->GetScale();
	}
}