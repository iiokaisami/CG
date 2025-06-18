#include "EnemyBehaviorSpawn.h"

#include <Ease.h>

#include "../Enemy.h"
#include "EnemyBehaviorMove.h"
#include "EnemyBehaviorAttack.h"
#include "EnemyBehaviorHitReact.h"
#include "EnemyBehaviorDead.h"

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
	float t = float(motion_.count) / motion_.maxCount;
	float scale = Ease::OutBack(t); // 0〜1 の範囲で膨らみつつ出現
	Vector3 one(1.0f, 1.0f, 1.0f);
	//scale_ = one * scale;

	pEnemy_->SetScale(one * scale);


	MotionCount(motion_);

}
