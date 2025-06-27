#include "EnemyBehaviorAttack.h"

#include <Ease.h>

#include "../Enemy.h"
#include "EnemyBehaviorMove.h"
#include "EnemyBehaviorHitReact.h"
#include "EnemyBehaviorDead.h"

EnemyBehaviorAttack::EnemyBehaviorAttack(Enemy* _pEnemy) : EnemyBehaviorState("Attack", _pEnemy)
{
	// モーションの初期化
	motion_.count = 0;
	motion_.maxCount = 30; // 攻撃モーションのカウントを設定
}

void EnemyBehaviorAttack::Initialize()
{
}

void EnemyBehaviorAttack::Update()
{
	// 敵のトランスフォームをmotion_.transformにセット
	TransformUpdate(pEnemy_);

    // 溜め
    if (motion_.isActive)
    {
        if (motion_.count <= 20)
        {
            float t = float(motion_.count) / 20.0f; // 0〜1
            float ease = (t <= 0.5f)
                ? Ease::OutSine(t * 2.0f)               // 0〜0.5  → 縮小
                : Ease::OutSine((1.0f - t) * 2.0f);     // 0.5〜1  → 元に戻す

            float scaleValue = 1.0f - ease * 0.3f;
            motion_.transform.scale = Vector3(scaleValue, scaleValue, scaleValue);
			//pEnemy_->SetScale(motion_.transform.scale);
			pEnemy_->SetObjectScale(motion_.transform.scale);

            float shake = ((motion_.count % 2 == 0) ? 1 : -1) * 0.05f;
			pEnemy_->SetObjectPosition(motion_.transform.position + Vector3(shake, 0, shake));
            
        }
        // 攻撃
        else
        {
            // 攻撃フラグをリセット
            isAttack_ = false;

            // 回転しながら弾を飛ばす
            if (motion_.count == 21)
            {
                // 攻撃フラグを立てる
                isAttack_ = true;
            }

            // Y軸回転
            motion_.transform.rotation.y += 0.3f;
			pEnemy_->SetRotation(motion_.transform.rotation);

        }
    }

	// モーションカウントを更新
	MotionCount(motion_);

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
    else if (!pEnemy_->IsFarFromPlayer())
	{
		// プレイヤーとの距離が一定以上の場合、移動モーションに切り替え
		pEnemy_->ChangeBehaviorState(std::make_unique<EnemyBehaviorMove>(pEnemy_));
        return;
	}
    else if(!motion_.isActive && motion_.count >= motion_.maxCount)
    {
        // ステートが切り替わらなかったらもう一度
		ResetMotion();
    }


	// 攻撃クールダウンの管理
    if (attackCooldown_ >= 300)
    {
        attackCooldown_ = 60 * 4;
    }

    // クールダウンを減少
    attackCooldown_--;

    // 一定間隔で弾を発射
    if (attackCooldown_ <= 0)
    {
        // 攻撃モーションを開始
        motion_.isActive = true;
    }

	// フラグが立っている場合、攻撃を実行
    if (isAttack_)
    {
        pEnemy_->Attack();
    }

}

void EnemyBehaviorAttack::ResetMotion()
{
    // モーションの初期化
    motion_.count = 0;
    motion_.maxCount = 30; // 攻撃モーションのカウントを設定
}
