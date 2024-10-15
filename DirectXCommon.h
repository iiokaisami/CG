#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <array>
#include <format>
#include <chrono>

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

//DirectX12
#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"
#include "StringUtility.h"
#include "Logger.h"

class DirectXCommon
{
public:

	// 初期化
	void Initialize(WinApp* winApp);

	/// <summary>
	/// デバイスの初期化
	/// </summary>
	void InitializeDevice();

	/// <summary>
	/// コマンド関連初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// 各種デスクリプタヒープの生成
	/// </summary>
	void CreateDescriptorHeap();

	/// <summary>
	/// デスクリプタヒープを生成する
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptor, bool shaderVisible);

	/// <summary>
	/// レンダーターゲット初期化
	/// </summary>
	void InitializeFinalRenderTargets();

	/// <summary>
	/// CPUのDescriptorHandleを取得
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// GPUのDescriptorHandleを取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// SRVの指定番号のCPUデスクリプタハンドルを取得する
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// SRVの指定番号のGPUデスクリプタハンドルを取得する
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// 深度ステンシルビューの初期化
	/// </summary>
	void InitializeDepthStencilView();

	/// <summary>
	/// フェンス生成
	/// </summary>
	void CreateFence();

	/// <summary>
	/// ビューポート矩形の初期化
	/// </summary>
	void InitializeViewPort();

	/// <summary>
	/// シザリング矩形の設定
	/// </summary>
	void InitializeScissor();

	/// <summary>
	/// DXCコンパイラの生成
	/// </summary>
	void CreateDXCompiler();

	/// <summary>
	/// ImGuiの初期化
	/// </summary>
	void InitializeImGui();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// シェーダーのコンパイル
	/// </summary>
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

	/// <summary>
	/// バッファリソースの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// テクスチャリソースの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata);

	/// <summary>
	/// テクスチャデータの転送
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

	/// <summary>
	/// 
	/// </summary>


	//-----------ゲッター----------//

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device_; }
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator() { return commandAllocator_; }
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return commandList_; }
	// DescriptorHeap
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSrvDescriptorHeap() { return srvDescriptorHeap_; }
	// descriptorSizeSRV
	uint32_t GetDescriptorSizeSRV() { return descriptorSizeSRV_; }
	// ビューポート
	D3D12_VIEWPORT GetViewport() { return viewport_; }
	// シザー矩形
	D3D12_RECT GetScissorRect() { return scissorRect_; }

private:

	// FPS固定初期化
	void InitializeFixFPS();

	// FPS固定更新
	void UpdateFixFPS();

private:

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;


	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	// スワップチェイン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	// デスクリプタヒープ
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
	// resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_ = nullptr;
	// スワップチェイン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	// RTV
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	// バリア
	D3D12_RESOURCE_BARRIER barrier_{};

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;

	HANDLE fenceEvent_ = nullptr;

	 uint32_t descriptorSizeSRV_ = 0;
	 uint32_t descriptorSizeRTV_ = 0;
	 uint32_t descriptorSizeDSV_ = 0;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;

	// RTVを2つ作るのでディスクリプタを２つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2]{};

	// スワップチェーンリソース
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources_;

	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_ = 0;

	// DXC
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;

	// ビューポート
	D3D12_VIEWPORT viewport_{};
	// シザー矩形
	D3D12_RECT scissorRect_{}; 

	// 記録時間（FPS固定）
	std::chrono::steady_clock::time_point reference_;
};