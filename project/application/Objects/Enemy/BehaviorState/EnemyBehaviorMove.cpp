#include "EnemyBehaviorMove.h"

#include <Ease.h>

#include "../Enemy.h"
#include "EnemyBehaviorAttack.h"
#include "EnemyBehaviorHitReact.h"
#include "EnemyBehaviorDead.h"

EnemyBehaviorMove::EnemyBehaviorMove(Enemy* _pEnemy) : EnemyBehaviorState("Move", _pEnemy)
{
	motion_.isActive = true;
	motion_.count = 0;
	motion_.maxCount = 30; // 移動モーションのカウントを設定
}

void EnemyBehaviorMove::Initialize()
{
}

void EnemyBehaviorMove::Update()
{
	// 進行度（0〜1）
	float t = float(motion_.count) / motion_.maxCount;

	// 上下にゆっくり波打つ動き
	float wave = std::sin(t * std::numbers::pi_v<float>) * 0.3f;

	//position_ += Vector3(0.0f, wave, 0.0f);

	pEnemy_->SetPosition(pEnemy_->GetPosition() + Vector3(0.0f, wave, 0.0f));

	MotionCount(motion_);
}
