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

	ParticleManager* particleManager = nullptr;

#ifdef _DEBUG

	std::unique_ptr<ImGuiManager> imGuiManager = nullptr;

#endif // _DEBUG
};

