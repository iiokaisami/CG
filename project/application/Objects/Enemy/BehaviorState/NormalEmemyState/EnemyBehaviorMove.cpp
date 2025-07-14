#include "EnemyBehaviorMove.h"

#include <Ease.h>

#include "../../NormalEnemy.h"
#include "EnemyBehaviorAttack.h"
#include "EnemyBehaviorHitReact.h"
#include "EnemyBehaviorDead.h"

EnemyBehaviorMove::EnemyBehaviorMove(NormalEnemy* _pNormalEnemy) : EnemyBehaviorState("Move", _pNormalEnemy)
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
	TransformUpdate(pNormalEnemy_);

	// 動く前に切り替え処理
	if (pNormalEnemy_->GetHP() <= 0)
	{
		// HPが0以下なら、死亡モーションに切り替え
		pNormalEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorDead>(pNormalEnemy_));
		return;
	}
	else if (pNormalEnemy_->IsHit())
	{
		// ヒットフラグをリセット
		pNormalEnemy_->SetIsHit(false);

		// 無敵化
		pNormalEnemy_->SetIsInvincible(true);

		// ヒットしたら、ヒットリアクションモーションに切り替え
		pNormalEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorHitReact>(pNormalEnemy_));
		return;
	}
	else if (pNormalEnemy_->IsFarFromPlayer())
	{
		// プレイヤーとの距離が一定以下の場合、攻撃モーションに切り替え
		pNormalEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorAttack>(pNormalEnemy_));
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

	pNormalEnemy_->SetPosition(motion_.transform.position);

	// モーションカウントを更新
	MotionCount(motion_);

	// 移動
	pNormalEnemy_->Move();

}

void EnemyBehaviorMove::ResetMotion()
{
	motion_.isActive = true;
	motion_.count = 0;
	motion_.maxCount = 30; // 移動モーションのカウントを設定
}
