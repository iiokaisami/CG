#include "EnemyBehaviorHitReact.h"

#include <Ease.h>

#include "../Enemy.h"
#include "EnemyBehaviorMove.h"
#include "EnemyBehaviorDead.h"

EnemyBehaviorHitReact::EnemyBehaviorHitReact(Enemy* _pEnemy) : EnemyBehaviorState("Hit", _pEnemy)
{
	motion_.isActive = true;
	motion_.count = 0;
	motion_.maxCount = 30; // 移動モーションのカウントを設定
}

void EnemyBehaviorHitReact::Initialize()
{
}

void EnemyBehaviorHitReact::Update()
{
	// 敵のトランスフォームをmotion_.transformにセット
	TransformUpdate(pEnemy_);

	Vector3 shakeOffset =
	{
	((motion_.count % 2 == 0) ? 1.0f : -1.0f) * 0.1f,
	0.0f, // Y軸は揺らさない
	((motion_.count % 3 == 0) ? 1.0f : -1.0f) * 0.1f
	};

	Vector3 originPos = pEnemy_->GetPosition();

	pEnemy_->SetObjectPosition(originPos + shakeOffset);

	// モーションカウントを更新
	MotionCount(motion_);

	if (!motion_.isActive)
	{
		// 無敵状態を解除
		pEnemy_->SetIsInvincible(false);

		// ヒットリアクションモーションが終了したら、移動モーションに切り替え
		pEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorMove>(pEnemy_));
	}

}

void EnemyBehaviorHitReact::ResetMotion()
{
}
