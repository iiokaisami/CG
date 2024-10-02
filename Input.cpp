#include "Input.h"

#include <cassert>



void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT hr; /* = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));*/

	// DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスの生成
	
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));

	// 入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	// 排他的制御レベルのセット
	hr = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}

void Input::Update()
{
	// キーボード情報の取得開始
	keyboard->Acquire();
	// 全キーの入力状態を取得する
	BYTE key[256] = {};
	keyboard->GetDeviceState(sizeof(key), key);
}
