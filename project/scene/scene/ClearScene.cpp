#include "ClearScene.h"

void ClearScene::Initialize()
{
	// 必ず先頭でカメラを全クリア
	cameraManager.ClearAllCameras();

	camera_ = std::make_shared<Camera>();
	camera_->SetRotate({ 0.3f,0.0f,0.0f });
	camera_->SetPosition({ 0.0f,4.0f,-20.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera_);
	cameraManager.AddCamera(camera_);
	cameraManager.SetActiveCamera(0);

	cameraPosition_ = { 0.0f,4.0f,-10.0f };
	cameraRotate_ = { 0.3f,0.0f,0.0f };
	camera_->SetPosition(cameraPosition_);
	camera_->SetRotate(cameraRotate_);

	// --- 3Dオブジェクト ---
	for (uint32_t i = 0; i < 3; ++i)
	{
		Object3d* object = new Object3d();
		if (i == 0)
		{
			object->Initialize("sphere.obj");
		}
		if (i == 1)
		{
			object->Initialize("terrain.obj");
		}
		if (i == 2)
		{
			object->Initialize("plane.obj");
		}
		position_ = { 0.0f,0.0f,5.0f };
		scale_ = { 1.0f,1.0f,1.0f };
		object->SetPosition(position_);
		object->SetScale(scale_);

		object3ds.push_back(object);
	}

	//for (uint32_t i = 0; i < 1; ++i)
	//{
	//	Sprite* sprite = new Sprite();
	//	
	//	if (i == 0) {
	//		sprite->Initialize("uvChecker.png", { 0,0 }, color_, { 0,0 });
	//	}
	//	
	//	sprites.push_back(sprite);

	//	/*Vector2 size = sprite->GetSize();
	//	size.x = 370.0f;
	//	size.y = 370.0f;
	//	sprite->SetSize(size);*/
	//}

}

void ClearScene::Finalize()
{
	for (auto& obj : object3ds)
	{
		delete obj;
	}

	/*for (Sprite* sprite : sprites)
	{
		delete sprite;
	}*/

	cameraManager.RemoveCamera(0);
}

void ClearScene::Update()
{
	camera_->Update();
	camera_->SetPosition(cameraPosition_);
	camera_->SetRotate(cameraRotate_);


	for (auto& obj : object3ds)
	{
		obj->Update();
	}

	rotate_.y += 0.01f;
	object3ds[0]->SetScale(scale_);
	object3ds[0]->SetRotate(rotate_);

	/*for (Sprite* sprite : sprites)
	{
		sprite->Update();

		sprite->SetColor(color_);

	}*/



#ifdef _DEBUG

	// 透明度の更新

	ImGui::Begin("TitleScene");

	//ImGui::SliderFloat4("transparent", &color_.x, 0.0f, 1.0f);

	ImGui::SliderFloat3("cameraPosition", &cameraPosition_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("cameraRotate", &cameraRotate_.x, -3.14f, 3.14f);

	ImGui::SliderFloat3("sphere scale", &scale_.x, 0.0f, 10.0f);

	ImGui::End();

#endif // _DEBUG


	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	/*for (Sprite* sprite : sprites)
	{
		sprite->Draw();
	}*/

	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	for (auto& obj : object3ds)
	{
		obj->Draw();
	}

}
