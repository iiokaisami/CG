#include "TitleScene.h"

#include <ModelManager.h>

#include "../../Easing.h"

void TitleScene::Initialize()
{
	camera_ = std::make_shared<Camera>();
	camera_->SetRotate({ 0.3f,0.0f,0.0f });
	camera_->SetPosition({ 0.0f,4.0f,-20.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera_);
	cameraManager.AddCamera(camera_);
	cameraManager.SetActiveCamera(0);

	// --- 3Dオブジェクト ---
	//ModelManager::GetInstance()->LoadModel("cube.obj");

	/*for (uint32_t i = 0; i < 1; ++i)
	{
		Object3d* object = new Object3d();
		if (i == 0)
		{
			object->Initialize("cube.obj");
		}
		position_ = { 0.0f,4.0f,10.0f };
		object->SetPosition(position_);

		object->SetScale({ 1.2f,1.2f,1.2f });

		object3ds.push_back(object);
	}*/


	isSceneStart_ = true;
	isSceneChange_ = false;
	alpha_ = 1.0f;


	for (uint32_t i = 0; i < 5; ++i)
	{
		Sprite* sprite = new Sprite();
		if (i == 0 ) {
			sprite->Initialize("Title/Logo.png", { 0,0 }, color_, { 0,0 });
		}
		else if(i == 1)
		{
			sprite->Initialize("Title/UI.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 2)
		{
			sprite->Initialize("Title/obake.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 3)
		{
			sprite->Initialize("Title/obake2.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if (i == 4)
		{
			sprite->Initialize("black.png", { 0.0f,0.0f }, { 1,1,1,alpha_ }, { 0,0 });
		}
		sprites.push_back(sprite);

		/*Vector2 size = sprite->GetSize();
		size.x = 370.0f;
		size.y = 370.0f;
		sprite->SetSize(size);*/
	}

	// --- サウンド ---
	soundData_ = Audio::GetInstance()->LoadWav("BGM.wav");
	Audio::GetInstance()->PlayWave(soundData_, true, 0.2f);

}

void TitleScene::Finalize()
{
	/*for (auto& obj : object3ds) {
	delete obj;
}*/

	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}

	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData_);

	cameraManager.RemoveCamera(0);
}

void TitleScene::Update()
{
	/*for (auto& obj : object3ds) {
		obj->Update();
		obj->SetPosition(position_);
	}*/


	for (Sprite* sprite : sprites)
	{
		sprite->Update();

	}


	sprites[4]->SetColor({ 1.0f,1.0f,1.0f,alpha_ });



#ifdef _DEBUG

	// 透明度の更新

	ImGui::Begin("TitleScene");

	ImGui::SliderFloat4("transparent", &color_.x, 0.0f, 1.0f);

	ImGui::SliderFloat("alpha", &alpha_, 0.0f, 1.0f);

	ImGui::End();

#endif // _DEBUG


	// シーンスタート
	if (isSceneStart_ && alpha_ > 0.1f)
	{
		alpha_ -= 0.01f;

		if (alpha_ <= 0.1f)
		{
			isSceneStart_ = false;
		}
	}
	// フェードスタート
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && !isSceneStart_ )
	{
		isSceneChange_ = true;
	}
	// フェード
	if (isSceneChange_)
	{
		alpha_ += 0.01f;

		// 画面隠れたらシーン切り替え
		if (alpha_ >= 1.1f)
		{
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}
	


	/*if (Input::GetInstance()->TriggerKey(DIK_Q))
	{
		Audio::GetInstance()->SoundStop(soundData_);
	}
	if (Input::GetInstance()->TriggerKey(DIK_E))
	{
		Audio::GetInstance()->SoundStop(soundData2_);
	}*/
}

void TitleScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	/*for (auto& obj : object3ds) {
		obj->Draw();
	}*/

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	for (Sprite* sprite : sprites)
	{
		sprite->Draw();
	}
}
