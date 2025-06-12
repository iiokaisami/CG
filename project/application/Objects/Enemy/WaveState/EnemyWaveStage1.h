#pragma once

#include "EnemyWaveState.h"

class EnemyWaveStage1 : public EnemyWaveState
{
public:

	EnemyWaveStage1(EnemyManager* _pEnemyManager);
	
	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

};

