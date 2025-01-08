#include "scene/base/MyGame.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyGame game;

	game.Initialize();


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
			if (Input::GetInstance()->TriggerKey(DIK_0))
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


#ifdef _DEBUG
			// ImGui終了
			imGuiManager->End();
#endif // _DEBUG

			

			//ゲームの処理		描画処理








			//コマンドを積み込んで確定させる


			srvManager->PreDraw();

			dxCommon->PreDraw();


			// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
			//Object3dCommon::GetInstance()->CommonDrawSetting();
			Object3dCommon::GetInstance()->CommonDrawSetting();

			for (Object3d* object3d : object3ds)
			{
				object3d->Draw();
			}

			//いざ描画

			//commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
			//
			////wvp用のCBufferの場所を設定
			//commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

			////commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
			////描画！
			//commandList->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);


			


			spriteCommon->CommonDrawSetting();

			for (Sprite* sprite : sprites)
			{
				sprite->Draw();
			}

#ifdef _DEBUG
			// ImGui描画
			imGuiManager->Draw();
#endif // _DEBUG


			dxCommon->PostDraw();

		}
	}

	
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