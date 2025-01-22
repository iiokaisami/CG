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
			sprite->Initialize("uvChecker.png", { startPosition_.x,startPosition_.y }, { 1,1,1,1 }, { 0,0 });
		}
		sprites.push_back(sprite);

		/*Vector2 size = sprite->GetSize();
		size.x = 370.0f;
		size.y = 370.0f;
		sprite->SetSize(size);*/
	}

	// --- サウンド ---
	soundData_ = Audio::GetInstance()->LoadWav("fanfare.wav");
	Audio::GetInstance()->PlayWave(soundData_, false, 0.2f);
	soundData2_ = Audio::GetInstance()->LoadWav("BGM.wav");
	Audio::GetInstance()->PlayWave(soundData2_, true, 0.2f);

	isSceneChange_ = false;
	isScreenHide_ = false;
	movePosition_ = { 0.0f,0.0f,0.0f };
	t = 0.0f;
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
	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData2_);

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

		sprite->SetColor(color_);

	}



#ifdef _DEBUG

	// 透明度の更新

	ImGui::Begin("TitleScene");

	ImGui::SliderFloat4("transparent", &color_.x, 0.0f, 1.0f);


	ImGui::End();

#endif // _DEBUG


	
	

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		isSceneChange_ = true;
	}

	if (isSceneChange_)
	{
		movePosition_ = Lerp(startPosition_, endPosition_, EaseOutQuint(t));
		
		sprites[4]->SetPosition({ movePosition_.x,movePosition_.y});

		if (t >= 1.0f)
		{
			isScreenHide_ = true;
		}
	}

	if (isScreenHide_)
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
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
