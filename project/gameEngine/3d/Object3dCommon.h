#pragma once

#include <d3d12.h>
#include "../../externals/DirectXTex/d3dx12.h"

#include "DirectXCommon.h"
#include "Logger.h"
#include "CameraManager.h"

class Object3dCommon
{
#pragma region シングルトンインスタンス

private:
	static Object3dCommon* instance;

	Object3dCommon() = default;
	~Object3dCommon() = default;
	Object3dCommon(Object3dCommon&) = delete;
	Object3dCommon& operator = (Object3dCommon&) = delete;

public:
	// シングルトンインスタンスの取得
	static Object3dCommon* GetInstance();
	// 終了
	void Finalize();

#pragma endregion シングルトンインスタンス

public:

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	// 共通描画設定
	void CommonDrawSetting();

	//-----------ゲッター----------//

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

public: // セッター

	void SetDefaultCamera(std::shared_ptr<Camera> camera) { defaultCamera_ = camera; }

public: // ゲッター

	std::shared_ptr<Camera> GetDefaultCamera() const { return defaultCamera_; }

private:

	// ルートシグネイチャの生成
	void CreateRootSignature();

	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();

private:

	DirectXCommon* dxCommon_;

	std::shared_ptr<Camera> defaultCamera_ = nullptr;
	
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

	//ディスクリプタレンジの生成
	D3D12_DESCRIPTOR_RANGE descriptorRange_[1]{};
	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
	//InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc_{};
	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	//shaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
	

	//RootSignature作成
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//PSOを生成する
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;


	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	D3D12_ROOT_PARAMETER rootParameters_[6] = {};
	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1] = {};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
};

