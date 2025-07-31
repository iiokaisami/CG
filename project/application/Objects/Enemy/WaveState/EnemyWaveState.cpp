#include "EnemyWaveState.h"

#include "../EnemyManager.h"

#include <cassert>

EnemyWaveState::~EnemyWaveState()
{
}

void EnemyWaveState::LoadCSV(const std::string& csvPath)
{
	// CSVファイルの読み込み
	//ファイルを開く
	std::ifstream file(csvPath);
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void EnemyWaveState::UpdateEnemyPopCommands(EnemyManager* _pEnemyManager)
{
	//待機処理
	if (isEnemyWaiting_) 
	{
		enemyWaitingTimer_--;

		if (enemyWaitingTimer_ <= 0)
		{
			isEnemyWaiting_ = false;
		}
		return;
	}

	std::string line;

	while (getline(enemyPopCommands_, line)) 
	{
		std::istringstream line_stream(line);
		std::string word;

		getline(line_stream, word, ','); // コマンド取得

		// コメント行をスキップ
		if (word.find("//") == 0)
		{
			continue;
		}

		// POSITONコマンド（NormalEnemy or TrapEnemy）
		if (word == "POSITION")
		{

			// 敵タイプ（例: "Normal", "Trap"）
			std::string enemyType;
			getline(line_stream, enemyType, ',');

			// X, Y, Z 座標を取得
			getline(line_stream, word, ',');
			float x = static_cast<float>(atof(word.c_str()));

			getline(line_stream, word, ',');
			float y = static_cast<float>(atof(word.c_str()));

			getline(line_stream, word, ',');
			float z = static_cast<float>(atof(word.c_str()));

			if (enemyType == "Normal")
			{
				_pEnemyManager->NormalEnemyInit({ x, y, z });
			} 
			else if (enemyType == "Trap") 
			{
				_pEnemyManager->TrapEnemyInit({ x, y, z });
			}
		}
		// WAITコマンド
		else if (word == "WAIT")
		{
			getline(line_stream, word, ',');
			int waitTime = atoi(word.c_str());

			isEnemyWaiting_ = true;
			enemyWaitingTimer_ = waitTime;
			break;
		}
	}
}
