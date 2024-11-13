#pragma once

#include <d3d12.h>
#include "externals/DirectXTex/d3dx12.h"

#include "DirectXCommon.h"
#include "Logger.h"

class Object3dCommon
{
public:

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	// ルートシグネイチャの作成
	void CreateRootSignature();

	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();

	// 共通描画設定
	void CommonDrawSetting();

	//-----------ゲッター----------//

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

private:

	DirectXCommon* dxCommon_;

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

	//RootSignature作成
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//PSOを生成する
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

};

