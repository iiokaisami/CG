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

	// エネミー
	pEnemyManager_ = std::make_unique<EnemyManager>();
	pEnemyManager_->Initialize();

	// フィールド
	pField_ = std::make_unique<Field>();
	pField_->Initialize();

	// 壁の初期化
	for (int i = 0; i < 1; ++i)
	{
		auto wall = std::make_unique<Wall>();
		wall->Initialize();
		wall->SetPosition({ static_cast<float>(i * 5.0f),0.0f,3.0f });
		wall->SetRotation({ 0.0f, i * 0.7f, 0.0f });
		pWalls_.push_back(std::move(wall));
	}

	// 追尾の初期化
	cameraIsResting_ = true;
	cameraRestCenter_ = pPlayer_->GetPosition() + Vector3{ 0.0f,70.0f,-20.0f };

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
	pEnemyManager_->Finalize();
	pField_->Finalize();
	for (auto& wall : pWalls_)
	{
		wall->Finalize();
	}

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


#ifdef _DEBUG

	// 透明度の更新

	ImGui::Begin("PlayScene");

	ImGui::SliderFloat3("camera1Position", &camera1Position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("camera1Rotate", &camera1Rotate.x, -10.0f, 10.0f);

	ImGui::SliderFloat3("camera2Position", &camera2Position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("camera2Rotate", &camera2Rotate.x, -10.0f, 10.0f);

	ImGui::End();

	pPlayer_->ImGuiDraw();
	pEnemyManager_->ImGuiDraw();
	pField_->ImGuiDraw();
	for (auto& wall : pWalls_)
	{
		wall->ImGuiDraw();
	}

#endif // _DEBUG


	if (Input::GetInstance()->TriggerKey(DIK_UP) or pEnemyManager_->IsAllEnemyDefeated())
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

	pEnemyManager_->Draw();

	pField_->Draw();

	for (auto& wall : pWalls_)
	{
		wall->Draw();
	}

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	//for (Sprite* sprite : sprites)
	//{
	//	sprite->Draw();
	//}
}

void GamePlayScene::CameraUpdate()
{
	// カメラのシェイク
	CameraShake();

	// カメラの追従・引き
	CameraFollowZoom();

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

void GamePlayScene::CameraFollowZoom()
{
	if (!camera1 or !pPlayer_ or pEnemyManager_->GetToPlayerDistance().empty())
	{
		return;
	}

	Vector3 playerPos = pPlayer_->GetPosition();

	// 敵との最短距離を取得
	float minDistance = std::numeric_limits<float>::max();
	for (const Vector3& vec : pEnemyManager_->GetToPlayerDistance())
	{
		float dist = vec.Length();
		minDistance = std::min(minDistance, dist);
	}

	// ズーム(カメラのZ位置)を距離に応じて変化
	// 距離が近い→カメラ寄る、距離が遠い→カメラ引く
	const float minDist = 5.0f;
	const float maxDist = 50.0f;
	const float nearZ = -15.0f;
	const float farZ = -40.0f;
	float t = std::clamp((minDistance - minDist) / (maxDist - minDist), 0.0f, 1.0f);
	float cameraZ = std::lerp(nearZ, farZ, t);

	// カメラの理想的な相対位置
	Vector3 offset = { 0.0f, 70.0f, cameraZ };
	Vector3 targetPos = playerPos + offset;

	// カメラの回転
	Vector3 targetRot = { 1.2f, 0.0f, 0.0f };

	// 補間で滑らかに追従
	Vector3 currentPos = camera1->GetPosition();
	Vector3 nextPos;
	nextPos.Lerp(currentPos, targetPos, 0.8f);

	Vector3 currentRot = camera1->GetRotate();
	Vector3 nextRot;
	nextRot.Lerp(currentRot, targetRot, 0.25f);

	camera1->SetPosition(nextPos);
	camera1->SetRotate(nextRot);
}