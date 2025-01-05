#include "MyGame.h"

#include "SceneFactory.h"

void MyGame::Initialize()
{

	Framework::Initialize();

	// シーンマネージャに最初のシーンをセット
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);

	// 最初のシーンを設定
	SceneManager::GetInstance()->ChangeScene("TITLE");

	//// デバイス
	//Microsoft::WRL::ComPtr<ID3D12Device> device = dxCommon->GetDevice();
	//// コマンドアロケータ
	//Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = dxCommon->GetCommandAllocator();
	//// コマンドリスト
	//Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon->GetCommandList();
	//// DescriptorHeap
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap = dxCommon->GetSrvDescriptorHeap();
	//// descriptorSizeSRV
	//uint32_t descriptorSizeSRV = dxCommon->GetDescriptorSizeSRV();
	//// ビューポート
	//D3D12_VIEWPORT viewport = dxCommon->GetViewport();
	//// シザー矩形
	//D3D12_RECT scissorRect = dxCommon->GetScissorRect();
	
	// ポインタ
	//Input::GetInstance()->Initialize(winApp);


	//////////////////////////////////////

   // カメラマネージャーテスト

	//cameraManager = CameraManager::GetInstance();

	// カメラの作成
	camera1 = std::make_shared<Camera>();
	camera2 = std::make_shared<Camera>();

	camera1Rotate = camera1->GetRotate();
	camera1Position = camera1->GetPosition();
	camera1Position.z = -5.0f;
	camera2Rotate = camera2->GetRotate();
	camera2Position = camera2->GetPosition();
	camera2Position.z = -10.0f;

	// カメラの追加
	cameraManager.AddCamera(camera1);
	cameraManager.AddCamera(camera2);

	// アクティブカメラの設定
	cameraManager.SetActiveCamera(0);
	activeIndex = cameraManager.GetActiveIndex();

	Object3dCommon::GetInstance()->SetDefaultCamera(cameraManager.GetActiveCamera());


	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	// 3Dオブジェクトの初期化

	/*for (uint32_t i = 0; i < 2; ++i) {
		Object3d* object = new Object3d();
		if (i == 0) {
			object->Initialize("plane.obj");
		}
		if (i == 1) {
			object->Initialize("axis.obj");
		}

		Vector3 pos;
		pos.x = i * 2.0f;
		object->SetPosition(pos);

		object3ds.push_back(object);
	}*/




	


	//for (uint32_t i = 0; i < 1; ++i)
	//{
	//	Sprite* sprite = new Sprite();
	//	if (i == 0 || i == 3) {
	//		sprite->Initialize(spriteCommon, "uvChecker.png");
	//	}
	//	else {
	//		sprite->Initialize(spriteCommon, "monsterBall.png");
	//	}
	//	sprites.push_back(sprite);

	//	position = sprite->GetPosition();
	//	// 座標を変更する
	//	position.x = 100.0f;
	//	position.y = 100.0f;
	//	// 変更を反映する
	//	sprite->SetPosition(position);

	//	Vector2 size = sprite->GetSize();
	//	size.x = 370.0f;
	//	size.y = 370.0f;
	//	sprite->SetSize(size);
	//}

	//////////////////////////////////////


	//SRVを制作するDescriptorHeapの場所を決める
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();



	////SRVを制作するDescriptorHeapの場所を決める
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxCommon->GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxCommon->GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);

}

void MyGame::Finalize()
{


	// スプライト解放
	/*for (Sprite* sprite : sprites)
	{
		delete sprite;
	}*/


	// 3Dオブジェクト解放
	/*for (Object3d* object3d : object3ds)
	{
		delete object3d;
	}*/

	// カメラ解放
	cameraManager.RemoveCamera(0);
	cameraManager.RemoveCamera(1);



	//CloseHandle(fenceEvent);


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

	
	// 数字の0キーが押されていたら
	if (input->TriggerKey(DIK_0))
	{
		OutputDebugStringA("Hit 0\n");
	}


	////ゲームの処理		更新処理


#ifdef _DEBUG


			//ImGui


			// 次のウィンドウのサイズを設定 (幅 400, 高さ 300)
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	// ウィンドウの開始
	if (ImGui::Begin("My Window")) {
		// ウィンドウの内容
		ImGui::SliderFloat2("position", &position.x, -500.0f, 1000.0f);
	}
	// ウィンドウの終了
	ImGui::End();


	if (ImGui::CollapsingHeader("sprite"))
	{
		ImGui::Checkbox("FilipX", &isFilipX);
		ImGui::Checkbox("FilipY", &isFilipY);
		ImGui::SliderFloat2("textureLeftTop", &textureLeftTop.x, 0.0f, 1000.0f);
		ImGui::SliderFloat2("textureSize", &textureSize.x, 0.0f, 1500.0f);
	}


	if (ImGui::CollapsingHeader("cameraManager"))
	{
		ImGui::Text("activeIndex:(%d)", activeIndex);
		ImGui::SliderFloat3("rotate1", &camera1Rotate.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("position1", &camera1Position.x, -50.0f, 50.0f);
		ImGui::SliderFloat3("rotate2", &camera2Rotate.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("position2", &camera2Position.x, -50.0f, 50.0f);
	}


#endif // _DEBUG


	// カメラマネージャーのテスト
	cameraManager.UpdateAll();

	// ENTER押してカメラ切り替え
	if (input->TriggerKey(DIK_RETURN))
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




	/*for (Sprite* sprite : sprites)
	{
		sprite->Update();
		sprite->SetPosition(position);
		sprite->SetFlipX(isFilipX);
		sprite->SetFlipY(isFilipY);
		sprite->SetTextureLeftTop(textureLeftTop);
		sprite->SetTextureSize(textureSize);
	}*/

	//// 現在の座標を変数で受ける
	//Vector2 position = sprite->GetPosition();
	//// 座標を変更する
	//position.x += 0.1f;
	//position.y += 0.1f;
	//// 変更を反映する
	//sprite->SetPosition(position);

	// 角度を変化させるテスト
	/*float rotation = sprite->GetRotation();
	rotation += 0.01f;
	sprite->SetRotation(rotation);*/

	//// 色を変化させるテスト
	//Vector4 color = sprite->GetColor();
	//color.x += 0.01f;
	//if (color.x > 1.0f) 
	//{
	//	color.x -= 1.0f;
	//}
	//sprite->SetColor(color);

	// サイズを変化させるテスト
	/*Vector2 size = sprite->GetSize();
	size.x += 0.1f;
	size.y += 0.1f;
	sprite->SetSize(size);*/

}

void MyGame::Draw()
{
	//ゲームの処理		描画処理


	//コマンドを積み込んで確定させる


	srvManager->PreDraw();

	dxCommon->PreDraw();


	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	object3dCommon->CommonDrawSetting();

	sceneManager_->Draw();

	//for (Object3d* object3d : object3ds)
	//{
	//	//object3d->Draw();
	//}


	//spriteCommon->CommonDrawSetting();

	//for (Sprite* sprite : sprites)
	//{
	//	//sprite->Draw(textureSrvHandleGPU);
	//}



#ifdef _DEBUG
	// ImGui描画
	imGuiManager->Draw();
#endif // _DEBUG


	dxCommon->PostDraw();

}
