#include "Framework.h"

void Framework::Run()
{
	Initialize();

	while (true)
	{
		// 終了リクエストが来たら抜ける
		if (IsEndRequest())
		{
			break;
		}

#ifdef _DEBUG
		// ImGui開始
		imGuiManager->Begin();

#endif // _DEBUG

		// 毎フレーム更新
		Update();

#ifdef _DEBUG
		// ImGui終了
		imGuiManager->End();
#endif // _DEBUG

		// 描画
		Draw();
	}

	// ゲームの終了
	Finalize();

}

void Framework::Initialize()
{
	// WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	// DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	// キーボード入力
	input = Input::GetInstance();
	input->Initialize(winApp);

	// SRVマネージャーの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	// オーディオ
	audio = Audio::GetInstance();
	audio->Initialize();

	// シーンマネージャ
	sceneManager_ = SceneManager::GetInstance();

	// スプライト共通部分の初期化
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxCommon);

	// テクスチャマネージャー
	textureManager = TextureManager::GetInstance();
	textureManager->Initialize(dxCommon, srvManager);

	// 3Dオブジェクト
	object3dCommon = Object3dCommon::GetInstance();
	object3dCommon->Initialize(dxCommon);

	// モデル共通部分の初期化
	modelManager = ModelManager::GetInstance();
	modelManager->Initialize(dxCommon);

	// パーティクル	
	particleManager = ParticleManager::GetInstance();
	particleManager->Initialize(dxCommon, srvManager);

#ifdef _DEBUG

	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp, dxCommon);

#endif // _DEBUG
}

void Framework::Finalize()
{
	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;
	winApp = nullptr;

	// DirectX解放
	delete dxCommon;

	input->Finalize();

	// SRVマネージャー解放
	delete srvManager;

	sceneManager_->Finalize();
	delete sceneFactory_;

	audio->Finalize();

	// スプライト共通部分解放
	spriteCommon->Finalize();

	textureManager->Finalize();

	object3dCommon->Finalize();

	modelManager->Finalize();

#ifdef _DEBUG
	// ImGuiManager解放
	imGuiManager->Finalize();
	delete imGuiManager;
#endif // _DEBUG
}

void Framework::Update()
{
	// シーンマネージャーの更新
	sceneManager_->Update();

	// 入力更新
	input->Update();

	// パーティクル更新
	particleManager->Update();
}
