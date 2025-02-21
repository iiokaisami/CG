#include "MyGame.h"

#include <thread>

void MyGame::Initialize()
{
	Framework::Initialize();

	// シーンマネージャに最初のシーンをセット
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);

	// 最初のシーンを設定
	SceneManager::GetInstance()->ChangeScene("TITLE");



	// モデルの読み込み処理をスレッドで実行
	std::thread loadModelThread(&MyGame::LoadModel, this);
	std::thread loadAudioThread(&MyGame::LoadSound, this);

	// スレッドの終了を待つ
	loadModelThread.join(); 
	loadAudioThread.join();

	// パーティクルグループの生成
	particleManager->CreateParticleGroup("exampleGroup", "resources/images/uvChecker.png", "plane.obj");
	particleManager->CreateParticleGroup("secondGroup", "resources/images/monsterBall.png", "plane.obj");

	useExampleGroup_ = true;
}

void MyGame::Finalize()
{
	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}


	delete sceneFactory_;
	sceneFactory_ = nullptr;

	Framework::Finalize();

}

void MyGame::Update()
{
	Framework::Update();



#ifdef _DEBUG

	if (ImGui::CollapsingHeader("particleManager"))
	{
		ImGui::Checkbox("Use Example Group", &useExampleGroup_);
	}

#endif // _DEBUG


	// パーティクルの生成
	if (useExampleGroup_)
	{
		particleManager->Emit("exampleGroup", Vector3(0.0f, 0.0f, 0.0f), 10);
	} else
	{
		particleManager->Emit("secondGroup", Vector3(0.0f, 0.0f, 0.0f), 10);
	}

}

void MyGame::Draw()
{

	//ゲームの処理		描画処理

	//コマンドを積み込んで確定させる


	srvManager->PreDraw();

	dxCommon->PreDraw();

	sceneManager_->Draw();

	particleManager->Draw();

#ifdef _DEBUG
	// ImGui描画
	imGuiManager->Draw();
#endif // _DEBUG


	dxCommon->PostDraw();

}

void MyGame::LoadModel()
{
	ModelManager::GetInstance()->LoadModel("cube.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("plane.obj");
}

void MyGame::LoadSound()
{
	soundData_ = Audio::GetInstance()->LoadWav("fanfare.wav");
	soundData2_ = Audio::GetInstance()->LoadWav("BGM.wav");
}
