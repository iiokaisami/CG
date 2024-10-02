#pragma once
#include <windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

//Input
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"

class Input
{
public:

	// namespace省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	// 初期化
	void Initialize(WinApp* winApp);
	// 更新
	void Update();

    /// <summary>
    /// キーの押下をチェック
    /// </summary>
    /// <perm name="keyNumber">キー番号( DIK_0 等)</param>
	///<returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

    /// <summary>
    /// キーのトリガーをチェック
    /// </summary>
	/// <param name="KeyNumber">キー番号( DIK_0 等 )</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

private:

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;

	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard = nullptr;

	// 全キーの状態
	BYTE key[256] = {};
	// 前回の全キーの状態
	BYTE keyPre[256] = {};
};

