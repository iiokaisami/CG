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
	winApp = std::make_unique<WinApp>();
	winApp->Initialize();

	// DirectXの初期化
	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());

	// SRVマネージャーの初期化
	srvManager = std::make_unique<SrvManager>();
	srvManager->Initialize(dxCommon.get());

	// キーボード入力
	input = Input::GetInstance();
	input->Initialize(winApp.get());

	// オーディオ
	audio = Audio::GetInstance();
	audio->Initialize();

	// シーンマネージャ
	sceneManager_ = SceneManager::GetInstance();

	// スプライト共通部分の初期化
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxCommon.get());

	// テクスチャマネージャー
	textureManager = TextureManager::GetInstance();
	textureManager->Initialize(dxCommon.get(), srvManager.get());

	// 3Dオブジェクト
	object3dCommon = Object3dCommon::GetInstance();
	object3dCommon->Initialize(dxCommon.get());

	// モデル共通部分の初期化
	modelManager = ModelManager::GetInstance();
	modelManager->Initialize(dxCommon.get());

#ifdef _DEBUG

	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApp.get(), dxCommon.get());

#endif // _DEBUG
}

void Framework::Finalize()
{
	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	winApp.reset();
	winApp = nullptr;

	// DirectX解放
	dxCommon.reset();

	// SRVマネージャー解放
	srvManager.reset();

	sceneManager_->Finalize();
	delete sceneFactory_;

	input->Finalize();

	audio->Finalize();

	// スプライト共通部分解放
	spriteCommon->Finalize();

	textureManager->Finalize();

	object3dCommon->Finalize();

	modelManager->Finalize();

#ifdef _DEBUG
	// ImGuiManager解放
	imGuiManager->Finalize();
	imGuiManager.reset();
#endif // _DEBUG
}

void Framework::Update()
{
	// シーンマネージャーの更新
	sceneManager_->Update();

	// 入力更新
	input->Update();
}
