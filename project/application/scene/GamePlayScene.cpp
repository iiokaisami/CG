#include "GamePlayScene.h"

#include <ModelManager.h>

void GamePlayScene::Initialize()
{
	// 必ず先頭でカメラを全クリア
	cameraManager.ClearAllCameras();

	// カメラの作成
	camera = std::make_shared<Camera>();

	cameraRotate = camera->GetRotate();
	cameraPosition = camera->GetPosition();
	cameraPosition.y = 70.0f;
	cameraPosition.z = -15.0f;
	cameraRotate.x = 1.2f;

	// カメラの追加
	cameraManager.AddCamera(camera);

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

	// エネミー
	pEnemyManager_ = std::make_unique<EnemyManager>();
	pEnemyManager_->Initialize();

	// フィールド
	pField_ = std::make_unique<Field>();
	pField_->Initialize();

	// ゴールの初期化
	pGoal_ = std::make_unique<Goal>();
	pGoal_->Initialize();

	// 追尾の初期化
	cameraIsResting_ = true;
	cameraRestCenter_ = pPlayer_->GetPosition() + Vector3{ 0.0f,70.0f,-20.0f };

	// スプライト
	//for (uint32_t i = 0; i < 1; ++i)
	//{
	//	Sprite* sprite = new Sprite();

	//	if (i == 0)
	//	{
	//		//sprite->Initialize("titleUI.png", { 0,0 }, color_, { 0,0 });
	//	}

	//	sprites.push_back(sprite);
	//}

	// レベルデータの読み込み
	levelData_ = LevelDataLoader::LoadLevelData("wallSetting");
	if (levelData_)
	{
		// レベルデータに基づいて壁を配置
		for (const auto& wallData : levelData_->walls)
		{
			auto wall = std::make_unique<Wall>();
			wall->Initialize();
			wall->SetPosition(wallData.position);
			wall->SetRotation(wallData.rotation);
			wall->SetScale(wallData.scale);
			pWalls_.push_back(std::move(wall));
		}
	}

	// シーン開始時にフェードイン
	transition_ = std::make_unique<FadeTransition>(FadeTransition::Mode::FadeInOnly);
	isTransitioning_ = true;
	transition_->Start(nullptr);

}

void GamePlayScene::Finalize()
{
	pPlayer_->Finalize();
	pEnemyManager_->Finalize();
	pField_->Finalize();
	for (auto& wall : pWalls_)
	{
		wall->Finalize();
	}
	pGoal_->Finalize();

	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}
	sprites.clear();

	// カメラ解放
	cameraManager.RemoveCamera(0);
	cameraManager.RemoveCamera(1);
}

void GamePlayScene::Update()
{
	// トランジション更新
	if (isTransitioning_ && transition_)
	{
		transition_->Update();

		// トランジション終了判定
		if (transition_->IsFinished())
		{
			transition_.reset();
			isTransitioning_ = false;
		}

	}

	for (Sprite* sprite : sprites)
	{
		sprite->Update();

		sprite->SetColor(color_);

	}

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

	camera->SetRotate(cameraRotate);
	camera->SetPosition(cameraPosition);

	// 当たり判定チェック
	colliderManager_->CheckAllCollision();

	// プレイヤーの更新
	pPlayer_->Update();
	// プレイヤーの位置をエネミーマネージャーにセット
	pEnemyManager_->SetPlayerPosition(pPlayer_->GetPosition());

	// カメラの更新(シェイク、追尾、引き)
	CameraUpdate();

	// エネミーの更新
	pEnemyManager_->Update();

	// フィールドの更新
	pField_->Update();

	// 壁の更新
	for (auto& wall : pWalls_)
	{
		wall->Update();
	}

	// ゴールの更新
	pGoal_->Update();
	pGoal_->SetBarrierDestroyed(pEnemyManager_->IsAllEnemyDefeated());


#ifdef _DEBUG

	ImGui::Begin("PlayScene");


	// 透明度の更新
	ImGui::SliderFloat4("SpriteColor", &color_.x, 0.0f, 1.0f);

	// camera
	Vector3 cam1Pos = camera->GetPosition();
	Vector3 cam1Rot = camera->GetRotate();
	if (ImGui::SliderFloat3("cameraPosition", &cam1Pos.x, -100.0f, 100.0f))
	{
		camera->SetPosition(cam1Pos);
	}
	if (ImGui::SliderFloat3("cameraRotate", &cam1Rot.x, -10.0f, 10.0f)) 
	{
		camera->SetRotate(cam1Rot);
	}


	ImGui::End();

	pPlayer_->ImGuiDraw();
	pEnemyManager_->ImGuiDraw();
	pField_->ImGuiDraw();
	for (auto& wall : pWalls_)
	{
		wall->ImGuiDraw();
	}
	pGoal_->ImGuiDraw();


#endif // _DEBUG


	if (Input::GetInstance()->TriggerKey(DIK_UP) or pGoal_->IsCleared())
	{
		// トランジション開始
		transition_ = std::make_unique<FadeTransition>();
		isTransitioning_ = true;
		transition_->Start([]
			{
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("CLEAR");
			});
	}
	if (Input::GetInstance()->TriggerKey(DIK_DOWN) or (pPlayer_->IsDead() && !pPlayer_->IsAutoControl()))
	{
		// トランジション開始
		transition_ = std::make_unique<FadeTransition>();
		isTransitioning_ = true;
		transition_->Start([]
			{
				// シーン切り替え
				SceneManager::GetInstance()->ChangeScene("GAMEOVER");
			});
	}
}

void GamePlayScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	pPlayer_->Draw();

	pEnemyManager_->Draw();

	pField_->Draw();

	for (auto& wall : pWalls_)
	{
		wall->Draw();
	}

	pGoal_->Draw();


	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	for (Sprite* sprite : sprites)
	{
		sprite->Draw();
	}


	// トランジション描画
	if (isTransitioning_ && transition_)
	{
		transition_->Draw();
	}
}

void GamePlayScene::CameraUpdate()
{
	// カメラのシェイク
	CameraShake();

	// カメラの追従
	CameraFollow();

}

void GamePlayScene::CameraShake()
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
}

void GamePlayScene::CameraFollow()
{
	if (!camera or !pPlayer_) 
	{
		return;
	}

	Vector3 playerPos = pPlayer_->GetPosition();

	// 固定のオフセット（プレイヤーから見たカメラ位置）
	Vector3 offset = { 0.0f, 80.0f, -20.0f };  // Y: 高さ、Z: 後方

	// 追従先の位置・回転
	Vector3 targetPos = playerPos + offset;
	Vector3 targetRot = { 2.0f, 0.0f, 0.0f };  // やや下向き

	// 滑らかに補間して追従
	Vector3 currentPos = camera->GetPosition();
	Vector3 nextPos;
	nextPos.Lerp(currentPos, targetPos, 0.8f);

	Vector3 currentRot = camera->GetRotate();
	Vector3 nextRot;
	nextRot.Lerp(currentRot, targetRot, 0.25f);

	camera->SetPosition(nextPos);
	camera->SetRotate(nextRot);
}
