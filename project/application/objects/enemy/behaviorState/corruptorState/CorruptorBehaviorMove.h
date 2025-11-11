#pragma once

#include "CorruptorBehaviorState.h"

class CorruptorBehaviorMove : public CorruptorBehaviorState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pCorruptor">コラプターのポインタ</param>
	CorruptorBehaviorMove(Corruptor* _pCorruptor);

	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;
	
	// モーションのリセット
	void ResetMotion() override;

};

