#include <Windows.h>

#include <format>
#include <cassert>
#include <numbers>
#include <vector>
#include <DirectXMath.h>
#include <fstream>
#include <sstream>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "Object3dCommon.h"
#include "Object3d.h"
#include "ModelCommon.h"
#include "Model.h"
#include "ModelManager.h"
#include "SrvManager.h"

#ifdef _DEBUG

#include "ImGuiManager.h"
#include "imgui/imgui.h"

#endif // _DEBUG

#include "./objects//Player//Player.h"
#include "./objects/Skydome/Skydome.h"


class ResourceObject
{
public:
	ResourceObject(ID3D12Resource* resource)
		:resource_(resource)
	{}

	~ResourceObject()
	{
		if (resource_)
		{
			resource_->Release();
		}
	}

	ID3D12Resource* Get() { return resource_; }

private:
	ID3D12Resource* resource_;
};



//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	//D3DResourceLeakChecker leakCheck;


	// ポインタ
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	ModelCommon* modelCommon = nullptr;
	SrvManager* srvManager = nullptr;

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

	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp, dxCommon);

#endif // _DEBUG

	// ゲームで使うモデル
	std::unique_ptr<Player>pPlayer_ = nullptr;
	std::unique_ptr<Skydome>pSkydome_ = nullptr;

	




	// テクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxCommon,srvManager);


	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	hr;


	
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



	////////////////////////////////////////////////

	// Player
	pPlayer_ = std::make_unique<Player>();
	pPlayer_->Initialize();
	pPlayer_->SetCamera(cameraManager.GetActiveCamera());

	// Skydome
	pSkydome_ = std::make_unique<Skydome>();
	pSkydome_->Initialize();

	//////////////////////////////////////////////////


	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	// 3Dオブジェクトの初期化
	std::vector<Object3d*>object3ds;

	for (uint32_t i = 0; i < 2; ++i) {
		Object3d* object = new Object3d();
		if (i == 0) {
			object->Initialize("plane.obj");
		}
		if (i == 1) {
			object->Initialize("axis.obj");
		}

		Vector3 position;
		position.x = i * 2.0f;
		object->SetPosition(position);

		object3ds.push_back(object);
	}

	

	

	//////////////////////////////////////
	Vector2 position{};

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



	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	

	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxCommon->GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxCommon->GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);



	bool isFilipX = false;
	bool isFilipY = false;

	Vector2 textureLeftTop{};
	Vector2 textureSize{ 500.0f ,500.0f};

	

	//メインループ
	//ウィンドウのxボタンが押されるまでループ
	while (true)
	{

		// Windowsのメッセージ処理
		if (winApp->ProcessMessage())
		{
			// ゲームループを抜ける
			break;
		}
		else
		{

#ifdef _DEBUG
			// ImGui開始
			imGuiManager->Begin();

#endif // _DEBUG

			

			// 入力の更新
			Input::GetInstance()->Update();
			// 数字の0キーが押されていたら
			if (Input::GetInstance()-> TriggerKey(DIK_0))
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
			if (Input::GetInstance()->TriggerKey(DIK_RETURN))
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






			uint32_t i = 1;
			for (Object3d* object3d : object3ds)
			{
				object3d->Update();


				Vector3 pos{},pos2{},rotate{}, rotate2{};

				pos2 = { 1.0f,-1.0f,1.0f };

				if (i == 1)
				{
					rotate=object3d->GetRotate();
					rotate.y += 0.1f;
					object3d->SetPosition(pos);
					object3d->SetRotate(rotate);
				}
				else
				{
					rotate2 = object3d->GetRotate();
					rotate2.z += 0.05f;
					object3d->SetPosition(pos2);
					object3d->SetRotate(rotate2);
				}
				i++;

				// カメラ切り替え
				object3d->SetCamera(cameraManager.GetActiveCamera());
			}
			
			


			for (Sprite* sprite : sprites)
			{
				sprite->Update();
				sprite->SetPosition(position);
				sprite->SetFlipX(isFilipX);
				sprite->SetFlipY(isFilipY);
				sprite->SetTextureLeftTop(textureLeftTop);
				sprite->SetTextureSize(textureSize);
			}

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

			////////////////////////////////////////////////////

			pPlayer_->Update();
			pSkydome_->Update();
			//pSkydome_->SetCamera(cameraManager.GetActiveCamera());

			////////////////////////////////////////////////////


#ifdef _DEBUG
			// ImGui終了
			imGuiManager->End();
#endif // _DEBUG

			

			//ゲームの処理		描画処理








			//コマンドを積み込んで確定させる


			srvManager->PreDraw();

			dxCommon->PreDraw();


			// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
			Object3dCommon::GetInstance()->CommonDrawSetting();
			
			////////////////////////////////////////////////

		    // 天球描画
		    pSkydome_->Draw();
		    // プレーヤー描画
			pPlayer_->Draw();

			///////////////////////////////////////////////

			for (Object3d* object3d : object3ds)
			{
				object3d->Draw();
			}


			spriteCommon->CommonDrawSetting();

			for (Sprite* sprite : sprites)
			{
				sprite->Draw(textureSrvHandleGPU);
			}

		

#ifdef _DEBUG
			// ImGui描画
			imGuiManager->Draw();
#endif // _DEBUG


			dxCommon->PostDraw();

		}
	}

	//////////////////////////////////////////

	pPlayer_->Finalize();
	pSkydome_->Finalize();

	/////////////////////////////////////////

	
	// テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();

	// 入力解放
	Input::GetInstance()->Finalize();
	
	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;

	// スプライト共通部分解放
	delete spriteCommon;

	// DirectX解放
	delete dxCommon;

	// スプライト解放
	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}

	// 3Dオブジェクト共通部解放
	Object3dCommon::GetInstance()->Finalize();
	

	// 3Dオブジェクト解放
	for (Object3d* object3d : object3ds)
	{
		delete object3d;
	}

	// モデル共通部分解放
	delete modelCommon;

	// モデル解放
	//delete model;
	
	// 3Dモデルマネージャー解放
	ModelManager::GetInstance()->Finalize();

	// カメラ解放
	cameraManager.RemoveCamera(0);
	cameraManager.RemoveCamera(1);

	// SRVマネージャー解放
	delete srvManager;



#ifdef _DEBUG
	// ImGuiManager解放
	imGuiManager->Finalize();
	delete imGuiManager;
#endif // _DEBUG

	



	//CloseHandle(fenceEvent);
	

	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}

	return 0;
}