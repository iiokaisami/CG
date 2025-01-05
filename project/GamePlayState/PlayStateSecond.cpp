#include "PlayStateSecond.h"

#include "../scene/scene/GamePlayScene.h"
#include "PlayStateThird.h"

PlayStateSecond::PlayStateSecond(GamePlayScene* _pScene) : GamePlayState("State Second", _pScene)
{
	// EnemyPopData2.csvの読み込み
	pPlayScene_->LoadEnemyPopData2();
}

void PlayStateSecond::Update()
{
	// 敵の発生コマンドの更新
	pPlayScene_->UpdateEnemyPopCommands2();

	for (auto& enemy : pPlayScene_->GetEnemy())
	{
		enemy->SetPlayerPosition(pPlayScene_->GetPlayerPos());
		enemy->Update();
	}

	// ステート切り替え
	if (Input::GetInstance()->TriggerKey(DIK_L))
	{
		// ステート遷移
		pPlayScene_->ChangeState(std::make_unique<PlayStateThird>(pPlayScene_));
	}
}
