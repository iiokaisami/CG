#include "EnemyBehaviorSpawn.h"

#include <Ease.h>

#include "../Enemy.h"
#include "EnemyBehaviorMove.h"

EnemyBehaviorSpawn::EnemyBehaviorSpawn(Enemy* _pEnemy) : EnemyBehaviorState("Spawn", _pEnemy)
{
	// モーションの初期化
	motion_.isActive = true;
	motion_.count = 0;
	motion_.maxCount = 30; // スポーンモーションのカウントを設定
}

void EnemyBehaviorSpawn::Initialize()
{
}

void EnemyBehaviorSpawn::Update()
{
	// 敵のトランスフォームをmotion_.transformにセット
	TransformUpdate(pEnemy_);

	// イージングの進行度（0〜1）
	float t = float(motion_.count) / motion_.maxCount;
	motion_.transform.scale.x = Ease::OutBack(t);
	motion_.transform.scale.y = Ease::OutBack(t);
	motion_.transform.scale.z = Ease::OutBack(t);
	Vector3 one(1.0f, 1.0f, 1.0f);
	
	//pEnemy_->SetScale(one * motion_.transform.scale);

	pEnemy_->SetObjectPosition(motion_.transform.position);
	pEnemy_->SetObjectRotation(motion_.transform.rotation);
	pEnemy_->SetObjectScale(one * motion_.transform.scale);

	// モーションカウントを更新
	MotionCount(motion_);

	if (!motion_.isActive)
	{
		// 無敵状態を解除
		pEnemy_->SetIsInvincible(false);

		// スポーンモーションが終了したら、次の状態に移行
		pEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorMove>(pEnemy_));
	}
}

void EnemyBehaviorSpawn::ResetMotion()
{
}
