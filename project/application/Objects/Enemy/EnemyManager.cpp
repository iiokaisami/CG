#include "EnemyManager.h"

void EnemyManager::Initialize()
{
	// エネミーのcsvデータを読み込み
	LoadEnemyPopData1();
	LoadEnemyPopData2();
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
	// isDeatエネミーがたったら削除
	pEnemies_.erase(
		std::remove_if(pEnemies_.begin(), pEnemies_.end(),
			[](std::unique_ptr<Enemy>& enemy)
			{
				if (enemy->IsDead())
				{
					enemy->Finalize();
					return true;
				}
				return false;
			}),
		pEnemies_.end()
	);
	// エネミーの出現コマンドの更新
	UpdateEnemyPopCommands1();
	
	if (pEnemies_.size() == 0) // エネミーの数が10未満ならば
	{
		if (phase_ == 1)
		{
			phase_ = 2; // 次のフェーズへ
		}
	}
	else if (pEnemies_.size() >= 1 && phase_ == 0)
	{
		phase_ = 1;
	}

	if (phase_ == 2)
	{
		// エネミーの出現コマンドの更新
		UpdateEnemyPopCommands2();
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

void EnemyManager::EnemyInit()
{
	// エネミー
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->SetPosition(enemyPosition_);
	enemy->Initialize();
	enemy->SetPlayerPosition(playerPosition_);
	enemy->Update();

	// 敵を登録
	pEnemies_.push_back(std::move(enemy));
}

void EnemyManager::LoadEnemyPopData1()
{
	//ファイルを開く
	std::ifstream file;
	file.open("resources/csv/EnemyPop1.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands1 << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void EnemyManager::LoadEnemyPopData2()
{
	//ファイルを開く
	std::ifstream file;
	file.open("resources/csv/EnemyPop2.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands2 << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void EnemyManager::UpdateEnemyPopCommands1()
{
	//待機処理
	if (isEnemyWaiting_)
	{
		enemyWaitingTimer_--;

		if (enemyWaitingTimer_ <= 0)
		{
			//待機完了
			isEnemyWaiting_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンドループ
	while (getline(enemyPopCommands1, line)) {
		//1行分の文字列を入れる変数
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}


		// POSITONコマンド
		if (word.find("POSITION") == 0)
		{

			// X座標
			getline(line_stream, word, ',');
			float x = (float)atoi(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)atoi(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			float z = (float)atoi(word.c_str());

			enemyPosition_ = { x,y,z };

			// 敵発生
			EnemyInit();

		}
		// WAITコマンド
		else if (word.find("WAIT") == 0)
		{

			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機時間
			isEnemyWaiting_ = true;
			enemyWaitingTimer_ = waitTime;



			//コマンドループを抜ける
			break;
		}
	}
}

void EnemyManager::UpdateEnemyPopCommands2()
{
	//待機処理
	if (isEnemyWaiting_)
	{
		enemyWaitingTimer_--;

		if (enemyWaitingTimer_ <= 0)
		{
			//待機完了
			isEnemyWaiting_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンドループ
	while (getline(enemyPopCommands2, line)) {
		//1行分の文字列を入れる変数
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}


		// POSITONコマンド
		if (word.find("POSITION") == 0)
		{

			// X座標
			getline(line_stream, word, ',');
			float x = (float)atoi(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)atoi(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			float z = (float)atoi(word.c_str());

			enemyPosition_ = { x,y,z };

			// 敵発生
			EnemyInit();

		}
		// WAITコマンド
		else if (word.find("WAIT") == 0)
		{

			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機時間
			isEnemyWaiting_ = true;
			enemyWaitingTimer_ = waitTime;



			//コマンドループを抜ける
			break;
		}
	}
}
