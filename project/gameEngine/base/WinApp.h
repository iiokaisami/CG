#pragma once

#include <windows.h>
#include <cstdint>

//ImGui
#include "../../externals/imgui/imgui.h"
#include "../../externals/imgui/imgui_impl_dx12.h"

/// <summary>
/// Windowsアプリケーションクラス
/// </summary>
class WinApp
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:

	//クライアント領域のサイズ
	static const int32_t kClientWidth = 1200;
	static const int32_t kClientHeight = 720;


	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 終了
	void Finalize();

	// getter
	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return wc.hInstance; }

	// メッセージの処理
	bool ProcessMessage();


private:

	// ウィンドウハンドル
	HWND hwnd = nullptr;

	//ウィンドウクラスの設定
	WNDCLASS wc{};

};