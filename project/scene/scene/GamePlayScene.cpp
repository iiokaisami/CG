#include "GamePlayScene.h"

#include <ModelManager.h>

void GamePlayScene::Initialize()
{
	// 必ず先頭でカメラを全クリア
	cameraManager.ClearAllCameras();

	// カメラの作成
	camera1 = std::make_shared<Camera>();
	camera2 = std::make_shared<Camera>();

	camera1Rotate = camera1->GetRotate();
	camera1Position = camera1->GetPosition();
	camera1Position.y = 70.0f;
	camera1Position.z = -15.0f;
	camera1Rotate.x = 1.2f;
	camera2Rotate = camera2->GetRotate();
	camera2Position = camera2->GetPosition();
	camera2Position.y = 4.0f;
	camera2Position.z = -10.0f;

	// カメラの追加
	cameraManager.AddCamera(camera1);
	cameraManager.AddCamera(camera2);

	// アクティブカメラの設定
	cameraManager.SetActiveCamera(0);
	activeIndex = cameraManager.GetActiveIndex();

	Object3dCommon::GetInstance()->SetDefaultCamera(cameraManager.GetActiveCamera());

	// 衝突判定
	colliderManager_ = ColliderManager::GetInstance();
	colliderManager_->Initialize();

	// プレイヤー
	pPlayer_ = std::make_unique<Player>();
	pPlayer_->Initialize();


	// フィールド
	pField_ = std::make_unique<Field>();
	pField_->Initialize();


	// エネミーのcsvデータを読み込み
	LoadEnemyPopData1();


	// スプライト
	/*for (uint32_t i = 0; i < 1; ++i)
	{
		Sprite* sprite = new Sprite();

		if (i == 0) {
			sprite->Initialize("monsterBall.png", { 0,0 }, color_, { 0,0 });
		}

		sprites.push_back(sprite);
	}*/
}

void GamePlayScene::Finalize()
{
	pPlayer_->Finalize();
	for (auto& enemy : pEnemies_)
	{
		enemy->Finalize();
	}
	pField_->Finalize();

	/*for (Sprite* sprite : sprites)
	{
		delete sprite;
	}
	sprites.clear();*/

	// カメラ解放
	cameraManager.RemoveCamera(0);
	cameraManager.RemoveCamera(1);
}

void GamePlayScene::Update()
{
	// 当たり判定チェック
	colliderManager_->CheckAllCollision();

	/*for (Sprite* sprite : sprites)
	{
		sprite->Update();

		sprite->SetColor(color_);

	}*/

	// カメラマネージャーのテスト
	cameraManager.UpdateAll();

	// P押してカメラ切り替え
	if (Input::GetInstance()->TriggerKey(DIK_P))
	{
		if (cameraManager.GetActiveIndex() == 0)
		{
			cameraManager.SetActiveCamera(1);

		}
		else if (cameraManager.GetActiveIndex() == 1)
		{
			cameraManager.SetActiveCamera(0);

		}
	}
	// 稼働中のカメラインデックス
	activeIndex = cameraManager.GetActiveIndex();

	camera1->SetRotate(camera1Rotate);
	camera1->SetPosition(camera1Position);
	camera2->SetRotate(camera2Rotate);
	camera2->SetPosition(camera2Position);

	// プレイヤーの更新
	pPlayer_->Update();

	// カメラの更新(引き、シェイク)
	CameraUpdate();

	// エネミーの更新
	for (auto& enemy : pEnemies_)
	{
		enemy->SetPlayerPosition(pPlayer_->GetPosition());
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

	// フィールドの更新
	pField_->Update();


#ifdef _DEBUG

	// 透明度の更新

	ImGui::Begin("PlayScene");

	ImGui::SliderFloat3("camera1Position", &camera1Position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("camera1Rotate", &camera1Rotate.x, -10.0f, 10.0f);

	ImGui::SliderFloat3("camera2Position", &camera2Position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("camera2Rotate", &camera2Rotate.x, -10.0f, 10.0f);

	ImGui::End();

	pPlayer_->ImGuiDraw();
	for (auto& enemy : pEnemies_)
	{
		enemy->ImGuiDraw();
	}
	pField_->ImGuiDraw();

#endif // _DEBUG


	if (Input::GetInstance()->TriggerKey(DIK_UP))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
	if (Input::GetInstance()->TriggerKey(DIK_DOWN) or pPlayer_->IsDead())
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
}

void GamePlayScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	pPlayer_->Draw(); 
	for (auto& enemy : pEnemies_) 
	{
		enemy->Draw();
	}

	pField_->Draw();

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	//for (Sprite* sprite : sprites)
	//{
	//	sprite->Draw();
	//}
}

void GamePlayScene::EnemyInit()
{
	// エネミー
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->SetPosition(enemyPosition_);
	enemy->Initialize();
	enemy->SetPlayerPosition(pPlayer_->GetPosition());
	enemy->Update();

	// 敵を登録
	pEnemies_.push_back(std::move(enemy));

}

void GamePlayScene::LoadEnemyPopData1()
{
	//ファイルを開く
	std::ifstream file;
	file.open("resources/csv/EnemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands1 << file.rdbuf();

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

void GamePlayScene::CameraUpdate()
{
	// アクティブカメラの情報を取得
	auto activeCamera = cameraManager.GetActiveCamera();
	if (activeCamera)
	{
		auto viewMatrix = activeCamera->GetViewMatrix();
	}

	// プレイヤーがヒットした場合にカメラをシェイク
	if (pPlayer_->IsHitMoment())
	{
		// アクティブなカメラを取得
		if (activeCamera)
		{
			// カメラをシェイク (持続時間,振幅)
			activeCamera->StartShake(0.3f, 0.5f);

			// ヒットフラグをリセット
			pPlayer_->SetHitMoment(false);
		}
	}

	// シェイク
	if (activeCamera)
	{
		activeCamera->UpdateShake(1.0f / 60.0f);
	}

	// カメラの引き

}
