#include "GamePlayScene.h"

#include <ModelManager.h>

void GamePlayScene::Initialize()
{
	//////////////////////////////////////

	// カメラマネージャーテスト


	// カメラの作成
	camera1 = std::make_shared<Camera>();
	camera2 = std::make_shared<Camera>();

	camera1Rotate = camera1->GetRotate();
	camera1Position = camera1->GetPosition();
	camera1Position.y = 4.0f;
	camera1Position.z = -5.0f;
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


	//////////////////////////////////////


	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("axis.obj");

	for (uint32_t i = 0; i < 1; ++i)
	{
		Object3d* object = new Object3d();
		if (i == 0)
		{
			object->Initialize("axis.obj");
		}
		position_ = { 0.0f,4.0f,10.0f };
		object->SetPosition(position_);

		object->SetScale({ 1.2f,1.2f,1.2f });

		object3ds.push_back(object);
	}

	for (uint32_t i = 0; i < 1; ++i)
	{
		Sprite* sprite = new Sprite();

		if (i == 0) {
			sprite->Initialize("monsterBall.png", { 0,0 }, color_, { 0,0 });
		}

		sprites.push_back(sprite);
	}
}

void GamePlayScene::Finalize()
{
	for (auto& obj : object3ds)
	{
		delete obj;
	}
	object3ds.clear();

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
	for (auto& obj : object3ds) 
	{
		obj->Update();
		obj->SetPosition(position_);
	}


	for (Sprite* sprite : sprites)
	{
		sprite->Update();

		sprite->SetColor(color_);

	}

	// カメラマネージャーのテスト
	cameraManager.UpdateAll();

	// ENTER押してカメラ切り替え
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
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



	uint32_t i = 1;
	for (Object3d* object3d : object3ds)
	{
		object3d->Update();


		Vector3 pos{}, pos2{}, rotate{}, rotate2{};

		pos2 = { 1.0f,-1.0f,1.0f };

		if (i == 1)
		{
			rotate = object3d->GetRotate();
			rotate.y += 0.1f;
			object3d->SetPosition(pos);
			object3d->SetRotate(rotate);
		}
		else
		{
			rotate2 = object3d->GetRotate();
			rotate2.z += 0.05f;
			object3d->SetPosition(pos2);
			object3d->SetRotate(rotate2);
		}
		i++;

		// カメラ切り替え
		object3d->SetCamera(cameraManager.GetActiveCamera());
	}



#ifdef _DEBUG

	// 透明度の更新

	ImGui::Begin("PlayScene");

	ImGui::SliderFloat4("transparent", &color_.x, 0.0f, 1.0f);

	ImGui::SliderFloat3("camera1Position", &camera1Position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("camera1Rotate", &camera1Rotate.x, -10.0f, 10.0f);

	ImGui::SliderFloat3("camera2Position", &camera2Position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("camera2Rotate", &camera2Rotate.x, -10.0f, 10.0f);

	ImGui::End();

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

	for (auto& obj : object3ds) 
	{
		obj->Draw();
	}

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	for (Sprite* sprite : sprites)
	{
		sprite->Draw();
	}
}
