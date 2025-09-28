#include "EnemyWaveStage1.h"

#include "../EnemyManager.h"
#include "EnemyWaveStage2.h"

EnemyWaveStage1::EnemyWaveStage1(EnemyManager* _pEnemyManager) : EnemyWaveState("Stage1", _pEnemyManager)
{
}

void EnemyWaveStage1::Initialize()
{
	// CSVファイルの読み込み
	LoadCSV("resources/csv/EnemyPop1.csv");

	// EnemyManagerから共有LevelDataを受け取る
	SetLevelData(pEnemyManager_->GetLevelData());

	currentWave_ = 1;
}

void EnemyWaveStage1::Update()
{
	// 確認用csv更新
	UpdateCSV(pEnemyManager_);

	// 敵の発生コマンドを更新
	UpdateEnemyPopCommands(pEnemyManager_);

	// ステート切り替え
	if (pEnemyManager_->IsWaveChange())
	{
		// ステート遷移
		pEnemyManager_->ChangeState(std::make_unique<EnemyWaveStage2>(pEnemyManager_));
	}
}
