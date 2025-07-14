#include "EnemyManager.h"

#include "WaveState/EnemyWaveStage1.h"
#include "WaveState/EnemyWaveStage2.h"

void EnemyManager::Initialize()
{
	// ウェーブステート
	ChangeState(std::make_unique<EnemyWaveStage1>(this));
}

void EnemyManager::Finalize()
{
	for (auto& enemy : pNormalEnemies_)
	{
		enemy->Finalize();
	}
}

void EnemyManager::Update()
{
	for (auto& enemy : pNormalEnemies_)
	{
		enemy->SetPlayerPosition(playerPosition_);
		enemy->Update();
		toPlayerDistance_.push_back(enemy->GetToPlayer());
	}

	// isDeat がたったら削除
	pNormalEnemies_.erase(
		std::remove_if(
			pNormalEnemies_.begin(),
			pNormalEnemies_.end(),
			[this](std::unique_ptr<NormalEnemy>& enemy)
			{
				if (enemy->IsDead())
				{
					enemy->Finalize();

					// 敵のカウントを減らす
					enemyCount_--;

					return true;
				}
				return false;
			}),
		pNormalEnemies_.end()
	);

	// ウェーブステートの更新
	pState_->Update();
	
	// 敵が全て倒されたらウェーブチェンジ開始
	if (enemyCount_ == 0)
	{
		if (waveChangeInterval_ > 0)
		{
			isWaveChange_ = false;
			waveChangeInterval_--;
		} 
		else
		{
			// ステート遷移
			isWaveChange_ = true;
			waveChangeInterval_ = 120;
		}
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : pNormalEnemies_)
	{
		enemy->Draw();
	}
}

void EnemyManager::ImGuiDraw()
{
	for (auto& enemy : pNormalEnemies_)
	{
		enemy->ImGuiDraw();
	}
}

void EnemyManager::EnemyInit(const Vector3& pos)
{
	// エネミー
	std::unique_ptr<NormalEnemy> enemy = std::make_unique<NormalEnemy>();
	enemy->SetPosition(pos);
	enemy->Initialize();
	enemy->SetPlayerPosition(playerPosition_);
	enemy->Update();

	// 敵を登録
	pNormalEnemies_.push_back(std::move(enemy));
	
	// 敵のカウントを増やす
	enemyCount_++;
}

void EnemyManager::ChangeState(std::unique_ptr<EnemyWaveState> _pState)
{
	pState_ = std::move(_pState);
	pState_->Initialize();
}
