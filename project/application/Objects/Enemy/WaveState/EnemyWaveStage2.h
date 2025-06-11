#pragma once

#include "EnemyWaveState.h"

class EnemyWaveStage2 : public EnemyWaveState
{
public:

	EnemyWaveStage2(EnemyManager* _pEnemyManager);

	// 更新
	void Update() override;
};

