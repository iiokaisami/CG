#include "MyGame.h"



void MyGame::Initialize()
{

	Framework::Initialize();

	// シーンマネージャに最初のシーンをセット
	//sceneFactory_ = new SceneFactory();
	//SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);

	// 最初のシーンを設定
	//SceneManager::GetInstance()->ChangeScene("TITLE");

	//////////////////////////////////////

	// カメラマネージャーテスト


	// カメラの作成
	//camera1 = std::make_shared<Camera>();
	//camera2 = std::make_shared<Camera>();

	//camera1Rotate = camera1->GetRotate();
	//camera1Position = camera1->GetPosition();
	//camera1Position.z = -5.0f;
	//camera2Rotate = camera2->GetRotate();
	//camera2Position = camera2->GetPosition();
	//camera2Position.z = -10.0f;

	//// カメラの追加
	//cameraManager.AddCamera(camera1);
	//cameraManager.AddCamera(camera2);

	//// アクティブカメラの設定
	//cameraManager.SetActiveCamera(0);
	//activeIndex = cameraManager.GetActiveIndex();

	//Object3dCommon::GetInstance()->SetDefaultCamera(cameraManager.GetActiveCamera());


	//////////////////////////////////////

}

void MyGame::Finalize()
{
	
	
	// カメラ解放
	//cameraManager.RemoveCamera(0);
	//cameraManager.RemoveCamera(1);


	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}

	Framework::Finalize();
}

void MyGame::Update()
{

	Framework::Update();

	// カメラマネージャーのテスト
	//cameraManager.UpdateAll();

	//// ENTER押してカメラ切り替え
	//if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	//{
	//	if (cameraManager.GetActiveIndex() == 0)
	//	{
	//		cameraManager.SetActiveCamera(1);

	//	}
	//	else if (cameraManager.GetActiveIndex() == 1)
	//	{
	//		cameraManager.SetActiveCamera(0);

	//	}
	//}
	//// 稼働中のカメラインデックス
	//activeIndex = cameraManager.GetActiveIndex();

	//camera1->SetRotate(camera1Rotate);
	//camera1->SetPosition(camera1Position);
	//camera2->SetRotate(camera2Rotate);
	//camera2->SetPosition(camera2Position);

	//// アクティブカメラの情報を取得
	//auto activeCamera = cameraManager.GetActiveCamera();
	//if (activeCamera)
	//{
	//	auto viewMatrix = activeCamera->GetViewMatrix();
	//	// viewMatrix を使った処理
	//}



	//uint32_t i = 1;
	//for (Object3d* object3d : object3ds)
	//{
	//	object3d->Update();


	//	Vector3 pos{}, pos2{}, rotate{}, rotate2{};

	//	pos2 = { 1.0f,-1.0f,1.0f };

	//	if (i == 1)
	//	{
	//		rotate = object3d->GetRotate();
	//		rotate.y += 0.1f;
	//		object3d->SetPosition(pos);
	//		object3d->SetRotate(rotate);
	//	}
	//	else
	//	{
	//		rotate2 = object3d->GetRotate();
	//		rotate2.z += 0.05f;
	//		object3d->SetPosition(pos2);
	//		object3d->SetRotate(rotate2);
	//	}
	//	i++;

	//	// カメラ切り替え
	//	object3d->SetCamera(cameraManager.GetActiveCamera());
	//}



}

void MyGame::Draw()
{

	//ゲームの処理		描画処理

	//コマンドを積み込んで確定させる


	srvManager->PreDraw();

	dxCommon->PreDraw();

	sceneManager_->Draw();

#ifdef _DEBUG
	// ImGui描画
	imGuiManager->Draw();
#endif // _DEBUG


	dxCommon->PostDraw();

}
