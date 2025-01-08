#include "MyGame.h"



void MyGame::Initialize()
{

	

	// WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	// DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	// スプライト共通部分の初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	// 3Dオブジェクト共通部の初期化
	Object3dCommon::GetInstance()->Initialize(dxCommon);

	// モデル共通部分の初期化
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);

	// 3Dモデルマネージャーの初期化
	ModelManager::GetInstance()->Initialize(dxCommon);

	// SRVマネージャーの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);







#ifdef _DEBUG

	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp, dxCommon);

#endif // _DEBUG


	// テクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxCommon, srvManager);


	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = dxCommon->GetDevice();
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = dxCommon->GetCommandAllocator();
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon->GetCommandList();
	// DescriptorHeap
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap = dxCommon->GetSrvDescriptorHeap();
	// descriptorSizeSRV
	uint32_t descriptorSizeSRV = dxCommon->GetDescriptorSizeSRV();
	// ビューポート
	D3D12_VIEWPORT viewport = dxCommon->GetViewport();
	// シザー矩形
	D3D12_RECT scissorRect = dxCommon->GetScissorRect();












	// ポインタ
	Input::GetInstance()->Initialize(winApp);




	//////////////////////////////////////

	// カメラマネージャーテスト

	auto& cameraManager = CameraManager::GetInstance();

	// カメラの作成
	auto camera1 = std::make_shared<Camera>();
	auto camera2 = std::make_shared<Camera>();

	Vector3 camera1Rotate = camera1->GetRotate();
	Vector3 camera1Position = camera1->GetPosition();
	camera1Position.z = -5.0f;
	Vector3 camera2Rotate = camera2->GetRotate();
	Vector3 camera2Position = camera2->GetPosition();
	camera2Position.z = -10.0f;

	// カメラの追加
	cameraManager.AddCamera(camera1);
	cameraManager.AddCamera(camera2);

	// アクティブカメラの設定
	cameraManager.SetActiveCamera(0);
	uint32_t activeIndex = cameraManager.GetActiveIndex();

	Object3dCommon::GetInstance()->SetDefaultCamera(cameraManager.GetActiveCamera());


	//////////////////////////////////////



	//ModelManager::GetInstance()->LoadModel("plane.obj");
	//ModelManager::GetInstance()->LoadModel("axis.obj");

	//// 3Dオブジェクトの初期化
	//std::vector<Object3d*>object3ds;

	//for (uint32_t i = 0; i < 2; ++i) {
	//	Object3d* object = new Object3d();
	//	if (i == 0) {
	//		object->Initialize("plane.obj");
	//	}
	//	if (i == 1) {
	//		object->Initialize("axis.obj");
	//	}

	//	Vector3 position;
	//	position.x = i * 2.0f;
	//	object->SetPosition(position);

	//	object3ds.push_back(object);
	//}







	//////////////////////////////////////
	

	std::vector<Sprite*> sprites;
	for (uint32_t i = 0; i < 1; ++i)
	{
		Sprite* sprite = new Sprite();
		if (i == 0 || i == 3) {
			sprite->Initialize(spriteCommon, "uvChecker.png");
		}
		else {
			sprite->Initialize(spriteCommon, "monsterBall.png");
		}
		sprites.push_back(sprite);

		position = sprite->GetPosition();
		// 座標を変更する
		position.x = 100.0f;
		position.y = 100.0f;
		// 変更を反映する
		sprite->SetPosition(position);

		Vector2 size = sprite->GetSize();
		size.x = 370.0f;
		size.y = 370.0f;
		sprite->SetSize(size);
	}

	//////////////////////////////////////

}

void MyGame::Finalize()
{
}

void MyGame::Update()
{
}

void MyGame::Draw()
{
}
