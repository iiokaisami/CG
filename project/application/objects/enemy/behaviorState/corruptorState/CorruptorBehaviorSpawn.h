#pragma once

#include "CorruptorBehaviorState.h"

/// <summary>
/// コラプターの出現状態
/// </summary>
class CorruptorBehaviorSpawn : public CorruptorBehaviorState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pCorruptor">コラプターのポインタ</param>
	CorruptorBehaviorSpawn(Corruptor* _pCorruptor);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;
	
	// モーションのリセット
	void ResetMotion() override;

};

