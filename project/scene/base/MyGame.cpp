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
	/*particleManager->CreateParticleGroup("RingGroup", "resources/images/monsterBall.png", "plane.obj", "Ring");
	particleManager->CreateParticleGroup("CylinderGroup", "resources/images/gradationLine.png", "plane.obj", "Cylinder");
	particleManager->CreateParticleGroup("ConeGroup", "resources/images/gradationLine.png", "plane.obj", "Cone");
	particleManager->CreateParticleGroup("SpiralGroup", "resources/images/gradationLine.png", "plane.obj", "Spiral");
	particleManager->CreateParticleGroup("TorusGroup", "resources/images/gradationLine.png", "plane.obj", "Torus");
	particleManager->CreateParticleGroup("HelixGroup", "resources/images/gradationLine.png", "plane.obj", "Helix");*/

	
	// Cylinderを出すときに向き指定する
	ParticleMotion::SetDirection("UP");

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

	particleManager->DebugUI();

#endif // _DEBUG

}

void MyGame::Draw()
{
	//ゲームの処理		描画処理

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
