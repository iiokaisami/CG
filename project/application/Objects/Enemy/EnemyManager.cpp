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
	for (auto& enemy : pEnemies_)
	{
		enemy->Finalize();
	}
}

void EnemyManager::Update()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->SetPlayerPosition(playerPosition_);
		enemy->Update();
		toPlayerDistance_.push_back(enemy->GetToPlayer());
	}

	// isDeat がたったら削除
	pEnemies_.erase(
		std::remove_if(
			pEnemies_.begin(), 
			pEnemies_.end(),
			[this](std::unique_ptr<Enemy>& enemy)
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
		pEnemies_.end()
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
	for (auto& enemy : pEnemies_)
	{
		enemy->Draw();
	}
}

void EnemyManager::ImGuiDraw()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->ImGuiDraw();
	}
}

void EnemyManager::EnemyInit(const Vector3& pos)
{
	// エネミー
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->SetPosition(pos);
	enemy->Initialize();
	enemy->SetPlayerPosition(playerPosition_);
	enemy->Update();

	// 敵を登録
	pEnemies_.push_back(std::move(enemy));
	
	// 敵のカウントを増やす
	enemyCount_++;
}

void EnemyManager::ChangeState(std::unique_ptr<EnemyWaveState> _pState)
{
	pState_ = std::move(_pState);
	pState_->Initialize();
}
