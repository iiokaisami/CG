#pragma once

#include <Windows.h>
#include <format>
#include <cassert>
#include <numbers>
#include <vector>
#include <DirectXMath.h>
#include <fstream>
#include <sstream>
#include <iostream>

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
#include "Audio.h"
#include "RenderTexture.h"

#include "postEffect/PostEffectManager.h"
#include "postEffect/GrayscalePass.h"
#include "postEffect/VignettePass.h"
#include "postEffect/BoxFilterPass.h"

#include "../../ParticleEmitter.h"

#include "../../scene/base/SceneManager.h"
#include "../../scene/base/SceneFactory.h"   

#ifdef _DEBUG

#include "../../ImGuiManager.h"
#include "../../imgui/imgui.h"

#endif // _DEBUG

class Framework
{
public:

	virtual ~Framework() = default;

	void Run();

	// 初期化
	virtual void Initialize();
	
	// 終了
	virtual void Finalize();
	
	// 更新
	virtual void Update();
	
	// 描画	
	virtual void Draw() = 0;

	// 終了チェック
	virtual bool IsEndRequest() const { return winApp->ProcessMessage(); }

protected:

	// ポインタ
	std::unique_ptr<WinApp> winApp = nullptr;
	std::unique_ptr<DirectXCommon> dxCommon = nullptr;
	std::unique_ptr<SrvManager> srvManager = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	SceneManager* sceneManager_ = nullptr;
	AbstractSceneFactory* sceneFactory_ = nullptr;

	SpriteCommon* spriteCommon = nullptr;
	TextureManager* textureManager = nullptr;
	Object3dCommon* object3dCommon = nullptr;
	ModelManager* modelManager = nullptr;

	std::unique_ptr <RenderTexture> renderTexture = nullptr;

	std::unique_ptr<PostEffectManager> postEffectManager = nullptr;
	std::unique_ptr<GrayscalePass> grayscalePass = nullptr;
	std::unique_ptr<VignettePass> vignettePass = nullptr;
	std::unique_ptr<BoxFilterPass> boxFilterPass = nullptr;

	ParticleManager* particleManager = nullptr;

	D3D12_GPU_DESCRIPTOR_HANDLE inputSrv;
	Microsoft::WRL::ComPtr<ID3D12Resource> inputRes;
	D3D12_RESOURCE_STATES state;

#ifdef _DEBUG

	std::unique_ptr<ImGuiManager> imGuiManager = nullptr;

#endif // _DEBUG
};

