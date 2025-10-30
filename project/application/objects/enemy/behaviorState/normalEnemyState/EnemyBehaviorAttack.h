#pragma once

#include "EnemyBehaviorState.h"

/// <summary>
/// 通常敵の攻撃状態
/// </summary>
class EnemyBehaviorAttack : public EnemyBehaviorState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pNormalEnemy">通常敵ポインタ</param>
	EnemyBehaviorAttack(NormalEnemy* _pNormalEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;


private:

	// 攻撃フラグ
	bool isAttack_ = false;
	// 攻撃クールタイム
	int attackCooldown_ = 0;

};

