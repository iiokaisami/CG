#include "EnemyBehaviorAttack.h"

#include <Ease.h>

#include "../Enemy.h"
#include "EnemyBehaviorMove.h"
#include "EnemyBehaviorHitReact.h"
#include "EnemyBehaviorDead.h"

EnemyBehaviorAttack::EnemyBehaviorAttack(Enemy* _pEnemy) : EnemyBehaviorState("Attack", _pEnemy)
{
	// モーションの初期化
	motion_.isActive = true;
	motion_.count = 0;
	motion_.maxCount = 30; // 攻撃モーションのカウントを設定
}

void EnemyBehaviorAttack::Initialize()
{
}

void EnemyBehaviorAttack::Update()
{


	MotionCount(motion_);
}
