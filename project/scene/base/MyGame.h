#pragma once

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

/// パーティクルテスト///

#include "../ParticleManager.h"
#include "../ParticleEmitter.h"
#include <iostream>

/////////////////////////

#ifdef _DEBUG

#include "../ImGuiManager.h"
#include "../imgui/imgui.h"

#endif // _DEBUG

class MyGame
{
public:

	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 更新
	void Update();

	// 描画	
	void Draw();

private:


	// ポインタ
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	ModelCommon* modelCommon = nullptr;
	SrvManager* srvManager = nullptr;


#ifdef _DEBUG

	ImGuiManager* imGuiManager = nullptr;

#endif // _DEBUG



	Vector2 position{};


	bool isFilipX = false;
	bool isFilipY = false;

	Vector2 textureLeftTop{};
	Vector2 textureSize{ 500.0f ,500.0f };

};

