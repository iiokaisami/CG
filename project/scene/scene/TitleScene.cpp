#include "TitleScene.h"

#include <ModelManager.h>

void TitleScene::Initialize()
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

	for (uint32_t i = 0; i < 1; ++i)
	{
		Sprite* sprite = new Sprite();
		
		if (i == 0)
		{
			sprite->Initialize("uvChecker.png", { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 });
		}
		
		sprites.push_back(sprite);
	 
	}

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
	camera_->Update();
	camera_->SetPosition(cameraPosition_);
	camera_->SetRotate(cameraRotate_);


	for (auto& obj : object3ds)
	{
		obj->Update();
	}

	object3ds[0]->SetScale(scale_);

	for (Sprite* sprite : sprites)
	{
		sprite->Update();

		sprite->SetColor(color_);

	}



#ifdef _DEBUG

	// 透明度の更新

	ImGui::Begin("TitleScene");

	//ImGui::SliderFloat4("transparent", &color_.x, 0.0f, 1.0f);

	ImGui::SliderFloat3("cameraPosition", &cameraPosition_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("cameraRotate", &cameraRotate_.x, -3.14f, 3.14f);

	ImGui::SliderFloat3("sphere scale", &scale_.x, 0.0f, 10.0f);

	if (ImGui::Button("Terrain Draw"))
	{
		isTerrainDraw = !isTerrainDraw;
	}

	// ライトの設定
	ImGui::Checkbox("Enable Lighting", &enableLighting);

	ImGui::Checkbox("Directional Light", &enableDirectionalLight);
	if (enableDirectionalLight)
	{
		ImGui::ColorEdit3("Directional Light Color", &directionalLightColor.x);
		ImGui::SliderFloat3("Directional Light Direction", &directionalLightDirection.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Directional Light Intensity", &directionalLightIntensity, 0.0f, 10.0f);
	}

	ImGui::Checkbox("Point Light", &enablePointLight);
	if (enablePointLight)
	{
		ImGui::ColorEdit3("Point Light Color", &pointLightColor.x);
		ImGui::SliderFloat3("Point Light Position", &pointLightPosition.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Point Light Intensity", &pointLightIntensity, 0.0f, 1.0f);
		ImGui::SliderFloat("Point Light Radius", &pointLightRadius, 0.0f, 50.0f);
		ImGui::SliderFloat("Point Light Decay", &pointLightDecay, 0.0f, 1.0f);
	}

	ImGui::Checkbox("Spot Light", &enableSpotLight);
	if (enableSpotLight)
	{
		ImGui::ColorEdit3("Spot Light Color", &spotLightColor.x);
		ImGui::SliderFloat3("Spot Light Position", &spotLightPosition.x, -20.0f, 20.0f);
		ImGui::SliderFloat3("Spot Light Direction", &spotLightDirection.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Spot Light Intensity", &spotLightIntensity, 0.0f, 1.0f);
		ImGui::SliderFloat("Spot Light Distance", &spotLightDistance, 0.0f, 50.0f);
		ImGui::SliderFloat("Spot Light Decay", &spotLightDecay, 0.0f, 1.0f);
		ImGui::SliderFloat("Spot Light Cons Angle", &spotLightConsAngle, 0.0f, 10.0f);
		ImGui::SliderFloat("Spot Light Cos FalloffStart", &spotLightCosFalloffStart, 0.0f, 1.0f);
	}

	ImGui::End();

#endif // _DEBUG

	// ライトの設定
	SetLightSettings();

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

	//for (Sprite* sprite : sprites)
	//{
	//	sprite->Draw();
	//}

	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	//for (auto& obj : object3ds)
	//{
		//obj->Draw();
	//}
	object3ds[0]->Draw();
	if (isTerrainDraw)
	{
		object3ds[1]->Draw();
	}
	object3ds[2]->Draw();
}

void TitleScene::SetLightSettings()
{
	for (auto& obj : object3ds)
	{
		obj->SetLighting(enableLighting);
		obj->SetDirectionalLightEnable(enableDirectionalLight);
		obj->SetDirectionalLightColor({ directionalLightColor.x, directionalLightColor.y, directionalLightColor.z, 1.0f });
		obj->SetDirectionalLightDirection(directionalLightDirection);
		obj->SetDirectionalLightIntensity(directionalLightIntensity);

		obj->SetPointLightEnable(enablePointLight);
		obj->SetPointLightColor({ pointLightColor.x, pointLightColor.y, pointLightColor.z, 1.0f });
		obj->SetPointLightPosition(pointLightPosition);
		obj->SetPointLightIntensity(pointLightIntensity);
		obj->SetPointLightRadius(pointLightRadius);
		obj->SetPointLightDecay(pointLightDecay);

		obj->SetSpotLightEnable(enableSpotLight);
		obj->SetSpotLightColor({ spotLightColor.x, spotLightColor.y, spotLightColor.z, 1.0f });
		obj->SetSpotLightPosition(spotLightPosition);
		obj->SetSpotLightDirection(spotLightDirection);
		obj->SetSpotLightIntensity(spotLightIntensity);
		obj->SetSpotLightDistance(spotLightDistance);
		obj->SetSpotLightDecay(spotLightDecay);
		obj->SetSpotLightConsAngle(spotLightConsAngle);
		obj->SetSpotLightCosFalloffStart(spotLightCosFalloffStart);
	}
}