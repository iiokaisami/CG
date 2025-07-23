#include "TrapEnemyBehaviorSetTrap.h"

#include <Ease.h>

#include "../../TrapEnemy.h"
#include "TrapEnemyBehaviorMove.h"
#include "TrapEnemyBehaviorHitReact.h"
#include "TrapEnemyBehaviorDead.h"

TrapEnemyBehaviorSetTrap::TrapEnemyBehaviorSetTrap(TrapEnemy* _pTrapEnemy) : TrapEnemyBehaviorState("SetTrap", _pTrapEnemy)
{
	// モーションの初期化
	motion_.count = 0;
	motion_.maxCount = 30; // 攻撃モーションのカウントを設定
}

void TrapEnemyBehaviorSetTrap::Initialize()
{
	trapCooldown_ = 10;	
}

void TrapEnemyBehaviorSetTrap::Update()
{
    // 敵のトランスフォームをmotion_.transformにセット
    TransformUpdate(pTrapEnemy_);

    const int expandTime = 15;
    const int totalTime = 30;

    if (motion_.isActive)
    {
        if (motion_.count < expandTime)
        {
            // ググっと膨らむ
            float t = float(motion_.count) / expandTime;
            float scaleValue = 1.0f + Ease::OutBack(t) * 0.5f;

            // プルプル震える
            float shakeMag = 0.05f; // 震えの大きさ
            float shakeX = std::sin(motion_.count * 0.5f) * shakeMag;
            float shakeY = std::cos(motion_.count * 0.7f) * shakeMag;

            motion_.transform.scale = Vector3(scaleValue, scaleValue, scaleValue);
            pTrapEnemy_->SetObjectScale(motion_.transform.scale);
            pTrapEnemy_->SetObjectPosition(motion_.transform.position + Vector3(shakeX, shakeY, 0.0f));
        }
        else if (motion_.count == expandTime)
        {
            // フラグを切り替え、シュッと元に戻す
            isSetTrap_ = true;
            motion_.transform.scale = Vector3(1.0f, 1.0f, 1.0f);
            pTrapEnemy_->SetObjectScale(motion_.transform.scale);
            pTrapEnemy_->SetObjectPosition(motion_.transform.position); // 位置も元に戻す
        }
        else
        {
            // 通常サイズ
            motion_.transform.scale = Vector3(1.0f, 1.0f, 1.0f);
            pTrapEnemy_->SetObjectScale(motion_.transform.scale);
            pTrapEnemy_->SetObjectPosition(motion_.transform.position);
        }
    }

    if (pTrapEnemy_->GetHP() <= 0)
    {
        // HPが0以下なら、死亡モーションに切り替え
        pTrapEnemy_->ChangeBehaviorState(std::make_unique<TrapEnemyBehaviorDead>(pTrapEnemy_));
        return;
    } 
    else if (pTrapEnemy_->IsHit())
    {
        // ヒットフラグをリセット
        pTrapEnemy_->SetIsHit(false);
        // 無敵化
        pTrapEnemy_->SetIsInvincible(true);
        // ヒットしたら、ヒットリアクションモーションに切り替え
        pTrapEnemy_->ChangeBehaviorState(std::make_unique<TrapEnemyBehaviorHitReact>(pTrapEnemy_));
        return;
    } 
    else if (!pTrapEnemy_->IsFarFromPlayer())
    {
        // プレイヤーとの距離が一定以上の場合、移動モーションに切り替え
        pTrapEnemy_->ChangeBehaviorState(std::make_unique<TrapEnemyBehaviorMove>(pTrapEnemy_));
        return;
    } 
    else if (!motion_.isActive && motion_.count >= motion_.maxCount)
    {
        // ステートが切り替わらなかったらもう一度
        ResetMotion();

    }

    // モーションカウントを更新
    MotionCount(motion_);

    // 罠設置が完了フラグを切り替える
    isTrapCooldownComplete_ = (trapCooldown_ > kMaxTrapCooldown);
    pTrapEnemy_->SetIsTrapCooldownComplete(isTrapCooldownComplete_);

    // クールタイム進行
    trapCooldown_--;
    
    if(isSetTrap_)
    {
        pTrapEnemy_->TrapInit(isNextTrapTimeBomb_);
        trapCooldown_ = kMaxTrapCooldown;
    }

    // 一定間隔で弾を発射
    if (trapCooldown_ <= 0)
    {
        // 攻撃モーションを開始
        motion_.isActive = true;
        trapCooldown_ = 60 * 6;
    }
}

void TrapEnemyBehaviorSetTrap::ResetMotion()
{
	// モーションの初期化
	motion_.count = 0;
	motion_.maxCount = 30; // 攻撃モーションのカウントを設定
}
