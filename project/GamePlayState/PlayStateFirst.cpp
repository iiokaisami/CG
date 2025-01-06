#include "PlayStateFirst.h"

#include "../scene/scene/GamePlayScene.h"
#include "PlayStateSecond.h"

PlayStateFirst::PlayStateFirst(GamePlayScene* _pScene) : GamePlayState("State First", _pScene)
{
	// EnemyPopData1.csvの読み込み
	pPlayScene_->LoadEnemyPopData1();
}

void PlayStateFirst::Update()
{
	// 敵の発生コマンドの更新
	pPlayScene_->UpdateEnemyPopCommands1();
	
	for (auto& enemy : pPlayScene_->GetEnemy())
	{
		enemy->SetPlayerPosition(pPlayScene_->GetPlayerPos());
		enemy->Update();
	}

	// ステート切り替え
	if (/*Input::GetInstance()->TriggerKey(DIK_L) or */pPlayScene_->GetIsEnemyPhaseChange())
	{
		// ステート遷移
		pPlayScene_->ChangeState(std::make_unique<PlayStateSecond>(pPlayScene_));
	}
}
