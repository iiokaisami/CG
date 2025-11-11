#pragma once

#include "CorruptorBehaviorState.h"

class CorruptorBehaviorSelfDestruct : public CorruptorBehaviorState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pCorruptor">コラプターのポインタ</param>
	CorruptorBehaviorSelfDestruct(Corruptor* _pCorruptor);

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// モーションのリセット
	void ResetMotion() override;

};

