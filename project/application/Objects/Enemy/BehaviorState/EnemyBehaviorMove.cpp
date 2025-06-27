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
	// 敵のトランスフォームをmotion_.transformにセット
	TransformUpdate(pEnemy_);

	// 動く前に切り替え処理
	if (pEnemy_->GetHP() <= 0)
	{
		// HPが0以下なら、死亡モーションに切り替え
		pEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorDead>(pEnemy_));
		return;
	}
	else if (pEnemy_->IsHit())
	{
		// ヒットフラグをリセット
		pEnemy_->SetIsHit(false);

		// 無敵化
		pEnemy_->SetIsInvincible(true);

		// ヒットしたら、ヒットリアクションモーションに切り替え
		pEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorHitReact>(pEnemy_));
		return;
	}
	else if (pEnemy_->IsFarFromPlayer())
	{
		// プレイヤーとの距離が一定以下の場合、攻撃モーションに切り替え
		pEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorAttack>(pEnemy_));
		return;
	}
	else
	{
		// ステートが切り替わらなかったらもう一度
		ResetMotion();
	}

	// イージング進行度（0〜1）
	float t = float(motion_.count) / motion_.maxCount;

	// 上下にゆっくり波打つ動き
	float wave = std::sin(t * std::numbers::pi_v<float>) * 0.3f;

	motion_.transform.position += Vector3(0.0f, wave, 0.0f); // Y軸方向に波打つ

	pEnemy_->SetPosition(motion_.transform.position);

	// モーションカウントを更新
	MotionCount(motion_);

	// 移動
	pEnemy_->Move();

}

void EnemyBehaviorMove::ResetMotion()
{
	motion_.isActive = true;
	motion_.count = 0;
	motion_.maxCount = 30; // 移動モーションのカウントを設定
}
