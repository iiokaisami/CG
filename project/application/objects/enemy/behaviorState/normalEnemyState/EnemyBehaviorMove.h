#pragma once

#include "EnemyBehaviorState.h"

/// <summary>
/// 敵キャラクターの移動状態を管理するクラス
/// </summary>
class EnemyBehaviorMove : public EnemyBehaviorState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pNormalEnemy">通常敵ポインタ</param>
	EnemyBehaviorMove(NormalEnemy* _pNormalEnemy);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;

};

