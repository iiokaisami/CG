#include "EnemyWaveStage1.h"

#include "../EnemyManager.h"
#include "EnemyWaveStage2.h"

EnemyWaveStage1::EnemyWaveStage1(EnemyManager* _pEnemyManager) : EnemyWaveState("Stage1", _pEnemyManager)
{
}

void EnemyWaveStage1::Update()
{

	// ステート切り替え
	if (/*Input::GetInstance()->TriggerKey(DIK_L) or */pEnemyManager_->IsWaveChange())
	{
		// ステート遷移
		pEnemyManager_->ChangeState(std::make_unique<EnemyWaveStage2>(pEnemyManager_));
	}
}
