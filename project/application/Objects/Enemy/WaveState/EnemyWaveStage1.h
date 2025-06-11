#pragma once

#include "EnemyWaveState.h"

class EnemyWaveStage1 : public EnemyWaveState
{
public:

	EnemyWaveStage1(EnemyManager* _pEnemyManager);
	
	// 更新
	void Update() override;
};

