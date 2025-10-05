#include "EnemyManager.h"

#include "waveState/EnemyWaveStage1.h"
#include "waveState/EnemyWaveStage2.h"

void EnemyManager::Initialize()
{
	// エディタ読み込み
	auto levelData = std::make_shared<LevelData>();
	levelData = LevelDataLoader::LoadLevelData("enemySpawn");
	SetLevelData(levelData);

	// ウェーブステート
	ChangeState(std::make_unique<EnemyWaveStage1>(this));

}

void EnemyManager::Finalize()
{
	for (auto& enemy : pTrapEnemies_)
	{
		enemy->Finalize();
	}

	for (auto& enemy : pNormalEnemies_)
	{
		enemy->Finalize();
	}
}

void EnemyManager::Update()
{
	// ノーマルエネミーの更新
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

	// トラップエネミーの更新
	for (auto& enemy : pTrapEnemies_)
	{
		enemy->SetPlayerPosition(playerPosition_);
		enemy->Update();
	}

	// isDead がたったら削除
	pTrapEnemies_.erase(
		std::remove_if(
			pTrapEnemies_.begin(),
			pTrapEnemies_.end(),
			[this](std::unique_ptr<TrapEnemy>& enemy)
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
		pTrapEnemies_.end()
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

	for (auto& enemy : pTrapEnemies_)
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

	for (auto& enemy : pTrapEnemies_)
	{
		enemy->ImGuiDraw();
	}
}

void EnemyManager::TitleEnemyInit()
{
	NormalEnemyInit({ 2.0f,0.5f,-5.0f });
	TrapEnemyInit({ -2.0f,0.5f,2.0f });
	NormalEnemyInit({ -5.0f,0.5f,-2.0f });
	TrapEnemyInit({ 5.0f,0.5f,5.0f });
}

void EnemyManager::TitleEnemyUpdate()
{
	// ノーマルエネミーの更新
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

	// トラップエネミーの更新
	for (auto& enemy : pTrapEnemies_)
	{
		enemy->SetPlayerPosition(playerPosition_);
		enemy->Update();
	}

	// isDead がたったら削除
	pTrapEnemies_.erase(
		std::remove_if(
			pTrapEnemies_.begin(),
			pTrapEnemies_.end(),
			[this](std::unique_ptr<TrapEnemy>& enemy)
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
		pTrapEnemies_.end()
	);

	// 敵が全て倒されたらもう一度出現
	if (enemyCount_ == 0)
	{
		TitleEnemyInit();
	}
}

void EnemyManager::NormalEnemyInit(const Vector3& pos)
{
	// ノーマルエネミー
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

void EnemyManager::TrapEnemyInit(const Vector3& pos)
{
	// トラップエネミー
	std::unique_ptr<TrapEnemy> trapEnemy = std::make_unique<TrapEnemy>();
	trapEnemy->SetPosition(pos);
	trapEnemy->Initialize();
	trapEnemy->SetPlayerPosition(playerPosition_);
	trapEnemy->Update();

	// 敵を登録
	pTrapEnemies_.push_back(std::move(trapEnemy));

	// 敵のカウントを増やす
	enemyCount_++;
}

void EnemyManager::ChangeState(std::unique_ptr<EnemyWaveState> _pState)
{
	pState_ = std::move(_pState);
	pState_->Initialize();
}
