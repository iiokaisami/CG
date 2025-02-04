#include "GamePlayScene.h"

#include "../../GamePlayState/PlayStateFirst.h"
#include "../../GamePlayState/PlayStateSecond.h"
#include "../../GamePlayState/PlayStateThird.h"

void GamePlayScene::Initialize()
{
	collisionManager_ = CollisionManager::GetInstance();
	collisionManager_->Initialize();

	camera_ = std::make_shared<Camera>();
	camera_->SetRotate({ 0.3f,0.0f,0.0f });
	camera_->SetPosition({ 0.0f,4.0f,-10.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera_);
	cameraManager.AddCamera(camera_);
	cameraManager.SetActiveCamera(0);

	isSceneStart_ = true;
	isClearSceneChange_ = false;
	isGameOverSceneChange_ = false;
	alpha_ = 1.0f;

	for (uint32_t i = 0; i < 13; ++i)
	{
		Sprite* sprite = new Sprite();
		if (i == 0) {
			sprite->Initialize("Play/gameUI.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if(i == 1) {
			sprite->Initialize("black.png", { 0,0 }, { 1,1,1,alpha_ }, { 0,0 });
		}
		else if (i == 2) {
			sprite->Initialize("num/enemyCount.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 3) {
			sprite->Initialize("num/0.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 4) {
			sprite->Initialize("num/1.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 5) {
			sprite->Initialize("num/2.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 6) {
			sprite->Initialize("num/3.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 7) {
			sprite->Initialize("num/4.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 8) {
			sprite->Initialize("num/5.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 9) {
			sprite->Initialize("num/6.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 10) {
			sprite->Initialize("num/7.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 11) {
			sprite->Initialize("num/8.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 12) {
			sprite->Initialize("num/9.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}

		sprites.push_back(sprite);

		Vector2 size = sprite->GetSize();

		sprite->SetSize(size);
	}

	// Player
	pPlayer_ = std::make_unique<Player>();
	pPlayer_->Initialize();
	pPlayer_->SetCamera(cameraManager.GetActiveCamera());

	// ステート
	ChangeState(std::make_unique<PlayStateFirst>(this));

	// Skydome
	pSkydome_ = std::make_unique<Skydome>();
	pSkydome_->Initialize();

	// 床
	pGround_ = std::make_unique<Ground>();
	pGround_->Initialize();

	enemyPhaseChangeInterval_ = 120;

	// --- サウンド ---
	soundData_ = Audio::GetInstance()->LoadWav("BGM.wav");
	Audio::GetInstance()->PlayWave(soundData_, true, 0.2f);
}

void GamePlayScene::Finalize()
{
	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}

	pPlayer_->Finalize();
	for (auto& enemy : pEnemy_)
	{
		enemy->Finalize();
		delete enemy;
	}

	pSkydome_->Finalize();
	pSkydome_.reset();

	pGround_->Finalize();
	pGround_.reset();

	cameraManager.RemoveCamera(0);

	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData_);
}

void GamePlayScene::Update()
{
	for (Sprite* sprite : sprites)
	{
		sprite->Update();

	}

	pPlayer_->Update();

	if (!isSceneStart_)
	{
		pState_->Update();
	}

	pSkydome_->Update();

	pGround_->Update();

#ifdef _DEBUG

	pPlayer_->ImGuiDraw();
	
	for (auto& enemy : pEnemy_)
	{
		enemy->ImGuiDraw();
	}

	ImGui::Begin("GamePlayScene");

	ImGui::Text("EnemyCount : %d", enemyCount_);

	ImGui::Text("HIT Position : %.0f", hitPosition_.x);
	ImGui::Text("HIT Position : %.0f", hitPosition_.y);
	ImGui::Text("HIT Position : %.0f", hitPosition_.z);

	if (ImGui::Button("CCC"))
	{
		isClearSceneChange_ = true;
	}
	if (ImGui::Button("GGG"))
	{
		isGameOverSceneChange_ = true;
	}

	ImGui::End();

#endif // _DEBUG


	// 当たり判定チェック
	collisionManager_->CheckAllCollision();

	// プレイヤーがダメージを受けたときにカメラをシェイク
	if (pPlayer_->IsHit())
	{
		cameraManager.StartShakeActiveCamera(0.2f, 0.1f); // 小規模のシェイク
	}

	// カメラの更新
	cameraManager.UpdateAll(0.016f); // 例としてフレームレートを60FPSと仮定


	// デスフラグの立った敵を削除
	pEnemy_.remove_if([this](Enemy* enemy) {
		if (enemy->IsDead()) {
			enemy->Finalize();
			delete enemy;
			enemyCount_--;
			return true;
		}
		return false;
		});

	if (enemyPhaseChangeInterval_ > 0)
	{
		isEnemyPhaseChange_ = false;
		enemyPhaseChangeInterval_--;
	}
	else if (enemyCount_ <= 0)
	{
		// ステート遷移
		isEnemyPhaseChange_ = true;
		enemyPhaseChangeInterval_ = 120;
	}

	// シーンスタート
	if (isSceneStart_ && alpha_ > 0.1f)
	{
		alpha_ -= 0.01f;

		if (alpha_ <= 0.1f)
		{
			isSceneStart_ = false;
		}
	}

	// クリアフェードスタート
	if (isClear_ && !isSceneStart_)
	{
		isClearSceneChange_ = true;
	}
	// クリアフェード
	if (isClearSceneChange_)
	{
		alpha_ += 0.01f;

		// 画面隠れたらシーン切り替え
		if (alpha_ >= 1.1f)
		{
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		}
	}

	// ゲームオーバーフェードスタート
	if (pPlayer_->IsDead())
	{
		isGameOverSceneChange_ = true;
	}
	if (isGameOverSceneChange_)
	{
		alpha_ += 0.01f;
		// 画面隠れたらシーン切り替え
		if (alpha_ >= 1.1f)
		{
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
	}

	sprites[1]->SetColor({ 1.0f,1.0f,1.0f,alpha_ });
}

void GamePlayScene::Draw()
{

	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	// 天球描画
	pSkydome_->Draw();
	// 床描画
	pGround_->Draw();
	// プレーヤー描画
	pPlayer_->Draw();
	// 敵描画
	for (auto& enemy : pEnemy_)
	{
		enemy->Draw();
	}

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();


	for (uint32_t i = 0; i < 3; ++i)
	{
		sprites[i]->Draw();
	}

	sprites[enemyCount_ + 3]->Draw();
	
}

void GamePlayScene::EnemyInit()
{
	Enemy* newEnemy = new Enemy();

	newEnemy->SetPosition(enemyPosition_);
	newEnemy->Initialize();
	newEnemy->SetPlayerPosition(pPlayer_->GetPosition());

	// 敵を登録する
	pEnemy_.push_back(newEnemy);

	// 敵出現カウント
	enemyCount_++;
}

void GamePlayScene::LoadEnemyPopData1()
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

void GamePlayScene::LoadEnemyPopData2()
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

void GamePlayScene::LoadEnemyPopData3()
{
	//ファイルを開く
	std::ifstream file;
	file.open("resources/csv/EnemyPop3.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands3 << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdateEnemyPopCommands1()
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

void GamePlayScene::UpdateEnemyPopCommands2()
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

void GamePlayScene::UpdateEnemyPopCommands3()
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
	while (getline(enemyPopCommands3, line)) {
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

void GamePlayScene::ChangeState(std::unique_ptr<GamePlayState> _pState)
{
	pState_ = std::move(_pState);
}