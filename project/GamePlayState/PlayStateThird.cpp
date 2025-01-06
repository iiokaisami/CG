#include "PlayStateThird.h"

#include "../scene/scene/GamePlayScene.h"

PlayStateThird::PlayStateThird(GamePlayScene* _pScene) : GamePlayState("State Third", _pScene)
{
	// EnemyPopData3.csvの読み込み
	pPlayScene_->LoadEnemyPopData3();
}

void PlayStateThird::Update()
{
	// 敵の発生コマンドの更新
	pPlayScene_->UpdateEnemyPopCommands3();

	for (auto& enemy : pPlayScene_->GetEnemy())
	{
		enemy->SetPlayerPosition(pPlayScene_->GetPlayerPos());
		enemy->Update();
	}

	// ステート切り替え
	if (pPlayScene_->GetIsEnemyPhaseChange())
	{
		pPlayScene_->SetClearFlag(true);
	}
}
