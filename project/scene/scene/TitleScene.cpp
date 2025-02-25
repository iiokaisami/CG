#include "TitleScene.h"

#include <ModelManager.h>

void TitleScene::Initialize()
{
	camera_ = std::make_shared<Camera>();
	camera_->SetRotate(cameraRot_);
	camera_->SetPosition(cameraPos_);
	Object3dCommon::GetInstance()->SetDefaultCamera(camera_);
	cameraManager.AddCamera(camera_);
	cameraManager.SetActiveCamera(0);

	collisionManager_ = CollisionManager::GetInstance();
	collisionManager_->Initialize();


	// プレイヤー
	player_ = std::make_shared<Player>();
	player_->Initialize();

	// フィールド
	field_ = std::make_shared<Field>();
	field_->Initialize();
	field_->SetPosition({ 0.0f,-0.5f,0.0f });
}

void TitleScene::Finalize()
{
	player_->Finalize();
	field_->Finalize();
	
	cameraManager.RemoveCamera(0);
}

void TitleScene::Update()
{

	cameraManager.UpdateAll();
	camera_->SetRotate(cameraRot_);
	camera_->SetPosition(cameraPos_);


	// プレイヤー
	player_->Update();

	// フィールド
	field_->Update();


	// 当たり判定チェック
	collisionManager_->CheckAllCollision();

#ifdef _DEBUG

	// 透明度の更新

	ImGui::Begin("TitleScene");

	ImGui::SliderFloat3("CameraPos", &cameraPos_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("CameraRot", &cameraRot_.x, -5.0f, 5.0f);
	
	ImGui::End();

	player_->ImGuiDraw();

#endif // _DEBUG

	


	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	// プレイヤー
	player_->Draw();

	// フィールド
	field_->Draw();

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();


}
