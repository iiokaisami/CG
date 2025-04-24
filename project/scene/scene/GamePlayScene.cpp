#include "GamePlayScene.h"

#include <ModelManager.h>

void GamePlayScene::Initialize()
{
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


	// プレイヤー
	pPlayer_ = std::make_unique<Player>();
	pPlayer_->Initialize();

	// エネミー
	pEnemy_ = std::make_unique<Enemy>();
	pEnemy_->Initialize();
	pEnemy_->SetPlayerPosition(pPlayer_->GetPosition());

	// フィールド
	pField_ = std::make_unique<Field>();
	pField_->Initialize();


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
	pEnemy_->Finalize();
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
	/*for (Sprite* sprite : sprites)
	{
		sprite->Update();

		sprite->SetColor(color_);

	}*/

	// カメラマネージャーのテスト
	cameraManager.UpdateAll();

	// ENTER押してカメラ切り替え
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

	// アクティブカメラの情報を取得
	auto activeCamera = cameraManager.GetActiveCamera();
	if (activeCamera)
	{
		auto viewMatrix = activeCamera->GetViewMatrix();
		// viewMatrix を使った処理
	}

	// プレイヤーの更新
	pPlayer_->Update();

	// エネミーの更新
	pEnemy_->SetPlayerPosition(pPlayer_->GetPosition());
	pEnemy_->Update();

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
	pEnemy_->ImGuiDraw();
	pField_->ImGuiDraw();

#endif // _DEBUG


	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GamePlayScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	pPlayer_->Draw();
	pEnemy_->Draw();
	pField_->Draw();

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	//for (Sprite* sprite : sprites)
	//{
	//	sprite->Draw();
	//}
}
