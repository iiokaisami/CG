#include "EnemyWaveStage2.h"

#include "../EnemyManager.h"

EnemyWaveStage2::EnemyWaveStage2(EnemyManager* _pEnemyManager) : EnemyWaveState("Stage1", _pEnemyManager)
{
}

void EnemyWaveStage2::Update()
{
	// ステート切り替え
	if (pEnemyManager_->IsWaveChange())
	{
		// クリアフラグ
		//pEnemyManager_->SetClearFlag(true);
	}
}
