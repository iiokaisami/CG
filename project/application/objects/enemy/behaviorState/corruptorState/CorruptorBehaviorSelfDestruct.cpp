#include "CorruptorBehaviorSelfDestruct.h"

#include <Ease.h>

#include "../../Corruptor.h"

CorruptorBehaviorSelfDestruct::CorruptorBehaviorSelfDestruct(Corruptor* _pCorruptor) : CorruptorBehaviorState("SelfDestruct", _pCorruptor)
{
	motion_.isActive = true;
	motion_.count = 0;
	motion_.maxCount = 60; // 自爆モーションのカウントを設定
}

void CorruptorBehaviorSelfDestruct::Initialize()
{
}

void CorruptorBehaviorSelfDestruct::Update()
{
	// 敵のトランスフォームをmotion_.transformにセット
	TransformUpdate(pCorruptor_);
	
	// イージングの進行度
	float t = float(motion_.count) / motion_.maxCount;
	
	// はじける演出
	float scale = Lerp(1.0f, 2.0f, Ease::OutCubic(t));
	motion_.transform.scale = Vector3(scale, scale, scale);
	
	pCorruptor_->SetObjectScale(motion_.transform.scale);
	pCorruptor_->SetScale(motion_.transform.scale);

	// モーションカウント
	MotionCount(motion_);

	if(!motion_.isActive)
	{
		// 自爆したのでデスフラグを立てる
		pCorruptor_->SetIsDead(true);

		// パーティクル
		ParticleEmitter::Emit("explosionGroup", motion_.transform.position, 15);
	}

}

void CorruptorBehaviorSelfDestruct::ResetMotion()
{
}
