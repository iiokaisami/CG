#include "TitleScene.h"

#include <ModelManager.h>

void TitleScene::Initialize()
{
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
	ModelManager::GetInstance()->LoadModel("sphere.obj");

	for (uint32_t i = 0; i < 1; ++i)
	{
		Object3d* object = new Object3d();
		if (i == 0)
		{
			object->Initialize("sphere.obj");
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

	// --- サウンド ---
	soundData_ = Audio::GetInstance()->LoadWav("fanfare.wav");
	Audio::GetInstance()->PlayWave(soundData_, false, 0.2f);
	soundData2_ = Audio::GetInstance()->LoadWav("BGM.wav");
	Audio::GetInstance()->PlayWave(soundData2_, true, 0.2f);

}

void TitleScene::Finalize()
{
	for (auto& obj : object3ds)
	{
		delete obj;
	}

	/*for (Sprite* sprite : sprites)
	{
		delete sprite;
	}*/

	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData_);
	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData2_);

	cameraManager.RemoveCamera(0);
}

void TitleScene::Update()
{
	camera_->Update();
	camera_->SetPosition(cameraPosition_);
	camera_->SetRotate(cameraRotate_);


	for (auto& obj : object3ds) 
	{
		obj->Update();
		obj->SetPosition(position_);
		obj->SetScale(scale_);
	}


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

	ImGui::SliderFloat3("scale", &scale_.x, 0.0f, 10.0f);

	ImGui::End();

#endif // _DEBUG


	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	if (Input::GetInstance()->TriggerKey(DIK_Q))
	{
		Audio::GetInstance()->SoundStop(soundData_);
	}
	if (Input::GetInstance()->TriggerKey(DIK_E))
	{
		Audio::GetInstance()->SoundStop(soundData2_);
	}
}

void TitleScene::Draw()
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
