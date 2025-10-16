#pragma once

#include <d3d12.h>
//#include "externals/DirectXTex/d3dx12.h"
#include "../../externals/DirectXTex/d3dx12.h"
#include "DirectXCommon.h"
#include "Logger.h"

/// <summary>
/// モデル共通
/// </summary>
class ModelCommon
{
public:

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

public: // ゲッター
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

private:

	DirectXCommon* dxCommon_;


};

