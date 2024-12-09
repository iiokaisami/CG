#pragma once

#include "DirectXCommon.h"

class ImGuiManager
{
public:

	// 初期化
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	// 終了
	void Finalize();

	// ImGui受付開始
	void Begin();

	// ImGui受付終了
	void End();

	// 画面への描画
	void Draw();

private:

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;

};

